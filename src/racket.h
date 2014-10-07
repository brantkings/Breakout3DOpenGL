#ifndef RAQUETE_H
#define RAQUETE_H

#include "ball.h"
#include "game.h"


class Racket : public Entity
{
	//double tamanho;
	double velocityX;
	bool poisoned;
public:
	Racket();
	virtual void update(Game* g,unsigned int time);
	virtual void render();
	virtual void init();
	void poison();
	void cure();
	void print();
	virtual Box* collisionBox();
	virtual ~Racket();
};

#endif
