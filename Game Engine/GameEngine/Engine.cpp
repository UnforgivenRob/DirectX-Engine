#include "Engine.h"
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
namespace
{
	Engine* app = 0;
}

Engine::Engine()
	: bRun( true ), mEnable4xMsaa( false ), mMainWnd( 0 ), mClientWidth( 800 ), mClientHeight( 600 ), md3dDriverType( D3D_DRIVER_TYPE_HARDWARE ), 
	mCaption( "Game Engine" ),  msaaQuality(0), Device(0), Context(0), SwapChain(0), DepthStencil(0), RenderTargetView(0), DepthStencilView(0)
{
	ZeroMemory( &Viewport, sizeof( D3D11_VIEWPORT ) );
	app = this;
}

Engine::~Engine()
{
	ReleaseCOM(RenderTargetView);
	ReleaseCOM(DepthStencilView);
	ReleaseCOM(SwapChain);
	ReleaseCOM(DepthStencil);

	if( Context )
	{
		Context->ClearState();
	}

	ReleaseCOM(Context);
	ReleaseCOM(Device);
}

void Engine::run()
{
	PreInit();

	Initialize();

	PreLoad();

	LoadContent();

	while( bRun )
	{
		Update();

		ClearBuffers();

		Draw();

		SwapBuffers();
	}

	UnloadContent();
}

void Engine::PreInit()
{
}

void Engine::PreLoad()
{
	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT res = D3D11CreateDevice( 0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, featureLevels, 0, D3D11_SDK_VERSION, &Device, &featureLevel , &Context );

	if( FAILED(res) )
	{
		MessageBox( 0, "D3D11CreateDevice Failed\n", 0, 0 );
	}

	Device->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality );
	assert( msaaQuality > 0 );

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if( mEnable4xMsaa )
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = msaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Windowed = true;
	swapChainDesc.OutputWindow = mMainWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	IDXGIDevice1* dxgiDevice = 0;
	Device->QueryInterface( __uuidof(IDXGIDevice), (void**)&dxgiDevice );
	
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent( __uuidof(IDXGIAdapter), (void**)&dxgiAdapter );

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent( __uuidof(IDXGIFactory), (void**)&dxgiFactory );

	IDXGISwapChain* swapChain;

	dxgiFactory->CreateSwapChain(Device, &swapChainDesc, &swapChain );

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
}

void Engine::ClearBuffers()
{
}
void Engine::SwapBuffers()
{
}
