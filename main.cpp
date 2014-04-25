#include "jogo.h"

int main(int argc, char** argv)
{
	srand48(time(NULL));

	int& a = argc;
	Jogo* principal = new Jogo();
	principal->inicializarOpenGL(a, argv);
	
	principal->iniciar();

	delete principal;

	return 0;
}
