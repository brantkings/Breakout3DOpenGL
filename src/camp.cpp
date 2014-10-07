#include "camp.h"


int Brick::ID_GENERAL = 0;

Brick::Brick(Camp* f,TypeBrick t)
{
	id = ID_GENERAL;
	ID_GENERAL++;

	parent = f;

	tipo = t;

	size = 0.1;
	height = 1.0;
	width = 3.0;

	fading = false;
	
	switch(tipo)
	{
		case NORMAL:
			color[0] = 1.0; color[1] = 1.0; color[2] = 0.0; color[3] = 1.0;
			valor = 10;
			break;
		case HARD:
			color[0] = 0.7; color[1] = 0.7; color[2] = 0.0; color[3] = 1.0;
			valor = 20;
			break;
		case STRONG:
			color[0] = 1.0; color[1] = 0.2; color[2] = 0.0; color[3] = 1.0;
			valor = 30;
			break;
		case EXTRA:
			color[0] = 0.0; color[1] = 1.0; color[2] = 0.0; color[3] = 1.0;
			valor = 5;
			break;
		case BONUS:
			color[0] = 0.0; color[1] = 0.0; color[2] = 1.0; color[3] = 1.0;
			valor = 100;
			break;
		case POISON:
			color[0] = 1.0; color[1] = 0.0; color[2] = 1.0; color[3] = 1.0;
			valor = 30;
			break;
		case NORMAL_MARKED:
			color[0] = 1.0; color[1] = 1.0; color[2] = 0.0; color[3] = 1.0;
			valor = 12; //No meio vale mais
			break;
		
	}

	x = 0.0;
	y = -0.8;
	z = 1.0;
	//Tamanho é a base do cubo, altura e largura são multiplicadores
}

Brick::~Brick()
{
	
}

void Brick::render()
{
	//cout << "\nRender ID " << id << ": " << x << " " << y << " " << z;
	glPushMatrix();

	glTranslatef(-x,y,z);
	glColor4dv(color);
	glScalef(width,height,1.0);

	glutSolidCube(size);

	glPopMatrix();
}

void Brick::update(Game* j, unsigned int tempo)
{
	if(fading) fade();
}

void Brick::fade()
{
	fading = true;
	color[3] -= 0.05;
	//if(cor[3] <= 0) l->remove(this);
}

Box* Brick::collisionBox()
{
	Box* c = new Box();

	c->minX = x - size * width / 2.0;
	c->maxX = x + size * width / 2.0;
	c->minY = y - size * height / 2.0;
	c->maxY = y + size * height / 2.0;
	
	return c;
}

//______________________________________________________________________________________________________________________________

Camp::Camp()
{
	back = 0.0;

	listNext = new ListMatrix();

	brickMatrix = createMatrix(RANDOM_CAMP);

	for(int i=0;i<CAMP_LIST;i++)
	{
		listNext->push_back(createMatrix(RANDOM_CAMP));
	}
	
}
Matrix* Camp::createMatrix(int bricks)
{
	Matrix* mat = new Matrix();
	mat->numberBricks = bricks;

	int matrixTypes[WIDTH_CAMP][HEIGHT_CAMP];

	//Clean the matrix
	for(int i = 0;i<WIDTH_CAMP;i++)
	{
		for(int j = 0; j < HEIGHT_CAMP; j++)
		{
			matrixTypes[i][j] = -1;
		}
	}

	//Create seeds
	int numSeeds = 1 + (rand() % 5);
	while(numSeeds>0)
	{
		numSeeds--;
		int i = rand() % WIDTH_CAMP;
		int j = rand() % HEIGHT_CAMP;

		if(matrixTypes[i][j] == -1)
		{
			bricks--;
			matrixTypes[i][j] = NORMAL;
		}
	}

	//¨Fatten" seeds until bricks are complete.
	while(bricks > 0)
	{
		for(int i = 0;i<WIDTH_CAMP && bricks > 0;i++)
		{
			for(int j = 0; j < HEIGHT_CAMP && bricks > 0; j++)
			{
				int novoI, novoJ;
				if(matrixTypes[i][j] == NORMAL)
				{
					matrixTypes[i][j] = NORMAL_MARKED;

					//Fatten upwards
					novoJ = j-1;
					if(novoJ>=0 && matrixTypes[i][novoJ] == -1)
					{
						matrixTypes[i][novoJ] = NORMAL;
						bricks--;
					}
					//Fatten downwards
					novoJ = j+1;
					if(novoJ<HEIGHT_CAMP && matrixTypes[i][novoJ] == -1)
					{
						matrixTypes[i][novoJ] = NORMAL;
						bricks--;
					}
					//Fatten left
					novoI = i-1;
					if(novoI>=0 && matrixTypes[novoI][j] == -1)
					{
						matrixTypes[novoI][j] = NORMAL;
						bricks--;
					}
					//Fatten right
					novoI = i+1;
					if(novoI<WIDTH_CAMP && matrixTypes[novoI][j] == -1)
					{
						matrixTypes[novoI][j] = NORMAL;
						bricks--;
					}
				
					//Upgrade Brick
					int chance = lrand48() % 101; //O a 100
					if(chance < CHANCE_HARD)
					{
						matrixTypes[i][j] = HARD;
					}
					else if(chance < CHANCE_HARD + CHANCE_STRONG)
					{
						matrixTypes[i][j] = STRONG;
					}
					else if(chance < CHANCE_HARD + CHANCE_STRONG + CHANCE_BONUS)
					{
						matrixTypes[i][j] = BONUS;
					}
					else if(chance < CHANCE_HARD + CHANCE_STRONG + CHANCE_EXTRA + CHANCE_BONUS)
					{
						matrixTypes[i][j] = EXTRA;
					}
					else if(chance < CHANCE_HARD + CHANCE_STRONG + CHANCE_EXTRA + CHANCE_BONUS + CHANCE_POISON)
					{
						matrixTypes[i][j] = POISON;
					}
				}
			}
		}
	}
	if(bricks < 0) mat->numberBricks -= bricks;

	//Cria tijolos em si
	for(int i = 0;i < HEIGHT_CAMP;i++)
	{
		for(int j = 0;j<WIDTH_CAMP;j++)
		{
			if(matrixTypes[j][i]!=-1)
			{
				Brick* t = new Brick(this, (TypeBrick) matrixTypes[j][i]);
				t->x = j * t->size * t->width - 1.0;
				t->y = i * t->size * t->height + 0.20;
				mat->m[j][i] = t;
	
			}
			else mat->m[j][i] = NULL;
		}
	}
	return mat;
}
void Camp::updateMatrix(Game* game, unsigned int time, Matrix* mat)
{
	for(int i = 0;i < HEIGHT_CAMP;i++) for(int j = 0;j<WIDTH_CAMP;j++)
	{
		Brick* t = mat->m[j][i];
		if(t!=NULL)
		{
			t->update(game, time);
			if(t->color[3] < 0)
			{
				mat->numberBricks--;
				delete t;
				mat->m[j][i] = NULL;
			}
		}
	}
}
void Camp::renderMatrix(Matrix* mat, bool foggy)
{
	for(int i = 0;i < HEIGHT_CAMP;i++)
	{
		for(int j = 0;j<WIDTH_CAMP;j++)
		{
			Brick* t = mat->m[j][i];
			if(t!=NULL)
			{
				if(foggy) t->color[3] -= 0.6; //Change alpha if foggy.
				if(!t->fading) t->render();
				if(foggy) t->color[3] += 0.6;
			}
		}
	}
	if(foggy) //If it is foggy it is on the background, if it is on the background, can't be fading.
	{
		for(int i = 0;i < HEIGHT_CAMP;i++)
		{
			for(int j = 0;j<WIDTH_CAMP;j++)
			{
				Brick* t = mat->m[j][i];
				if(t!=NULL)
				{
					if(t->fading) t->render();
				}
			}
		}
	}
	
}
void Camp::update(Game* game, unsigned int time)
{
	updateMatrix(game,time,brickMatrix);

	if(brickMatrix->numberBricks <= 0 && game->ball->y <= 0.0)
	{
		comeFromBackground(game);
		game->ball->increaseVelocity();
	} 
}
void Camp::render()
{
	//Começa uma matriz nova
	//int i = 0;
	glPushMatrix();
	ListMatrix::iterator it;
	glTranslatef(0.0,0.0,-back); //Pra fazer tudo voltar
	//size_t tamLista = listaProx->size();
	for(it = (listNext->begin()); it != (listNext->end()); it++)
	{
		if((*it) != NULL)
		{ 
			//cout << "\n" << i << "\n";
			//glPushMatrix();
			//glTranslatef(0.0,0.0,(tamLista - i)*CAMP_DISTANCE);
			glTranslatef(0.0,0.0,CAMP_DISTANCE);
			renderMatrix(*it,true);
			//glPopMatrix();
		}
	}
	//Termina matriz
	glPopMatrix();

	
	renderMatrix(brickMatrix,false);
	
	
}

void Camp::comeFromBackground(Game* game)
{
	if(back==0 && brickMatrix!=NULL)
	{
		//He just exploded a wall, so congratulate the player!
		game->congratulate();
		
		for(int i = 0;i < HEIGHT_CAMP;i++)
		{
			for(int j = 0;j<WIDTH_CAMP;j++)
			{
				Brick* t = brickMatrix->m[j][i];
				if(t!=NULL)
				{
					t->fade();
				}
			}
		}

	}
	back += 0.01;
	if(back >= CAMP_DISTANCE)
	{
		delete brickMatrix;
		brickMatrix = NULL;
		brickMatrix = *(listNext->begin());
		listNext->pop_front();
		back = 0.0;


		listNext->push_back(createMatrix(RANDOM_CAMP));
	}
}

void Camp::init()
{
	delete brickMatrix;
	brickMatrix = NULL;
	brickMatrix = *(listNext->begin());
	listNext->pop_front();
	back = 0.0;

	listNext->push_back(createMatrix(RANDOM_CAMP));
}

void Camp::print()
{
	cout << "\n	Numero de tijolos na matriz atual " << brickMatrix->numberBricks << ".";
	if(going > 0)cout << "\n	A fase está vindo!";
	cout << "\nExiste uma fila de " << CAMP_LIST << " matrizes de fases à seguir.";
}
Camp::~Camp()
{
	if(listNext != NULL) delete listNext;
	if(brickMatrix != NULL) delete brickMatrix;
}

