#ifndef JOGO_H
#define JOGO_H

#include "hud.h"

class Racket;
class Ball;
class Camp;
class HUD;

struct Keys
{
	bool a;
};
class Game
{
	bool pause;
	float angle;
	unsigned int gameTime;
public:
	static Game* singleton;
	bool gameOver;
	bool ready;
	int lives;
	int points;

	//HUD
	HUD* hud;

	//Game elements
	Racket* player;
	Ball* ball;
	Camp* activeCamp;

	Keys pressedKeys;
	int mousePosX, mousePosY;

	Game();
	void initializeOpenGL(int&, char**);
	void init();
	void restartMatch(); //Recomeça outra partida
	void doPause();
	void reset(); //Recomeça tudo em posição inicial
	void endGame();


	/**
	 * Write the points given to the player, arcade-style.
	 */
	void writePoints(int pointsToWrite, float x, float y);

	/**
	 * Put a congratulation message on the screen, for the player.
	 */
	void congratulate();

	/* Callbacks */
	void mouse(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void keyboard(unsigned char key, int x, int y);
	void render();
	void update(unsigned int time);

	/**
	 * For debug purposes.
	 */
	void print();

	~Game();
};

#endif
