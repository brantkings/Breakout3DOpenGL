#include "hud.h"

EscritorPontos::EscritorPontos(char* oq,float px,float py,float tam)
{
	strncpy(texto,oq,20);
	x = px;
	y = py;
	tamanho = tam;

	alpha = 1.0;
}
void EscritorPontos::update()
{
	y += 0.001;
	alpha -= 0.03;
}
void EscritorPontos::render()
{
	double pos[3] = {-x,y,1.0};
	double tam[3] = {tamanho,tamanho,tamanho};
	float cor[4] = {1.0,1.0,0.4,alpha};
	escreverTexto(texto ,pos, tam, cor);
}

//________________________________________________________________________________________

MostradorVidas::MostradorVidas(double px, double py, double pz)
{
	x = px; y = py; z = pz;
}
void MostradorVidas::render(int vidas, double tamanho)
{
	int i;

	//cout << "\nM " << tamanho << " " << vidas << " - " << x << " " << y << " " << z;

	for(i=0;i<vidas;i++)
	{
		glPushMatrix();
		glTranslatef(-x - tamanho * i * 2,y,z + 1.0);
		glColor4f(1.0,1.0,1.0,1.0);
		glutSolidSphere(tamanho,9,9);
		glPopMatrix();
	}
}


//________________________________________________________________________________________

HUD::HUD(Jogo* j)
{
	pai = j;
	mostrador = new MostradorVidas(0.75,-0.75,0.0);
	listaEscritor = new ListaEscritores();
}
void HUD::render()
{
	glPushMatrix();

	//Bolas restantes
	double tamanhoDaBola;
	if(pai==NULL) return;
	if(pai->bola != NULL) tamanhoDaBola = pai->bola->tamanho;
	else tamanhoDaBola = TAMANHO_BOLA;
	mostrador->render(pai->vidas,tamanhoDaBola);


	//Desenhar linhas divisórias, para auxílio do jogador
	if(!pai->gameOver)
	{
		glBegin(GL_LINES);
			glColor4f(1.0,1.0,1.0,0.2);
			glVertex3f(MOUSE_CONTROLE_MENOR*2 - 1.0,-1.0,1.0);
			glColor4f(1.0,1.0,1.0,0.0);
			glVertex3f(MOUSE_CONTROLE_MENOR*2 - 1.0,1.0,1.0);
			glColor4f(1.0,1.0,1.0,0.2);
			glVertex3f(MOUSE_CONTROLE_MAIOR*2 - 1.0,-1.0,1.0);
			glColor4f(1.0,1.0,1.0,0.0);
			glVertex3f(MOUSE_CONTROLE_MAIOR*2 - 1.0,1.0,1.0);
		glEnd();
	}

	//Desenha escritores de linha
	if(!listaEscritor->empty())
	{
		ListaEscritores::iterator it;
		for(it = listaEscritor->begin(); it != listaEscritor->end(); it++)
		{
			if ((*it) != NULL) (*it)->render();
		}
	}

	//Desenha placar
	char placar[30];
	for(int i=0;i<30;i++) placar[i] = 0;
	sprintf(placar,"%d",pai->pontos);

	double pos[3] = {0.8,-0.8,1.0};
	double tam[3] = {0.8,0.8,1.0};
	float cor[4] = {0.8,0.8,1.0,0.9};
	escreverTexto(placar ,pos, tam, cor);

	glPopMatrix();
}
void HUD::update(Jogo* j, unsigned int tempo)
{
	if(!listaEscritor->empty())
	{
		ListaEscritores::iterator it;

		//Marca para remover
		bool marcou = false;
		ListaEscritores::iterator marcador; //Só um morre de cada vez

		for(it = listaEscritor->begin(); it != listaEscritor->end(); it++)
		{
			if ((*it) != NULL) 
			{
				(*it)->update();
				if((*it)->alpha < 0) 
				{
					marcador = it;
					marcou = true;
				}
			}
		}

		//Remove o que marcou
		if (marcou) 
		{
			listaEscritor->remove(*marcador);
			delete *marcador;
		}
	}
}
void HUD::adicionarMarcadorPontos(char* texto, float x, float y)
{
	EscritorPontos* e = new EscritorPontos(texto, x, y, 1.0);
	listaEscritor->push_back(e);
}
void HUD::imprimir()
{
	cout << "\n	Mostrador de vidas, placar e " << listaEscritor->size() << " escritores de pontos.";
}
HUD::~HUD()
{
	
}


