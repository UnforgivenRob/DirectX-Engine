#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	prevInstance, cmdLine, showCmd;
	Game* app = new Game(hInstance);
	app->run();

	delete app;

	return 0;
}