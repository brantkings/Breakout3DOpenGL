#ifndef HUD_H
#define HUD_H

#include "racket.h"
#include "game.h"

struct PointsWriter
{
	float x,y,alpha,size;
	char text[20];
	PointsWriter(char* oq,float x,float y,float siz);
	void update();
	void render();
};
struct LivesShow
{
	double x,y,z;
	LivesShow(double px, double py, double pz);
	void render(int lives, double size);
};

typedef list<PointsWriter*> ListWriters;

class HUD
{
	Game* dad;
	LivesShow* shower;
	ListWriters* writerList;
public:
	HUD(Game* j);
	void render();
	void update(Game* j, unsigned int time);
	void print();
	void addPointsMarker(char* text, float x, float y);
	~HUD();
};

#endif
