#ifndef BOLA_H
#define BOLA_H

#define VELOCIDADE_INICIAL 0.015

#include "camp.h"

class Ball : public Entity
{
	int hitRacketCount;
	int verificationCount;
	int ballCombo;
	bool fading;
	bool isGonnaIncreaseVelocity;
	bool angry;
	bool poisonous;
	float color[4];
public:
	Ball();
	virtual void update(Game* g,unsigned int time);
	virtual void render();
	virtual Box* collisionBox();
	virtual void init();
	void increaseVelocity();
	void getAngry();
	void getCalm();
	void getPoisonous();
	void print();
	void reflect(double normal[]);
	void fade();
	virtual ~Ball();
};

#endif
