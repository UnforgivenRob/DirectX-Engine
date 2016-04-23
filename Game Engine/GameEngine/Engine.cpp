#include "Engine.h"
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include <WindowsX.h>

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
namespace
{
	Engine* app = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return app->MsgProc(hwnd, msg, wParam, lParam);
}

LRESULT Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hwnd, wParam, lParam;
	switch( msg )
	{
	case WM_DESTROY:
		return 0;
	default:
		return 0;
	}
}

Engine::Engine( HINSTANCE hInstance )
	: bRun( true ), mEnable4xMsaa( false ), mApp( hInstance ), mMainWnd( 0 ), mClientWidth( 800 ), mClientHeight( 600 ), md3dDriverType( D3D_DRIVER_TYPE_HARDWARE ), 
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
	WNDCLASS wc = {};
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mApp;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mClientWidth, mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	mMainWnd = CreateWindow( "D3DWndClassName", mCaption, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mApp, 0 ); 
	if( !mMainWnd )
	{
		DWORD err = GetLastError(); err;
		MessageBox(0, "CreateWindow Failed.", 0, 0);
	}

	ShowWindow(mMainWnd, SW_SHOW);
	UpdateWindow(mMainWnd);
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

	dxgiFactory->CreateSwapChain(Device, &swapChainDesc, &SwapChain );

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	ID3D11Texture2D* backBuffer;

	SwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&backBuffer );
	Device->CreateRenderTargetView( backBuffer, 0, &RenderTargetView );
	
	ReleaseCOM(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if( mEnable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = msaaQuality - 1;
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

	Device->CreateTexture2D( &depthStencilDesc, 0, &DepthStencil );
	Device->CreateDepthStencilView( DepthStencil, 0, &DepthStencilView );

	Context->OMSetRenderTargets( 1, &RenderTargetView, DepthStencilView );

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width    = static_cast<float>(mClientWidth);
	Viewport.Height   = static_cast<float>(mClientHeight);
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	Context->RSSetViewports( 1, &Viewport );
}

void Engine::ClearBuffers()
{
}
void Engine::SwapBuffers()
{
}
