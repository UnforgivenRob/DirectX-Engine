#include "GraphicsEngine.h"
#include <assert.h>
#include "d3d12.h"
#include "d3dx12.h"

GraphicsEngine GraphicsEngine::engine = GraphicsEngine();

GraphicsEngine::GraphicsEngine()
	: msaaQuality(0), Device(0), swapChain(0), depthStencil(0)
{
	ZeroMemory( &engine.Viewport, sizeof( D3D12_VIEWPORT ) );
}

GraphicsEngine::~GraphicsEngine()
{
}

void GetHardwareAdapter(IDXGIFactory4 * pFactory, IDXGIAdapter1 ** ppAdapter)
{
	*ppAdapter = nullptr;
	for (UINT adpterIndex = 0; ; ++adpterIndex)
	{
		IDXGIAdapter1* pAdapter = nullptr;

		if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adpterIndex, &pAdapter))
		{
			break; // no more adapters to enumerate
		}

		//check to see if adapter support D3D 12, dont create actual device
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			*ppAdapter = pAdapter;
			return;
		}

		pAdapter->Release();
	}
}

void GraphicsEngine::Activate( HINSTANCE mApp, const char* caption, bool bEnable4xMsaa, int clientWidth, int clientHeight )
{
	bEnable4xMsaa;
	engine.mClientWidth = clientWidth;
	engine.mClientHeight = clientHeight;

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, engine.mClientWidth, engine.mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	engine.mMainWnd = CreateWindow( "D3DWndClassName", caption, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mApp, &engine ); 
	if( !engine.mMainWnd )
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
	}

	ShowWindow(engine.mMainWnd, SW_SHOW);
	UpdateWindow(engine.mMainWnd);

//	UINT createDeviceFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif

	ComPtr<IDXGIFactory4> factory;
	HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	if (FAILED(res))
	{
		MessageBox(0, "CreateDXGIFactory1 Failed\n", 0, 0);
	}

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	featureLevels;

#ifdef USE_WARP
	ComPtr<IDXGIAdapter> warpAdapter;
	res = factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
	assert(res == S_OK);
	res = D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&engine.Device));
	if (FAILED(res))
	{
		MessageBox(0, "D3D12CreateDevice Failed\n", 0, 0);
	}
#else
	ComPtr<IDXGIAdapter1> hardwareAdapter;
	GetHardwareAdapter( factory.Get(), &hardwareAdapter );
	assert(res == S_OK);
	res = D3D12CreateDevice( hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&engine.Device));
	if (FAILED(res))
	{
		MessageBox(0, "D3D12CreateDevice Failed\n", 0, 0);
	}
#endif

	//create command queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	res = engine.Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&engine.commandQueue) );

	if (FAILED(res))
	{
		MessageBox(0, "Create Command Queue Failed\n", 0, 0);
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	res = engine.Device->CheckFeatureSupport( D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels) );
	assert( res == S_OK );

	engine.msaaQuality = msQualityLevels.NumQualityLevels;
	assert( engine.msaaQuality > 0 );

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = engine.mClientWidth;
	swapChainDesc.Height = engine.mClientHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	
	ComPtr<IDXGISwapChain1> swapChain;

	res = factory->CreateSwapChainForHwnd(engine.commandQueue.Get(), engine.mMainWnd, &swapChainDesc, nullptr, nullptr, &swapChain);
	assert(res == S_OK);

	res = factory->MakeWindowAssociation(engine.mMainWnd, DXGI_MWA_NO_ALT_ENTER);
	assert(res == S_OK);

	res = swapChain.As(&engine.swapChain);
	assert(res == S_OK);
	
	engine.frameIndex = engine.swapChain->GetCurrentBackBufferIndex();

	{
		D3D12_DESCRIPTOR_HEAP_DESC descriptHeapDesc = {};
		descriptHeapDesc.NumDescriptors = FrameCount;
		descriptHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		res = engine.Device->CreateDescriptorHeap(&descriptHeapDesc, IID_PPV_ARGS(&engine.rtvDescriptorHeap));
		assert(res == S_OK);

		engine.descriptorSize = engine.Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(engine.rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT i = 0; i < FrameCount; i++)
		{
			res = engine.swapChain->GetBuffer(i, IID_PPV_ARGS(&engine.renderTargets[i]));
			assert(res == S_OK);
			engine.Device->CreateRenderTargetView(engine.renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr += engine.descriptorSize;

			res = engine.Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&engine.commandAllocator[i]));
			assert(res == S_OK);
		}
	}

	{
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		res = (D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		assert(res == S_OK);
		
		res = engine.Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&engine.rootSignature));
		assert(res == S_OK);
	}

	//OnResize();
}

void GraphicsEngine::Deactivate()
{
}

void GraphicsEngine::OnResize()
{
	assert( engine.Device );
	assert( engine.swapChain );
	assert(engine.commandAllocator);

	Vect v(0.0f, 0.0f, 0.0f, 0.0f);
	BufferSwap(v);
	
	HRESULT res = engine.commandList->Reset( engine.commandAllocator->Get(), nullptr);

	for ( int i = 0; i < engine.FrameCount; ++i )
	{
		engine.renderTargets[i].Reset();
	}

	engine.depthStencil.Reset();

	res = engine.swapChain->ResizeBuffers( 2, engine.mClientWidth, engine.mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0 );
	assert( res == S_OK );

	engine.frameIndex = 0;

	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(engine.rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT i = 0; i < FrameCount; i++)
		{
			res = engine.swapChain->GetBuffer(i, IID_PPV_ARGS(&engine.renderTargets[i]));
			assert(res == S_OK);
			engine.Device->CreateRenderTargetView(engine.renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr += engine.descriptorSize;
		}
	}

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = engine.mClientWidth;
	depthStencilDesc.Height = engine.mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = engine.mEnable4xMsaa ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = engine.mEnable4xMsaa ? (engine.msaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = (FLOAT)1.0f;
	optClear.DepthStencil.Stencil = 0;

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

	res = engine.Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &optClear, IID_PPV_ARGS(engine.depthStencil.GetAddressOf()) );
	assert(res == S_OK);

	engine.Device->CreateDepthStencilView( engine.depthStencil.Get(), nullptr, engine.dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
	assert( res == S_OK );

	D3D12_RESOURCE_BARRIER transition;
	transition.Transition.pResource = engine.depthStencil.Get();
	transition.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	transition.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	engine.commandList->ResourceBarrier(1, &transition);

	res = engine.commandList->Close();
	assert(res == S_OK);

	ID3D12CommandList* cmdLists[] = { engine.commandList.Get() };
	engine.commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	BufferSwap(v);

	engine.Viewport.TopLeftX = 0.0f;
	engine.Viewport.TopLeftY = 0.0f;
	engine.Viewport.Width    = static_cast<float>(engine.mClientWidth);
	engine.Viewport.Height   = static_cast<float>(engine.mClientHeight);
	engine.Viewport.MinDepth = 0.0f;
	engine.Viewport.MaxDepth = 1.0f;

	engine.scissorRect = { 0, 0, engine.mClientWidth, engine.mClientHeight };
}

void GraphicsEngine::setViewPort( const int posX, const int posY, const int width, const int height )
{
	posX, posY;
	engine.mClientWidth = width;
	engine.mClientHeight = height;
	OnResize();
}

void GraphicsEngine::BufferSwap( Vect& bgColor )
{
	bgColor;
	const UINT64 currentFenceValue = engine.fenceValues[engine.frameIndex];

	HRESULT res = engine.commandQueue->Signal(engine.fence.Get(), currentFenceValue);
	assert(res == S_OK);

	engine.frameIndex = engine.swapChain->GetCurrentBackBufferIndex();

	if (engine.fence->GetCompletedValue() < engine.fenceValues[engine.frameIndex])
	{
		res = engine.fence->SetEventOnCompletion(engine.fenceValues[engine.frameIndex], engine.fenceEvent);
		assert(res == S_OK);
		WaitForSingleObjectEx(engine.fenceValues, INFINITE, FALSE);
	}
		
	engine.fenceValues[engine.frameIndex] = currentFenceValue + 1;
}

ComPtr<ID3D12Device> GraphicsEngine::getDevice()
{
	return engine.Device;
}

ComPtr<IDXGISwapChain3> GraphicsEngine::getSwapChain()
{
	return engine.swapChain;
}

ComPtr<ID3D12RootSignature> GraphicsEngine::getRootSignature()
{
	return engine.rootSignature;
}

ComPtr<ID3D12GraphicsCommandList> GraphicsEngine::getCommandList()
{
	return engine.commandList;
}

ComPtr<ID3D12CommandQueue> GraphicsEngine::getCommandQueue()
{
	return engine.commandQueue;
}

ComPtr<ID3D12CommandAllocator> GraphicsEngine::getCurrentCommandAllocator()
{
	return engine.commandAllocator[engine.frameIndex];
}

ComPtr<ID3D12Resource> GraphicsEngine::getCurrentRenderTarget()
{
	return engine.renderTargets[engine.frameIndex];
}

D3D12_VIEWPORT * GraphicsEngine::getViewport()
{
	return &engine.Viewport;
}

D3D12_RECT * GraphicsEngine::getScissorRect()
{
	return &engine.scissorRect;
}

UINT GraphicsEngine::getCurrentFrameIndex()
{
	return engine.frameIndex;
}

ComPtr<ID3D12DescriptorHeap> GraphicsEngine::getRTVHeap()
{
	return engine.rtvDescriptorHeap;
}

UINT GraphicsEngine::getRTVHeapSize()
{
	return engine.descriptorSize;
}
