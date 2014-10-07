#ifndef FASE_H
#define FASE_H

#include "entity.h"
#include "game.h"

#define HEIGHT_CAMP 7
#define WIDTH_CAMP 8
#define CAMP_LIST 20
#define CAMP_DISTANCE 0.1
#define RANDOM_CAMP (lrand48() % 56) + 1

#define CHANCE_HARD 30
#define CHANCE_STRONG 20
#define CHANCE_EXTRA 5
#define CHANCE_BONUS 15
#define CHANCE_POISON 15

using namespace std;


class Camp;

enum TypeBrick
{
	NORMAL,
	HARD,
	STRONG,
	EXTRA,
	BONUS,
	POISON,
	NORMAL_MARKED
};

class Brick : public Entity
{
	Camp* parent;
public:
	static int ID_GENERAL;
	int id;
	bool fading;
	double color[4];

	TypeBrick tipo;

	int valor;

	Brick(Camp*, TypeBrick tipo);
	virtual void update(Game* j,unsigned int time);
	virtual void render();
	virtual Box* collisionBox();
	void fade();
	virtual ~Brick();
};

typedef Brick* BrickMatrix[WIDTH_CAMP][HEIGHT_CAMP];

/**
 * A matrix is a single field of bricks.
 */
struct Matrix
{
	int numberBricks;
	BrickMatrix m;
	Matrix()
	{
		numberBricks = 0;
		for(int i = 0;i < HEIGHT_CAMP;i++) for(int j = 0;j<WIDTH_CAMP;j++) m[j][i] = NULL;
	}
	~Matrix()
	{
		for(int i = 0;i < HEIGHT_CAMP;i++) for(int j = 0;j<WIDTH_CAMP;j++) if(m[j][i] != NULL) {delete m[j][i]; m[j][i] = NULL;}
	}
};

typedef list<Matrix*> ListMatrix;

class Camp
{
	int numberBricks;
	float back;
	bool going;

public:	
	ListMatrix* listNext;
	Matrix* brickMatrix;	

	Camp();

	/**
	 * Procedurally create a new play matrix based only on the number of bricks.
	 */
	Matrix* createMatrix(int numBricks);
	void updateMatrix(Game* j, unsigned int time, Matrix*);
	void renderMatrix(Matrix*, bool foggy);
	void comeFromBackground(Game* j);
	void update(Game* j, unsigned int time);
	void render();
	void init();
	void print();
	~Camp();
};

#endif
