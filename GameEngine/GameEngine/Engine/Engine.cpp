#include "Engine.h"
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include <WindowsX.h>
#include "GraphicsEngine.h"

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
	switch( msg )
	{
	case WM_ACTIVATE:
		return 0;
	case WM_SIZE:
		return 0;
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		return 0;
	case WM_DESTROY:
		bRun = false;
		return 0;
	case WM_MENUCHAR:
		return 0;
	case WM_GETMINMAXINFO:
		return 0;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		return 0;
	case WM_KEYDOWN:
		OnKeyDown( wParam );
		return 0;
	case WM_KEYUP:
		OnKeyUp( wParam );
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

Engine::Engine( HINSTANCE hInstance )
	: bRun( true ), mEnable4xMsaa( false ), mApp( hInstance ), mClientWidth( 800 ), mClientHeight( 600 ), md3dDriverType( D3D_DRIVER_TYPE_HARDWARE ), mCaption( "Game Engine" )
{
	app = this;
}

Engine::~Engine()
{
	GraphicsEngine::Deactivate();
}

void Engine::run()
{
	PreInit();

	Initialize();

	PreLoad();

	LoadContent();

	MSG msg = {};
	while( bRun )
	{
		SwapBuffers();
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}

		Update();

		ClearBuffers();

		Draw();		
	}

	UnloadContent();
}

void Engine::PreInit()
{
	WNDCLASS wc = { 0 };
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.hInstance     = mApp;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
	}

	GraphicsEngine::Activate( mApp, mCaption, mEnable4xMsaa, mClientWidth, mClientHeight );
}

void Engine::PreLoad()
{
}

void Engine::OnResize()
{
	GraphicsEngine::OnResize();
}

void Engine::SwapBuffers()
{
}

ComPtr<ID3D12Device> const Engine::getDevice() 
{ 
	return GraphicsEngine::getDevice(); 
}

