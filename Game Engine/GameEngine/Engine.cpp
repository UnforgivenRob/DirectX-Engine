#include "Engine.h"
#include <d3d11.h>

Engine::Engine()
	: bRun( true )
{
}

Engine::~Engine()
{
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
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	HRESULT res = D3D11CreateDevice( 0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, featureLevels, 0, D3D11_SDK_VERSION, &Device, &featureLevel , &Context );

	if( FAILED(res) )
	{
		MessageBox( 0, "D3D11CreateDevice Failed\n", 0, 0 );
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //check later whether it should be RGBA
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	IDXGIDevice1* dxgiDevice = (IDXGIDevice1*)Device;
	dxgiDevice->SetMaximumFrameLatency(1);

	IDXGIAdapter* dxgiAdapter;
	dxgiDevice->GetAdapter( &dxgiAdapter );

	IDXGIFactory1* dxgiFactory;
	dxgiAdapter->GetParent( IID_PPV_ARGS(&dxgiFactory) );

	IDXGISwapChain* swapChain;

	dxgiFactory->CreateSwapChain(Device, &swapChainDesc, &swapChain );
}

void Engine::ClearBuffers()
{
}
void Engine::SwapBuffers()
{
}
