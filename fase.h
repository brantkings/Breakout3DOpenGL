#ifndef FASE_H
#define FASE_H

#include "entidade.h"
#include "jogo.h"

#define ALTURA_FASE 7
#define LARGURA_FASE 8
#define LISTA_FASES 20
#define DISTANCIA_FASE 0.1
#define RANDOM_FASE (lrand48() % 56) + 1

#define CHANCE_DURO 30
#define CHANCE_FORTE 20
#define CHANCE_EXTRA 5
#define CHANCE_BONUS 15
#define CHANCE_VENENO 15

using namespace std;


class Fase;

enum TipoTijolo
{
	NORMAL,
	DURO,
	FORTE,
	EXTRA,
	BONUS,
	VENENO,
	NORMAL_MARCADO
};

class Tijolo : public Entidade
{
	Fase* pai;
public:
	static int ID_GERAL;
	int id;
	bool sumindo;
	double cor[4];

	TipoTijolo tipo;

	int valor;

	Tijolo(Fase*, TipoTijolo tipo);
	virtual void update(Jogo* j,unsigned int tempo);
	virtual void render();
	virtual Caixa* caixaColisao();
	void sumir();
	virtual ~Tijolo();
};

typedef Tijolo* MatrizTijolo[LARGURA_FASE][ALTURA_FASE];

struct Matriz
{
	int numeroTijolos;
	MatrizTijolo m;
	Matriz()
	{
		for(int i = 0;i < ALTURA_FASE;i++) for(int j = 0;j<LARGURA_FASE;j++) m[j][i] = NULL;
	}
	~Matriz()
	{
		for(int i = 0;i < ALTURA_FASE;i++) for(int j = 0;j<LARGURA_FASE;j++) if(m[j][i] != NULL) {delete m[j][i]; m[j][i] = NULL;}
	}
};

typedef list<Matriz*> ListaMatriz;

class Fase
{
	int numeroTijolos;
	float volta;
	bool vindo;

public:	
	ListaMatriz* listaProx;
	Matriz* matrizTijolo;	

	Fase();
	Matriz* criaMatriz(int tijolos);
	void updateMatriz(Jogo* j, unsigned int tempo, Matriz*);
	void renderMatriz(Matriz*, bool fosco);
	void vem(Jogo* j);
	void update(Jogo* j, unsigned int tempo);
	void render();
	void init();
	void imprimir();
	~Fase();
};

#endif
