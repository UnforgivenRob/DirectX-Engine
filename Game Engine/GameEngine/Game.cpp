#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>

Game::Game( HINSTANCE hInstance )
	: Engine( hInstance )
{
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;
	FullTimer.tic();
	intervalTimer.tic();
}

Game::~Game()
{
}

void Game::Initialize()
{
}

void Game::LoadContent()
{
}

void Game::Update()
{
	intervalTimer.toc();
	intervalTimer.tic();

	static int cnt = 0;
	static Time prev = Time( TIME_ZERO );
	static Time dif  = Time( TIME_ZERO );
	Time current = FullTimer.toc();
	dif += current - prev;
	prev = current;

	if( dif >=  Time( TIME_ONE_SECOND ) )
	{
		dif = Time ( TIME_ZERO );
		printf( "Tic%d\n", cnt );
		cnt++;
	}

	color[0] = 1.0f / (float)cnt;
	color[1] = 1.0f / (float)cnt;
	color[2] = 1.0f / (float)cnt;
}

void Game::Draw()
{
	assert( Context );
	assert( SwapChain );

	Context->ClearRenderTargetView(RenderTargetView, color );
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	HRESULT res = SwapChain->Present(0, 0);
	assert( res == S_OK );
}

void Game::UnloadContent()
{
}
