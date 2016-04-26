#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>

Game::Game( HINSTANCE hInstance )
	: Engine( hInstance )
{
	color = Vect( 1.0f, 1.0f, 1.0f, 1.0f );
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

	color[x] = 1.0f / (float)cnt;
	color[y] = 1.0f / (float)cnt;
	color[z] = 1.0f / (float)cnt;
}

void Game::Draw()
{
	assert( Context );
	assert( SwapChain );

	Context->ClearRenderTargetView(RenderTargetView, &color[x] );
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	HRESULT res = SwapChain->Present(0, 0);
	assert( res == S_OK );
}

void Game::UnloadContent()
{
}
