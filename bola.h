#ifndef BOLA_H
#define BOLA_H

#define VELOCIDADE_INICIAL 0.015

#include "fase.h"

class Bola : public Entidade
{
	int bateuRaqueteCount;
	int countVerifica;
	int bolaCombo;
	bool sumindo;
	bool vaiAumentarVelocidade;
	bool nervosa;
	bool venenosa;
	float cor[4];
public:
	Bola();
	virtual void update(Jogo* j,unsigned int tempo);
	virtual void render();
	virtual Caixa* caixaColisao();
	virtual void init();
	void aumentarVelocidade();
	void ficarNervosa();
	void ficarCalma();
	void ficarVenenosa();
	void imprimir();
	void refletir(double normal[]);
	void sumir();
	virtual ~Bola();
};

#endif
