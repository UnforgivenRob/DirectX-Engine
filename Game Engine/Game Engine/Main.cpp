#include "Game.h"

int main( int argc, char** argv )
{
	argc, argv;
	Game* app = new Game();
	app->run();

	delete app;
	return 0;
}