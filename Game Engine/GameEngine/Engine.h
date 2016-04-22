#ifndef ENGINE_H
#define ENGINE_H

#include <d3d11.h>

class Engine
{
public:
	Engine();
	~Engine();

	void run();

	virtual void Initialize() = 0;
	virtual void LoadContent() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void UnloadContent() = 0;

private:
	void PreLoad();
	void PreInit();
	void ClearBuffers();
	void SwapBuffers();

protected:
	bool bRun;

	HINSTANCE mApp;
	HWND	  mMainWnd;
	UINT	  msaaQuality;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;
	ID3D11Texture2D* DepthStencil;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
	D3D11_VIEWPORT Viewport;

	char* mCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};

#endif

