#include "racket.h"

Racket::Racket()
{
	init();
}

Racket::~Racket()
{
	
}

void Racket::render()
{
	//Desenhar paddle em si
	glPushMatrix();

	glTranslatef(-x,y,z);
	glScalef(width,height,1.0);
	
	if(poisoned) glColor4f(1.0,0.0,1.0,1.0);
	else glColor4f(1.0,1.0,1.0,1.0);
	glutSolidCube(size);

	glPopMatrix();
}

void Racket::update(Game* j, unsigned int tempo)
{
	//Muda a velocidade
	if(j->mousePosX < SCREEN_WIDTH*MOUSE_CONTROL_MIN)
	{
		double limiteIntervaloMouse = SCREEN_WIDTH*MOUSE_CONTROL_MIN - 0.05;
		velocityX = -MAXIMUM_VELOCITY*(limiteIntervaloMouse - j->mousePosX)/limiteIntervaloMouse;
	}
	else if(j->mousePosX >= SCREEN_WIDTH*MOUSE_CONTROL_MIN && j->mousePosX < SCREEN_WIDTH*MOUSE_CONTROL_MAX)
	{
		//Faz nada
	}
	else
	{
		double limiteIntervaloMouse = SCREEN_WIDTH*MOUSE_CONTROL_MIN + 0.05;
		velocityX = MAXIMUM_VELOCITY*(j->mousePosX - SCREEN_WIDTH*MOUSE_CONTROL_MAX)/limiteIntervaloMouse;
		//velocityX += 0.005;// * tempo * 1000 / FPS;
	}

	//Resistencia do "ar" e restricoes
	velocityX *= AIR_RESISTANCE;
	if(!poisoned)
	{
		if(velocityX < -MAXIMUM_VELOCITY) velocityX = -MAXIMUM_VELOCITY;
		else if (velocityX > MAXIMUM_VELOCITY) velocityX = MAXIMUM_VELOCITY;
		else if (velocityX * velocityX < 0.00001) velocityX = 0;
	}
	else
	{
		if(velocityX < -MAXIMUM_VELOCITY/4) velocityX = -MAXIMUM_VELOCITY/4;
		else if (velocityX > MAXIMUM_VELOCITY/4) velocityX = MAXIMUM_VELOCITY/4;
		else if (velocityX * velocityX < 0.00001) velocityX = 0;
	}

	//Soma a velocidade
	x += velocityX;
	if(x < -SCREEN_BORDER) x = -SCREEN_BORDER;
	else if (x > SCREEN_BORDER) x = SCREEN_BORDER;

	//cout << "\n" << this->x << " " << this->y << " VELO:" << this->velocityX;

	//cout << "\n" << j->mousePosX << " " << j->mousePosY;
}

void Racket::poison()
{
	poisoned = true;
}
void Racket::cure()
{
	poisoned = false;
}

void Racket::print()
{
	cout << "\n	Velocidade no eixo X " << velocityX;
	Entity::print();
}

void Racket::init()
{
	size = 0.1;
	height = 1.0;
	width = 3.0;

	velocityX = 0.0;
	
	poisoned = false;

	x = 0.0;
	y = -0.8;
	z = 1.0;
	//Tamanho é a base do cubo, altura e largura são multiplicadores
}

Box* Racket::collisionBox()
{
	Box* c = new Box();

	c->minX = x - size * width / 2.0;
	c->maxX = x + size * width / 2.0;
	c->minY = y - size * height / 2.0;
	c->maxY = y + size * height / 2.0;
	
	return c;
}
