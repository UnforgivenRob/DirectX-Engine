#include "Engine.h"


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
}

void Engine::ClearBuffers()
{
}
void Engine::SwapBuffers()
{
}
