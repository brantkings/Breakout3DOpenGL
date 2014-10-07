#include "hud.h"

PointsWriter::PointsWriter(char* oq,float px,float py,float tam)
{
	strncpy(text,oq,20);
	x = px;
	y = py;
	size = tam;

	alpha = 1.0;
}
void PointsWriter::update()
{
	y += 0.001;
	alpha -= 0.03;
}
void PointsWriter::render()
{
	double pos[3] = {-x,y,1.0};
	double tam[3] = {size,size,size};
	float cor[4] = {1.0,1.0,0.4,alpha};
	writeText(text ,pos, tam, cor);
}

//________________________________________________________________________________________

LivesShow::LivesShow(double px, double py, double pz)
{
	x = px; y = py; z = pz;
}
void LivesShow::render(int vidas, double size)
{
	int i;

	//cout << "\nM " << tamanho << " " << vidas << " - " << x << " " << y << " " << z;

	for(i=0;i<vidas;i++)
	{
		glPushMatrix();
		glTranslatef(-x - size * i * 2,y,z + 1.0);
		glColor4f(1.0,1.0,1.0,1.0);
		glutSolidSphere(size,9,9);
		glPopMatrix();
	}
}


//________________________________________________________________________________________

HUD::HUD(Game* j)
{
	dad = j;
	shower = new LivesShow(0.75,-0.75,0.0);
	writerList = new ListWriters();
}
void HUD::render()
{
	glPushMatrix();

	//Left balls
	double ballSize;
	if(dad==NULL) return;
	if(dad->ball != NULL) ballSize = dad->ball->size;
	else ballSize = BALL_SIZE;
	shower->render(dad->lives,ballSize);


	//Draw divisible lines to orientate the player
	if(!dad->gameOver)
	{
		glBegin(GL_LINES);
			glColor4f(1.0,1.0,1.0,0.2);
			glVertex3f(MOUSE_CONTROL_MIN*2 - 1.0,-1.0,1.0);
			glColor4f(1.0,1.0,1.0,0.0);
			glVertex3f(MOUSE_CONTROL_MIN*2 - 1.0,1.0,1.0);
			glColor4f(1.0,1.0,1.0,0.2);
			glVertex3f(MOUSE_CONTROL_MAX*2 - 1.0,-1.0,1.0);
			glColor4f(1.0,1.0,1.0,0.0);
			glVertex3f(MOUSE_CONTROL_MAX*2 - 1.0,1.0,1.0);
		glEnd();
	}

	//Draw line writers
	if(!writerList->empty())
	{
		ListWriters::iterator it;
		for(it = writerList->begin(); it != writerList->end(); it++)
		{
			if ((*it) != NULL) (*it)->render();
		}
	}

	//Draw score
	char scoreText[30];
	for(int i=0;i<30;i++) scoreText[i] = 0;
	sprintf(scoreText,"%d",dad->points);

	double pos[3] = {0.8,-0.8,1.0};
	double tam[3] = {0.8,0.8,1.0};
	float cor[4] = {0.8,0.8,1.0,0.9};
	writeText(scoreText ,pos, tam, cor);

	glPopMatrix();
}
void HUD::update(Game* j, unsigned int time)
{
	if(!writerList->empty())
	{
		ListWriters::iterator it;

		//Mark to remove
		bool marked = false;
		ListWriters::iterator markedWriter; //Only one dies at a time, more efficient.

		for(it = writerList->begin(); it != writerList->end(); it++)
		{
			if ((*it) != NULL) 
			{
				(*it)->update();
				if((*it)->alpha < 0) 
				{
					markedWriter = it;
					marked = true;
				}
			}
		}

		//Remove what was marked.
		if (marked)
		{
			writerList->remove(*markedWriter);
			delete *markedWriter;
		}
	}
}
void HUD::addPointsMarker(char* text, float x, float y)
{
	PointsWriter* e = new PointsWriter(text, x, y, 1.0);
	writerList->push_back(e);
}
void HUD::print()
{
	cout << "\n	Mostrador de vidas, placar e " << writerList->size() << " escritores de pontos.";
}
HUD::~HUD()
{
	
}


