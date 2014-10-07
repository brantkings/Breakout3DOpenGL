#include "game.h"

Game* Game::singleton = NULL;

/*void Render()
{
	Jogo::principal->render();
}
void Update()
{
	Jogo::principal->update();
}*/
void Mouse(int button, int state, int x, int y)
{
	Game::singleton->mouse(button,state,x,y);
}
void MouseMove(int x, int y)
{
	Game::singleton->mouseMove(x,y);
}
void Teclado(unsigned char tecla, int x, int y)
{
	Game::singleton->keyboard(tecla,x,y);
}
void Timer(int tempo)
{
	glutTimerFunc(tempo, Timer, tempo);
	
	//Only render if update is done.
	if(Game::singleton->ready) Game::singleton->render();

	//Update.
	Game::singleton->update(tempo);
	
}


//___________________________________________________________________________________________________________________
Game::Game()
{
	angle = 0.0;
	gameTime = 1000/FPS;
	ready = false;
	mousePosX = 0; mousePosY = 0;
	pause = true;
	gameOver = false;

	lives = 3;
	points = 0;

	player = new Racket();
	ball = new Ball();
	activeCamp = new Camp();
	hud = new HUD(this);

	if(Game::singleton == NULL) singleton = this;
}
Game::~Game()
{
	delete player;
	delete ball;
	if(Game::singleton == this) singleton = NULL;
}

void Game::initializeOpenGL(int& args, char** argv)
{
	//Init fields
	glutInit(&args, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//glutInitWindowPosition();

	//Create a window
	char* nomeDoJogo = (char*) malloc(sizeof(char) * 20);
	strcpy(nomeDoJogo,GAME_NAME);
	glutCreateWindow(nomeDoJogo);

	//Activate blend with alpha.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Activate texture, for text
	//glEnable(GL_TEXTURE_2D); // Enable just when it's time to write, or else everything with textures disappear.

	//Enable lights and materials.
	glEnable(GL_LIGHTING);	
	glEnable(GL_COLOR_MATERIAL);

	//Lightning.
	float ambient[4] = {0.3,0.3,0.35,1};
	float diffuse[4] = {0.5,0.5,0.7,1};
	float especular[4] = {1.0,1.0,1.0,1.0};
	float position[4] = {0.0,0.0,-1.0,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,especular);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glEnable(GL_LIGHT0);

	//
	glEnable(GL_DEPTH_TEST);

	//Set vision default field and projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,0.91,30.0); //zNear cant be 0 or else it doesnt work, cant be near 0 or else it gets sideways.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//OPENGL functions.
	//glutDisplayFunc(Render);
	//glutIdleFunc(Update);
	glutTimerFunc(gameTime, Timer, gameTime);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMove);
	glutKeyboardFunc(Teclado);
}

void Game::init()
{
	glutMainLoop();
}

void Game::restartMatch()
{
	reset();
	gameOver = false;
	lives = 3;
	points = 0;
}

void Game::doPause()
{
	pause = true;
}

void Game::endGame()
{
	gameOver = true;
}

void Game::reset()
{

	player->init();
	ball->init();
	
	activeCamp->init();

	pause = true;
}

/* Render the game */
void Game::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,0.91,30.0); //zNear cant be near 0, or else it won't work. Can't be near 0 either, or else everything gets sideways.
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Camera
	gluLookAt(0.0,0.0,0.0,
		0.0,0.0,1.0,
		0.0,1.0,0.0);
	
	/*  3D Game objects */
	if(!gameOver)
	{
		glPushMatrix();
		glRotatef(angle,1.0,0.0,0.0);
		glRotatef(90.0,0.0,1.0,0.0);
		glColor4f(1.0,1.0,1.0,0.05);
		glutWireTorus(2.0,2.0,18,18);
		glPopMatrix();
		

		//Objetos
		player->render();
		ball->render();
		activeCamp->render();

	}
	else
	{
		//Display game over screen.
		double pos[3] = {0.9,0.5,1.0};
		double tam[3] = {3.0,3.0,1.0};
		float cor[4] = {1.0,0.1,0.0,1.0};
		writeText("Game Over",pos, tam, cor);

		pos[0] = 0.8; pos[1] = 0.0; pos[2] = 1.0;
		tam[0] = 0.7; tam[1] = 0.7; tam[2] = 1.0;
		writeText("Press R to reset or Q to quit.",pos, tam, cor);
	}

	/* HUD in ortogonal projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0,1.0,-1.0,1.0,0.95,10.0);
	glMatrixMode(GL_MODELVIEW);

	hud->render();

	//Draw
	glutSwapBuffers();
	glFlush();

}


/* Update the game */
void Game::update(unsigned int time)
{
	if(!pause) angle+=0.1;
	if(!pause && !gameOver)
	{
		
		player->update(this,time);
		ball->update(this,time);
		activeCamp->update(this,time);
		hud->update(this,time);
	}

	if(lives < 0) endGame();

	//Ready to render()
	ready = true;
}



void Game::writePoints(int pointsToWrite, float x, float y)
{
	int i;
	char pts[20];
	for(i=0;i<20;i++) pts[i] = 0;
	sprintf(pts,"%d",pointsToWrite);
	
	hud->addPointsMarker(pts,x,y);
}
void Game::congratulate()
{
	int chance = lrand48() % 100;
	char text[20];
	if(chance < 10) strcpy(text,"Sensational!");
	else if(chance < 20) strcpy(text,"Cool!");
	else if(chance < 30) strcpy(text,"Keep it on!");
	else if(chance < 40) strcpy(text,"Dont stop now!");
	else if(chance < 50) strcpy(text,"Woow!");
	else if(chance < 60) strcpy(text,"Crazy!");
	else if(chance < 70) strcpy(text,"Incredible!");
	else if(chance < 80) strcpy(text,"Gawd!");
	else if(chance < 90) strcpy(text,"Dammit girl!");
	else		strcpy(text,"You are my hero!");
	hud->addPointsMarker(text,(float)(((float)(chance) / 100.0) - 1.0),0.0);
}

void Game::mouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		switch(button)
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
					update(gameTime);
					pause = true;
					render();
				}
				pause = true;
				print();
				break;
		}
	}
}
void Game::mouseMove(int x, int y)
{
	mousePosX = x; mousePosY = y;
}
void Game::keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'a':
			pressedKeys.a = true;
			break;
		case 'A':
			pressedKeys.a = true;
			break;
		case 'r':
			restartMatch();
			break;
		case 'q':
			exit(0);
			break;
	}
}

void Game::print()
{
	cout << "\nIN THIS FRAME:";
	cout << "\nLIVES -> " << lives;
	cout << "\nPOINTS -> " << points;
	cout << "\nMouse Position -> (" << mousePosX << " , " << mousePosY << ")";
	cout << "\n-----------------";
	cout << "\n Racket Status";
	player->print();
	cout << "\n Ball Status";
	ball->print();
	cout << "\n Camp Status";
	activeCamp->print();
	cout << "\n HUD Status";
	hud->print();
	cout << "\n_______________________________________________________________\n\n";
}
