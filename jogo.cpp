#include "jogo.h"

Jogo* Jogo::principal = NULL;

/*void Render()
{
	Jogo::principal->render();
}
void Update()
{
	Jogo::principal->update();
}*/
void Mouse(int botao, int estado, int x, int y)
{
	Jogo::principal->mouse(botao,estado,x,y);
}
void MouseMove(int x, int y)
{
	Jogo::principal->mouseMove(x,y);
}
void Teclado(unsigned char tecla, int x, int y)
{
	Jogo::principal->teclado(tecla,x,y);
}
void Timer(int tempo)
{
	//Update();
	//Render();

	//static time_t tempoPassou = time(NULL);
	

	glutTimerFunc(tempo, Timer, tempo);
	
	//Se o OpenGL for paralelizado, isso resolve o problema de não ter dado todo o update antes
	if(Jogo::principal->pronto) Jogo::principal->render();

	//Calcula tempo e faz update
	//double dif = difftime(time(NULL),tempoPassou); 
	//printf("\n%f",(int)dif);
	Jogo::principal->update(tempo);
	//tempoPassou = time(NULL);
	
}


//___________________________________________________________________________________________________________________
Jogo::Jogo()
{
	angle = 0.0;
	tempo = 1000/FPS;
	pronto = false;
	mousePosX = 0; mousePosY = 0;
	pause = true;
	gameOver = false;

	vidas = 3;
	pontos = 0;

	jogador = new Raquete();
	bola = new Bola();
	faseAtual = new Fase();
	hud = new HUD(this);

	if(Jogo::principal == NULL) principal = this;
}
Jogo::~Jogo()
{
	delete jogador;
	delete bola;
	if(Jogo::principal == this) principal = NULL;
}

void Jogo::inicializarOpenGL(int& args, char** argv)
{
	//Iniciar variaveis
	glutInit(&args, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(TELA_WIDTH,TELA_HEIGHT);
	//glutInitWindowPosition();

	//Criar uma janela
	char* nomeDoJogo = (char*) malloc(sizeof(char) * 20);
	strcpy(nomeDoJogo,JOGO_NOME);
	glutCreateWindow(nomeDoJogo);

	//Ativar blend com alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Ativar textura (texto)
	//glEnable(GL_TEXTURE_2D); // Ativa-se só na hora de escrever o texto, senão tudo sem textura desaparece

	//Ativar luz e cor de material
	glEnable(GL_LIGHTING);	
	glEnable(GL_COLOR_MATERIAL);

	float vetorAmbiente[4] = {0.3,0.3,0.35,1};
	float vetorDifuso[4] = {0.5,0.5,0.7,1};
	float vetorEspecular[4] = {1.0,1.0,1.0,1.0};
	float vetorPosicao[4] = {0.0,0.0,-1.0,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,vetorAmbiente);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,vetorDifuso);
	glLightfv(GL_LIGHT0,GL_SPECULAR,vetorEspecular);
	glLightfv(GL_LIGHT0,GL_POSITION,vetorPosicao);
	glEnable(GL_LIGHT0);

	//Ativar teste de profundidade
	glEnable(GL_DEPTH_TEST);

	//Setar campo de visão e projeção
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(0,(float)TELA_WIDTH/TELA_HEIGHT,0.0,0.5);
	glFrustum(-1.0,1.0,-1.0,1.0,0.91,30.0); //zNear nao pode ser 0, senão não funciona, e não pode ser perto de 0, senão fica de lado
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Setar funcoes importantes
	//glutDisplayFunc(Render);
	//glutIdleFunc(Update);
	glutTimerFunc(tempo, Timer, tempo);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMove);
	glutKeyboardFunc(Teclado);
}

void Jogo::iniciar()
{
	glutMainLoop();
}

void Jogo::recomecar()
{
	reset();
	gameOver = false;
	vidas = 3;
	pontos = 0;
}

void Jogo::pausar()
{
	pause = true;
}

void Jogo::fimJogo()
{
	gameOver = true;
}

void Jogo::reset()
{

	jogador->init();
	bola->init();
	
	faseAtual->init();

	pause = true;
}

/* Render the game */
void Jogo::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,0.91,30.0); //zNear nao pode ser 0, senão não funciona, e não pode ser perto de 0, senão fica de lado
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Camera
	gluLookAt(0.0,0.0,0.0,
		0.0,0.0,1.0,
		0.0,1.0,0.0);
	
	/*  Objetos do Jogo em 3D */
	if(!gameOver)
	{
		glPushMatrix();
		glRotatef(angle,1.0,0.0,0.0);
		glRotatef(90.0,0.0,1.0,0.0);
		glColor4f(1.0,1.0,1.0,0.05);
		glutWireTorus(2.0,2.0,18,18);
		glPopMatrix();
		

		//Objetos
		jogador->render();
		bola->render();
		faseAtual->render();

	}
	else
	{

		double pos[3] = {0.9,0.5,1.0};
		double tam[3] = {3.0,3.0,1.0};
		float cor[4] = {1.0,0.1,0.0,1.0};
		escreverTexto("Game Over",pos, tam, cor);

		pos[0] = 0.8; pos[1] = 0.0; pos[2] = 1.0;
		tam[0] = 0.7; tam[1] = 0.7; tam[2] = 1.0;
		escreverTexto("Aperte R para reiniciar ou Q para sair.",pos, tam, cor);
	}

	/* HUD em projeção ortogonal */

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0,1.0,-1.0,1.0,0.95,10.0);
	glMatrixMode(GL_MODELVIEW);

	hud->render();

	//Desenha
	glutSwapBuffers();
	glFlush();

}


/* Update the game */
void Jogo::update(unsigned int tempo)
{
	if(!pause) angle+=0.1;
	if(!pause && !gameOver)
	{
		
		jogador->update(this,tempo);
		bola->update(this,tempo);
		faseAtual->update(this,tempo);
		hud->update(this,tempo);
	}

	if(vidas < 0) fimJogo();

	//Deixa pronto pra fazer o render()
	pronto = true;
}



void Jogo::escreverPontos(int pontos, float x, float y)
{
	int i;
	char pts[20];
	for(i=0;i<20;i++) pts[i] = 0;
	sprintf(pts,"%d",pontos);
	
	hud->adicionarMarcadorPontos(pts,x,y);
}
void Jogo::parabens()
{
	//Aproveita a função de escrever pontos tela para parabenizar o jogador
	int chance = lrand48() % 100;
	char texto[20];
	if(chance < 10) strcpy(texto,"Bacana!");
	else if(chance < 20) strcpy(texto,"Legal!");
	else if(chance < 30) strcpy(texto,"Continue assim!");
	else if(chance < 40) strcpy(texto,"Nao pare agora!");
	else if(chance < 50) strcpy(texto,"Woow!");
	else if(chance < 60) strcpy(texto,"Doido!");
	else if(chance < 70) strcpy(texto,"Incrivel!");
	else if(chance < 80) strcpy(texto,"Nussinhora!");
	else if(chance < 90) strcpy(texto,"Que que isso!");
	else		strcpy(texto,"Doidimais!");
	hud->adicionarMarcadorPontos(texto,(float)(((float)(chance) / 100.0) - 1.0),0.0);
}

void Jogo::mouse(int botao, int estado, int x, int y)
{
	if(estado == GLUT_DOWN)
	{
		switch(botao)
		{
			case GLUT_LEFT_BUTTON:
				pause = !pause;
				break;
			case GLUT_MIDDLE_BUTTON:
				break;
			case GLUT_RIGHT_BUTTON:
				if(pause)
				{
					pause = false;
					update(tempo);
					pause = true;
					render();
				}
				pause = true;
				imprimir();
				break;
		}
	}
}
void Jogo::mouseMove(int x, int y)
{
	mousePosX = x; mousePosY = y;
	//cout << "\n" << mousePosX << " " << mousePosY;
}
void Jogo::teclado(unsigned char tecla, int x, int y)
{
	switch(tecla)
	{
		case 'a':
			teclasApertadas.a = true;
			break;
		case 'A':
			teclasApertadas.a = true;
			break;
		case 'r':
			recomecar();
			break;
		case 'q':
			exit(0);
			break;
	}
}
void Jogo::imprimir()
{
	cout << "\nNESSE FRAME:";
	cout << "\nVidas -> " << vidas;
	cout << "\nPontos -> " << pontos;
	cout << "\nPosicao do Mouse -> (" << mousePosX << " , " << mousePosY << ")";
	cout << "\n-----------------";
	cout << "\nImprimindo estado da Raquete";
	jogador->imprimir();
	cout << "\nImprimindo estado da Bola";
	bola->imprimir();
	cout << "\nImprimindo estado da Fase";
	faseAtual->imprimir();
	cout << "\nImprimindo estado da HUD";
	hud->imprimir();
	cout << "\n_______________________________________________________________\n\n";
}
