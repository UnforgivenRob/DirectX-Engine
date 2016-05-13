#include "GraphicsEngine.h"
#include <assert.h>

GraphicsEngine GraphicsEngine::engine = GraphicsEngine();

GraphicsEngine::GraphicsEngine()
	: msaaQuality(0), Device(0), Context(0), SwapChain(0), DepthStencil(0), RenderTargetView(0), DepthStencilView(0)
{
	ZeroMemory( &engine.Viewport, sizeof( D3D11_VIEWPORT ) );
}

GraphicsEngine::~GraphicsEngine()
{
}

void GraphicsEngine::Activate( HINSTANCE mApp, const char* caption, bool bEnable4xMsaa, int clientWidth, int clientHeight )
{
	engine.mClientWidth = clientWidth;
	engine.mClientHeight = clientHeight;

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, engine.mClientWidth, engine.mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	engine.mMainWnd = CreateWindow( "D3DWndClassName", caption, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mApp, 0 ); 
	if( !engine.mMainWnd )
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
	}

	ShowWindow(engine.mMainWnd, SW_SHOW);
	UpdateWindow(engine.mMainWnd);

	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	featureLevels;

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT res = D3D11CreateDevice( 0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &engine.Device, &featureLevel , &engine.Context );

	if( FAILED(res) )
	{
		MessageBox( 0, "D3D11CreateDevice Failed\n", 0, 0 );
	}

	res = engine.Device->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &engine.msaaQuality );
	assert( res == S_OK );
	assert( engine.msaaQuality > 0 );

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if( bEnable4xMsaa )
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = engine.msaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.OutputWindow = engine.mMainWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice1* dxgiDevice = 0;
	res = engine.Device->QueryInterface( __uuidof(IDXGIDevice), (void**)&dxgiDevice );
	assert( res == S_OK );
	
	IDXGIAdapter* dxgiAdapter = 0;
	res = dxgiDevice->GetParent( __uuidof(IDXGIAdapter), (void**)&dxgiAdapter );
	assert( res == S_OK );

	IDXGIFactory* dxgiFactory = 0;
	res = dxgiAdapter->GetParent( __uuidof(IDXGIFactory), (void**)&dxgiFactory );
	assert( res == S_OK );

	res = dxgiFactory->CreateSwapChain(engine.Device, &swapChainDesc, &engine.SwapChain );
	assert( res == S_OK );

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize();
}

void GraphicsEngine::Deactivate()
{
	ReleaseCOM(engine.RenderTargetView);
	ReleaseCOM(engine.DepthStencilView);
	ReleaseCOM(engine.SwapChain);
	ReleaseCOM(engine.DepthStencil);

	if( engine.Context )
	{
		engine.Context->ClearState();
	}

	ReleaseCOM(engine.Context);
	ReleaseCOM(engine.Device);
}

void GraphicsEngine::OnResize()
{
	assert( engine.Context );
	assert( engine.Device );
	assert( engine.SwapChain );

	ReleaseCOM( engine.RenderTargetView );
	ReleaseCOM( engine.DepthStencilView );
	ReleaseCOM( engine.DepthStencil );

	HRESULT res = engine.SwapChain->ResizeBuffers( 2, engine.mClientWidth, engine.mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0 );
	assert( res == S_OK );

	ID3D11Texture2D* backBuffer;
	res = engine.SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	assert( res == S_OK );
	res = engine.Device->CreateRenderTargetView( backBuffer, 0, &engine.RenderTargetView );
	assert( res == S_OK );
	ReleaseCOM( backBuffer );

	
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = engine.mClientWidth;
	depthStencilDesc.Height = engine.mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if( engine.mEnable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = engine.msaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	res = engine.Device->CreateTexture2D( &depthStencilDesc, 0, &engine.DepthStencil );
	assert( res == S_OK );
	res = engine.Device->CreateDepthStencilView( engine.DepthStencil, 0, &engine.DepthStencilView );
	assert( res == S_OK );

	engine.Context->OMSetRenderTargets( 1, &engine.RenderTargetView, engine.DepthStencilView );

	engine.Viewport.TopLeftX = 0;
	engine.Viewport.TopLeftY = 0;
	engine.Viewport.Width    = static_cast<float>(engine.mClientWidth);
	engine.Viewport.Height   = static_cast<float>(engine.mClientHeight);
	engine.Viewport.MinDepth = 0.0f;
	engine.Viewport.MaxDepth = 1.0f;

	engine.Context->RSSetViewports( 1, &engine.Viewport );
}

void GraphicsEngine::ClearBuffers( Vect& bgColor )
{
	assert( engine.Context );
	assert( engine.SwapChain );

	engine.Context->ClearRenderTargetView(engine.RenderTargetView, &bgColor[x] );
	engine.Context->ClearDepthStencilView(engine.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

ID3D11Device* GraphicsEngine::getDevice()
{
	return engine.Device;
}
ID3D11DeviceContext* GraphicsEngine::getContext()
{
	return engine.Context;
}

IDXGISwapChain* GraphicsEngine::getSwapChain()
{
	return engine.SwapChain;
}
