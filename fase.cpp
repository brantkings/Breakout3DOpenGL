#include "fase.h"


int Tijolo::ID_GERAL = 0;

Tijolo::Tijolo(Fase* f,TipoTijolo t)
{
	id = ID_GERAL;
	ID_GERAL++;

	pai = f;

	tipo = t;

	tamanho = 0.1;
	altura = 1.0;
	largura = 3.0;

	sumindo = false;
	
	switch(tipo)
	{
		case NORMAL:
			cor[0] = 1.0; cor[1] = 1.0; cor[2] = 0.0; cor[3] = 1.0;
			valor = 10;
			break;
		case DURO:
			cor[0] = 0.7; cor[1] = 0.7; cor[2] = 0.0; cor[3] = 1.0;
			valor = 20;
			break;
		case FORTE:
			cor[0] = 1.0; cor[1] = 0.2; cor[2] = 0.0; cor[3] = 1.0;
			valor = 30;
			break;
		case EXTRA:
			cor[0] = 0.0; cor[1] = 1.0; cor[2] = 0.0; cor[3] = 1.0;
			valor = 5;
			break;
		case BONUS:
			cor[0] = 0.0; cor[1] = 0.0; cor[2] = 1.0; cor[3] = 1.0;
			valor = 100;
			break;
		case VENENO:
			cor[0] = 1.0; cor[1] = 0.0; cor[2] = 1.0; cor[3] = 1.0;
			valor = 30;
			break;
		case NORMAL_MARCADO:
			cor[0] = 1.0; cor[1] = 1.0; cor[2] = 0.0; cor[3] = 1.0;
			valor = 12; //No meio vale mais
			break;
		
	}

	x = 0.0;
	y = -0.8;
	z = 1.0;
	//Tamanho é a base do cubo, altura e largura são multiplicadores
}

Tijolo::~Tijolo()
{
	
}

void Tijolo::render()
{
	//cout << "\nRender ID " << id << ": " << x << " " << y << " " << z;
	glPushMatrix();

	glTranslatef(-x,y,z);
	glColor4dv(cor);
	glScalef(largura,altura,1.0);

	glutSolidCube(tamanho);

	glPopMatrix();
}

void Tijolo::update(Jogo* j, unsigned int tempo)
{
	if(sumindo) sumir();
}

void Tijolo::sumir()
{
	sumindo = true;
	cor[3] -= 0.05;
	//if(cor[3] <= 0) l->remove(this);
}

Caixa* Tijolo::caixaColisao()
{
	Caixa* c = new Caixa();

	c->minX = x - tamanho * largura / 2.0;
	c->maxX = x + tamanho * largura / 2.0;
	c->minY = y - tamanho * altura / 2.0;
	c->maxY = y + tamanho * altura / 2.0;
	
	return c;
}

//______________________________________________________________________________________________________________________________

Fase::Fase()
{
	volta = 0.0;

	listaProx = new ListaMatriz();

	matrizTijolo = criaMatriz(RANDOM_FASE);

	for(int i=0;i<LISTA_FASES;i++)
	{
		listaProx->push_back(criaMatriz(RANDOM_FASE));
	}
	
}
Matriz* Fase::criaMatriz(int tijolos)
{
	Matriz* mat = new Matriz();
	mat->numeroTijolos = tijolos;

	int matrizTipos[LARGURA_FASE][ALTURA_FASE];

	//Limpa a matriz
	for(int i = 0;i<LARGURA_FASE;i++)
	{
		for(int j = 0; j < ALTURA_FASE; j++)
		{
			matrizTipos[i][j] = -1;
		}
	}

	//Cria sementes
	int numSementes = 1 + (rand() % 5);
	while(numSementes>0)
	{
		numSementes--;
		int i = rand() % LARGURA_FASE;
		int j = rand() % ALTURA_FASE;

		if(matrizTipos[i][j] == -1)
		{
			tijolos--;
			matrizTipos[i][j] = NORMAL;
		}
	}

	//"Incha" sementes até ter n tijolos
	while(tijolos > 0)
	{
		for(int i = 0;i<LARGURA_FASE && tijolos > 0;i++)
		{
			for(int j = 0; j < ALTURA_FASE && tijolos > 0; j++)
			{
				int novoI, novoJ;
				if(matrizTipos[i][j] == NORMAL)
				{
					matrizTipos[i][j] = NORMAL_MARCADO;

					//Incha cima
					novoJ = j-1;
					if(novoJ>=0 && matrizTipos[i][novoJ] == -1)
					{
						matrizTipos[i][novoJ] = NORMAL;
						tijolos--;
					}
					//Incha baixo
					novoJ = j+1;
					if(novoJ<ALTURA_FASE && matrizTipos[i][novoJ] == -1)
					{
						matrizTipos[i][novoJ] = NORMAL;
						tijolos--;
					}
					//Incha esquerda
					novoI = i-1;
					if(novoI>=0 && matrizTipos[novoI][j] == -1)
					{
						matrizTipos[novoI][j] = NORMAL;
						tijolos--;
					}
					//Incha direita
					novoI = i+1;
					if(novoI<LARGURA_FASE && matrizTipos[novoI][j] == -1)
					{
						matrizTipos[novoI][j] = NORMAL;
						tijolos--;
					}
				
					//Melhora tijolo
					int chance = lrand48() % 101; //O a 100
					if(chance < CHANCE_DURO)
					{
						matrizTipos[i][j] = DURO;
					}
					else if(chance < CHANCE_DURO + CHANCE_FORTE)
					{
						matrizTipos[i][j] = FORTE;
					}
					else if(chance < CHANCE_DURO + CHANCE_FORTE + CHANCE_BONUS)
					{
						matrizTipos[i][j] = BONUS;
					}
					else if(chance < CHANCE_DURO + CHANCE_FORTE + CHANCE_EXTRA + CHANCE_BONUS)
					{
						matrizTipos[i][j] = EXTRA;
					}
					else if(chance < CHANCE_DURO + CHANCE_FORTE + CHANCE_EXTRA + CHANCE_BONUS + CHANCE_VENENO)
					{
						matrizTipos[i][j] = VENENO;
					}
				}
			}
		}
	}
	if(tijolos < 0) mat->numeroTijolos -= tijolos;

	//Cria tijolos em si
	for(int i = 0;i < ALTURA_FASE;i++) 
	{
		for(int j = 0;j<LARGURA_FASE;j++)
		{
			if(matrizTipos[j][i]!=-1)
			{
				Tijolo* t = new Tijolo(this, (TipoTijolo) matrizTipos[j][i]);
				t->x = j * t->tamanho * t->largura - 1.0;
				t->y = i * t->tamanho * t->altura + 0.20;
				mat->m[j][i] = t;
	
			}
			else mat->m[j][i] = NULL;
		}
	}
	return mat;
}
void Fase::updateMatriz(Jogo* jog, unsigned int tempo, Matriz* mat)
{
	for(int i = 0;i < ALTURA_FASE;i++) for(int j = 0;j<LARGURA_FASE;j++)
	{
		Tijolo* t = mat->m[j][i];
		if(t!=NULL)
		{
			t->update(jog, tempo);
			if(t->cor[3] < 0)
			{
				mat->numeroTijolos--;
				delete t;
				mat->m[j][i] = NULL;
			}
		}
	}
}
void Fase::renderMatriz(Matriz* mat, bool fosco)
{
	for(int i = 0;i < ALTURA_FASE;i++) 
	{
		for(int j = 0;j<LARGURA_FASE;j++)
		{
			Tijolo* t = mat->m[j][i];
			if(t!=NULL)
			{
				if(fosco) t->cor[3] -= 0.6;
				if(!t->sumindo) t->render();
				if(fosco) t->cor[3] += 0.6;
			}
		}
	}
	if(fosco) //Se é fosco tá atras, se tá atras não tem sumindo
	{
		for(int i = 0;i < ALTURA_FASE;i++) 
		{
			for(int j = 0;j<LARGURA_FASE;j++)
			{
				Tijolo* t = mat->m[j][i];
				if(t!=NULL)
				{
					if(t->sumindo) t->render();
				}
			}
		}
	}
	
}
void Fase::update(Jogo* jog, unsigned int tempo)
{
	updateMatriz(jog,tempo,matrizTijolo);

	if(matrizTijolo->numeroTijolos <= 0 && jog->bola->y <= 0.0)
	{
		vem(jog);
		jog->bola->aumentarVelocidade();
	} 
}
void Fase::render()
{
	//Começa uma matriz nova
	//int i = 0;
	glPushMatrix();
	ListaMatriz::iterator it;
	glTranslatef(0.0,0.0,-volta); //Pra fazer tudo voltar
	//size_t tamLista = listaProx->size();
	for(it = (listaProx->begin()); it != (listaProx->end()); it++)
	{
		if((*it) != NULL)
		{ 
			//cout << "\n" << i << "\n";
			//glPushMatrix();
			//glTranslatef(0.0,0.0,(tamLista - i)*DISTANCIA_FASE);
			glTranslatef(0.0,0.0,DISTANCIA_FASE);
			renderMatriz(*it,true);
			//glPopMatrix();
		}
	}
	//Termina matriz
	glPopMatrix();

	
	renderMatriz(matrizTijolo,false);
	
	
}

void Fase::vem(Jogo* j)
{
	if(volta==0 && matrizTijolo!=NULL)
	{
		//Dá os parabéns pro jogador!
		j->parabens();
		
		for(int i = 0;i < ALTURA_FASE;i++) 
		{
			for(int j = 0;j<LARGURA_FASE;j++)
			{
				Tijolo* t = matrizTijolo->m[j][i];
				if(t!=NULL)
				{
					t->sumir();
				}
			}
		}

	}
	volta += 0.01;
	if(volta >= DISTANCIA_FASE)
	{
		delete matrizTijolo;
		matrizTijolo = NULL;
		matrizTijolo = *(listaProx->begin());
		listaProx->pop_front();
		volta = 0.0;


		listaProx->push_back(criaMatriz(RANDOM_FASE));
	}
}

void Fase::init()
{
	delete matrizTijolo;
	matrizTijolo = NULL;
	matrizTijolo = *(listaProx->begin());
	listaProx->pop_front();
	volta = 0.0;

	listaProx->push_back(criaMatriz(RANDOM_FASE));
}

void Fase::imprimir()
{
	cout << "\n	Numero de tijolos na matriz atual " << matrizTijolo->numeroTijolos << ".";
	if(vindo > 0)cout << "\n	A fase está vindo!";
	cout << "\nExiste uma fila de " << LISTA_FASES << " matrizes de fases à seguir."; 
}
Fase::~Fase()
{
	//delete listaTijolos;
	//for(int i = 0;i < ALTURA_FASE;i++) for(int j = 0;j<LARGURA_FASE;j++)
	//{
	//	Tijolo* t = matrizTijolo[j][i];
	//	if(t!=NULL)
	//	{
	//		delete t;
	//	}
	//}
}

