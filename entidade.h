#ifndef ENTIDADE_H
#define ENTIDADE_H

#include "util.h"

using namespace std;

class Jogo;

struct Caixa
{
	double minX,maxX,minY,maxY;
	double normal[2];
	Caixa()
	{
		minX = 0.0; maxX = 0.0; minY = 0.0; maxY = 0.0;
		normal[0] = 0.0; normal[1] = 0.0;
	}
	void imprimir()
	{
		cout << "\nCaixa: X= " << minX << " ate " << maxX << " | Y= " << minY << " ate " << maxY; 
	}
};
class Entidade
{
protected:
	double velocidadeX, velocidadeY;

public:
	Entidade();
	virtual ~Entidade();

	double x,y,z;
	double largura,altura,tamanho;
	
	virtual void update(Jogo* j,unsigned int tempo) = 0;
	virtual void render() = 0;
	virtual void imprimir();
	virtual Caixa* caixaColisao() = 0;
	bool colide(Entidade* e);
};

#endif
