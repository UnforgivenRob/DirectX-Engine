#ifndef ENGINE_H
#define ENGINE_H

#include <d3d12.h>
#include <wrl\client.h>

using Microsoft::WRL::ComPtr;

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class Engine
{
public:
	Engine( HINSTANCE hInstance );
	~Engine();

	void run();

	virtual void Initialize() = 0;
	virtual void LoadContent() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ClearBuffers() = 0;
	virtual void UnloadContent() = 0;

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	ComPtr<ID3D12Device> const getDevice();

private:
	void PreLoad();
	void PreInit();
	void SwapBuffers();	

	//Message Handling functions
	void OnResize();
	virtual void OnMouseDown( WPARAM btnState, int x, int y ){ btnState, x, y; }
	virtual void OnMouseUp( WPARAM btnState, int x, int y )  { btnState, x, y; }
	virtual void OnMouseMove( WPARAM btnState, int x, int y ){ btnState, x, y; }

	//keyboard input handling functings
	virtual void OnKeyDown( WPARAM keyState ) { keyState; }
	virtual void OnKeyUp( WPARAM keyState ) { keyState; }

protected:
	bool bRun;

	HINSTANCE mApp;


	char* mCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};

#endif

