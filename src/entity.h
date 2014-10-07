#ifndef ENTIDADE_H
#define ENTIDADE_H

#include "util.h"

using namespace std;

class Game;

struct Box
{
	double minX,maxX,minY,maxY;
	double normal[2];
	Box()
	{
		minX = 0.0; maxX = 0.0; minY = 0.0; maxY = 0.0;
		normal[0] = 0.0; normal[1] = 0.0;
	}
	void print()
	{
		cout << "\nBox: X= " << minX << " ate " << maxX << " | Y= " << minY << " ate " << maxY;
	}
};
class Entity
{
protected:
	double velocityX, velocityY;

public:
	Entity();
	virtual ~Entity();

	double x,y,z;
	double width,height,size;
	
	virtual void update(Game* j,unsigned int time) = 0;
	virtual void render() = 0;
	virtual void print();
	virtual Box* collisionBox() = 0;
	bool collide(Entity* e);
};

#endif
