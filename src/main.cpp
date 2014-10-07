#include "game.h"

int main(int argc, char** argv)
{
	srand48(time(NULL));

	int& a = argc;
	Game* mainGame = new Game();
	mainGame->initializeOpenGL(a, argv);
	
	mainGame->init();

	delete mainGame;

	return 0;
}
