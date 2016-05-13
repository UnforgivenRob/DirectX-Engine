#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H
#include <d3d11.h>
#include "MathEngine.h"

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class GraphicsEngine
{
public:
	static void Activate( HINSTANCE mApp, const char* caption, bool bEnable4xMsaa, int clientWindth, int clientHeight );
	static void OnResize();
	static void Deactivate();
	static void ClearBuffers( Vect& bgColor );
	static ID3D11Device* getDevice();
	static ID3D11DeviceContext* getContext();
	static IDXGISwapChain* getSwapChain();

private:
	GraphicsEngine();
	~GraphicsEngine();

	static GraphicsEngine engine;
	
	HWND	  mMainWnd;
	UINT	  msaaQuality;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;
	ID3D11Texture2D* DepthStencil;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
	D3D11_VIEWPORT Viewport;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};

#endif