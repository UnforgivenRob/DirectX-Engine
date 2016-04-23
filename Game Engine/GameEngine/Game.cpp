#include "Game.h"
#include "Time.h"
#include <stdio.h>

Game::Game( HINSTANCE hInstance )
	: Engine( hInstance )
{
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
		if( cnt > 10 )
		{
			bRun = false;
		}
	}	
}

void Game::Draw()
{
}

void Game::UnloadContent()
{
}
