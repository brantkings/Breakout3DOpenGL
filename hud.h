#ifndef HUD_H
#define HUD_H

#include "raquete.h"
#include "jogo.h"

struct EscritorPontos
{
	float x,y,alpha,tamanho;
	char texto[20];
	EscritorPontos(char* oq,float x,float y,float tam);
	void update();
	void render();
};
struct MostradorVidas
{
	double x,y,z;
	MostradorVidas(double, double, double);
	void render(int vidas, double tamanho);
};

typedef list<EscritorPontos*> ListaEscritores;

class HUD
{
	Jogo* pai;
	MostradorVidas* mostrador;
	ListaEscritores* listaEscritor;
public:
	HUD(Jogo* j);
	void render();
	void update(Jogo* j, unsigned int tempo);
	void imprimir();
	void adicionarMarcadorPontos(char* texto, float x, float y);
	~HUD();
};

#endif
