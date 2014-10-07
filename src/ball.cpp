#include "ball.h"
Ball::Ball()
{
	init();
}

Ball::~Ball()
{
}

void Ball::update(Game* j,unsigned int tempo)
{
	verificationCount++;

	if(fading)
	{
		fade();
		if(color[3] < 0.0)
		{
			init();
			j->lives--;
			j->doPause();
		}
	}

	//Verifica a raquete
	if(verificationCount%3 == 0)
	{
		verificationCount = 0;

		if(this->hitRacketCount<=0)
		{
			if(collide(j->player))
			{
				double n[] = {0.0,1.0};
				reflect(n);
				
				ballCombo = 0;
	
				velocityX += (x - j->player->x)/20.0;

				this->hitRacketCount = 20; //Evitar varias reflexões

				if(isGonnaIncreaseVelocity) 
				{
					velocityX *= 1.05; velocityY *= 1.05;
				}
	
				j->player->cure();

				if(angry)
				{
					getCalm();
				}

				if(poisonous)
				{
					j->player->poison();
					getCalm();
				}
			}
		}
		else this->hitRacketCount--;
	}

	//Verifica a fase
	if(verificationCount%3 == 1)
	{
		for(int i = 0;i < HEIGHT_CAMP;i++) for(int k = 0;k < WIDTH_CAMP;k++)
		{
			Brick* t = j->activeCamp->brickMatrix->m[k][i];
			if(t!=NULL)
			{
				if(!t->fading)
				{
					if(collide(t))
					{
						double nX,nY;
						if(x < t->x - t->size * t->width /2) {nX = -1.0; nY = 0.0;}
						if(x > t->x + t->size * t->width /2) {nX = 1.0; nY = 0.0;}
						else {nX = 0.0; nY = -1.0;}
						double n[] = {nX,nY};
						reflect(n);

						int toGetPoints = t->valor + t->valor * (ballCombo + 1) / 2;
			

						TypeBrick tipt = t->tipo;

						ballCombo++;

						double pointX = (x + t->x*2.0)/3.0;
						double pointY = (y + t->y*2.0)/3.0;

						switch(tipt)
						{
							case NORMAL:
								j->writePoints(toGetPoints,pointX,pointY);
								j->points += toGetPoints;
								t->fade();
								break;
							case HARD:
								t->color[0] = 1.0; t->color[1] = 1.0; t->color[2] = 0.0; t->color[3] = 0.85;
								t->tipo = NORMAL;
								break;
							case STRONG:
								j->writePoints(toGetPoints,pointX,pointY);
								j->points += toGetPoints;
								getAngry();
								t->fade();
								break;
							case EXTRA:
								j->writePoints(toGetPoints,pointX,pointY);
								j->points += toGetPoints;
								if(j->lives < 5) j->lives++;
								t->fade();
								break;
							case BONUS:
								j->writePoints(toGetPoints,pointX,pointY);
								j->points += toGetPoints;
								t->fade();
								break;
							case POISON:
								getPoisonous();
								j->writePoints(toGetPoints,pointX,pointY);
								j->points += toGetPoints;
								t->fade();
								break;
							case NORMAL_MARKED:
								t->fade();
								break;
						}

						break;
					}
				}
			}
		}
	}

	//Verifica paredes, teto e chao
	if(verificationCount%3 == 2)
	{
		if(x > 1.0) velocityX = -module(velocityX);
		if(x < -1.0) velocityX = module(velocityX);
		if(y > 1.0) velocityY = -module(velocityY);

		if(y < -0.9) fade(); //Sumir é igual a morrer
	}

	//Faz o movimento em si
	x += velocityX;
	y += velocityY;
}

void Ball::render()
{
	glPushMatrix();

	glTranslatef(-x,y,z);
	glColor4fv(color);
	glutSolidSphere(size,9,9);

	glPopMatrix();
}

void Ball::increaseVelocity()
{
	isGonnaIncreaseVelocity = true;
}


void Ball::print()
{
	cout << "\n	Ciclo de verificacao (0 col. com raquete, 1 col. com fase, 2 col. com borda) -> " << verificationCount;
	cout << "\n	Numero de combinacao " << ballCombo;
	cout << "\n	Se esta sumindo -> " << fading;
	cout << "\n	Cor da bola -> " << color[0] << " " << color[1] << " " << color[2] << " " << color[3];
	cout << "\n	Vai aumentar velocidade? " << isGonnaIncreaseVelocity;
	Entity::print();
}

void Ball::reflect(double normal[])
{
	normal = normalize2D(normal);

	double vetorVelocidade[] = {velocityX, velocityY};

	double angulo = scalarMult2D(normal,vetorVelocidade);

	//cout << "\n norm" << normal[1] << " ang" << angulo << " vel" << velocidadeY;
	
	velocityX -= 2 * normal[0] * (angulo);
	velocityY -= 2 * normal[1] * (angulo);
}
void Ball::fade()
{
	fading = true;
	color[3] -= 0.02;
}
void Ball::getAngry()
{
	if(!angry)
	{
		angry = true;
		velocityX *= 2;
		velocityY *= 2;
		color[0] = 1.0; color[1] = 0.0; color[2] = 0.0;
	}
}
void Ball::getCalm()
{
	if(angry)
	{
		angry = false;
		velocityX /= 2;
		velocityY /= 2;
	}
	if(poisonous)
	{
		poisonous = false;
	}
	color[0] = 1.0; color[1] = 1.0; color[2] = 1.0;
}
void Ball::getPoisonous()
{
	if(!angry)
	{
		poisonous = true;
		color[0] = 1.0; color[1] = 0.0; color[2] = 1.0;
	}
}

void Ball::init()
{
	verificationCount = 0;
	hitRacketCount = 0;
	size = BALL_SIZE;
	velocityY = -VELOCIDADE_INICIAL;
	velocityX = 0.0;
	color[0] = 1.0; color[1] = 1.0; color[2] = 1.0; color[3] = 1.0;
	fading = false;
	angry = false;
	ballCombo = 0;
	isGonnaIncreaseVelocity = false;
	
	y = 0.0;
	x = 0.0;
}
Box* Ball::collisionBox()
{
	Box* c = new Box();

	c->minX = x - size/2.0;
	c->maxX = x + size/2.0;
	c->minY = y - size/2.0;
	c->maxY = y + size/2.0;
	
	return c;
}
