#ifndef JOGO_H
#define JOGO_H

#include "hud.h"

class Raquete;
class Bola;
class Fase;
class HUD;

struct Teclas
{
	bool a;
};
class Jogo
{
	bool pause;
	float angle;
	unsigned int tempo;
public:
	static Jogo* principal;
	bool gameOver;
	bool pronto;
	int vidas;
	int pontos;

	//HUD
	HUD* hud;

	//ELEMENTOS DO JOGO
	Raquete* jogador;
	Bola* bola;
	Fase* faseAtual;

	Teclas teclasApertadas;
	int mousePosX, mousePosY;

	Jogo();
	void inicializarOpenGL(int&, char**);
	void iniciar();
	void recomecar(); //Recomeça outra partida
	void pausar();
	void reset(); //Recomeça tudo em posição inicial
	void fimJogo();

	void render();
	void update(unsigned int tempo);
	void escreverPontos(int pontos, float x, float y);
	void parabens();

	void mouse(int botao, int estado, int x, int y);
	void mouseMove(int x, int y);
	void teclado(unsigned char tecla, int x, int y);

	void imprimir();

	~Jogo();
};

#endif
