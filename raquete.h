#ifndef RAQUETE_H
#define RAQUETE_H

#include "bola.h"
#include "jogo.h"


class Raquete : public Entidade
{
	//double tamanho;
	double velocidadeX;
	bool envenenado;
public:
	Raquete();
	virtual void update(Jogo* j,unsigned int tempo);
	virtual void render();
	virtual void init();
	void envenenar();
	void curar();
	void imprimir();
	virtual Caixa* caixaColisao();
	virtual ~Raquete();
};

#endif
