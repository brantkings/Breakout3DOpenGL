#include "raquete.h"

Raquete::Raquete()
{
	init();
}

Raquete::~Raquete()
{
	
}

void Raquete::render()
{
	//Desenhar paddle em si
	glPushMatrix();

	glTranslatef(-x,y,z);
	glScalef(largura,altura,1.0);
	
	if(envenenado) glColor4f(1.0,0.0,1.0,1.0);
	else glColor4f(1.0,1.0,1.0,1.0);
	glutSolidCube(tamanho);

	glPopMatrix();
}

void Raquete::update(Jogo* j, unsigned int tempo)
{
	//Muda a velocidade
	if(j->mousePosX < TELA_WIDTH*MOUSE_CONTROLE_MENOR)
	{
		double limiteIntervaloMouse = TELA_WIDTH*MOUSE_CONTROLE_MENOR - 0.05;	
		velocidadeX = -VELOCIDADE_MAXIMA*(limiteIntervaloMouse - j->mousePosX)/limiteIntervaloMouse;
	}
	else if(j->mousePosX >= TELA_WIDTH*MOUSE_CONTROLE_MENOR && j->mousePosX < TELA_WIDTH*MOUSE_CONTROLE_MAIOR)
	{
		//Faz nada
	}
	else
	{
		double limiteIntervaloMouse = TELA_WIDTH*MOUSE_CONTROLE_MENOR + 0.05;
		velocidadeX = VELOCIDADE_MAXIMA*(j->mousePosX - TELA_WIDTH*MOUSE_CONTROLE_MAIOR)/limiteIntervaloMouse;
		//velocidadeX += 0.005;// * tempo * 1000 / FPS;
	}

	//Resistencia do "ar" e restricoes
	velocidadeX *= RESISTENCIA_AR;
	if(!envenenado)
	{
		if(velocidadeX < -VELOCIDADE_MAXIMA) velocidadeX = -VELOCIDADE_MAXIMA;
		else if (velocidadeX > VELOCIDADE_MAXIMA) velocidadeX = VELOCIDADE_MAXIMA;
		else if (velocidadeX * velocidadeX < 0.00001) velocidadeX = 0;
	}
	else
	{
		if(velocidadeX < -VELOCIDADE_MAXIMA/4) velocidadeX = -VELOCIDADE_MAXIMA/4;
		else if (velocidadeX > VELOCIDADE_MAXIMA/4) velocidadeX = VELOCIDADE_MAXIMA/4;
		else if (velocidadeX * velocidadeX < 0.00001) velocidadeX = 0;
	}

	//Soma a velocidade
	x += velocidadeX;
	if(x < -BORDA_TELA) x = -BORDA_TELA;
	else if (x > BORDA_TELA) x = BORDA_TELA;

	//cout << "\n" << this->x << " " << this->y << " VELO:" << this->velocidadeX;

	//cout << "\n" << j->mousePosX << " " << j->mousePosY;
}

void Raquete::envenenar()
{
	envenenado = true;
}
void Raquete::curar()
{
	envenenado = false;
}

void Raquete::imprimir()
{
	cout << "\n	Velocidade no eixo X " << velocidadeX;
	Entidade::imprimir();
}

void Raquete::init()
{
	tamanho = 0.1;
	altura = 1.0;
	largura = 3.0;

	velocidadeX = 0.0;
	
	envenenado = false;

	x = 0.0;
	y = -0.8;
	z = 1.0;
	//Tamanho é a base do cubo, altura e largura são multiplicadores
}

Caixa* Raquete::caixaColisao()
{
	Caixa* c = new Caixa();

	c->minX = x - tamanho * largura / 2.0;
	c->maxX = x + tamanho * largura / 2.0;
	c->minY = y - tamanho * altura / 2.0;
	c->maxY = y + tamanho * altura / 2.0;
	
	return c;
}
