#include "bola.h"
Bola::Bola()
{
	init();
}

Bola::~Bola()
{
}

void Bola::update(Jogo* j,unsigned int tempo)
{
	countVerifica++;

	if(sumindo)
	{
		sumir();
		if(cor[3] < 0.0)
		{
			init();
			j->vidas--;
			j->pausar();
		}
	}

	//Verifica a raquete
	if(countVerifica%3 == 0)
	{
		countVerifica = 0;

		if(this->bateuRaqueteCount<=0)
		{
			if(colide(j->jogador))
			{
				double n[] = {0.0,1.0};
				refletir(n);
				
				bolaCombo = 0;
	
				velocidadeX += (x - j->jogador->x)/20.0;

				this->bateuRaqueteCount = 20; //Evitar varias reflexões

				if(vaiAumentarVelocidade) 
				{
					velocidadeX *= 1.05; velocidadeY *= 1.05;
				}
	
				j->jogador->curar();

				if(nervosa)
				{
					ficarCalma();
				}

				if(venenosa)
				{
					j->jogador->envenenar();
					ficarCalma();
				}
			}
		}
		else this->bateuRaqueteCount--;
	}

	//Verifica a fase
	if(countVerifica%3 == 1)
	{
		for(int i = 0;i < ALTURA_FASE;i++) for(int k = 0;k < LARGURA_FASE;k++)
		{
			Tijolo* t = j->faseAtual->matrizTijolo->m[k][i];
			if(t!=NULL)
			{
				if(!t->sumindo)
				{
					if(colide(t))
					{
						double nX,nY;
						if(x < t->x - t->tamanho * t->largura /2) {nX = -1.0; nY = 0.0;}
						if(x > t->x + t->tamanho * t->largura /2) {nX = 1.0; nY = 0.0;}
						else {nX = 0.0; nY = -1.0;}
						double n[] = {nX,nY};
						refletir(n);

						int pontuacao = t->valor + t->valor * (bolaCombo + 1) / 2;				
			

						TipoTijolo tipt = t->tipo;

						bolaCombo++;

						double pontoX = (x + t->x*2.0)/3.0;
						double pontoY = (y + t->y*2.0)/3.0;

						switch(tipt)
						{
							case NORMAL:
								j->escreverPontos(pontuacao,pontoX,pontoY);
								j->pontos += pontuacao;
								t->sumir();
								break;
							case DURO:
								t->cor[0] = 1.0; t->cor[1] = 1.0; t->cor[2] = 0.0; t->cor[3] = 0.85;
								t->tipo = NORMAL;
								break;
							case FORTE:
								j->escreverPontos(pontuacao,pontoX,pontoY);
								j->pontos += pontuacao;
								ficarNervosa();
								t->sumir();
								break;
							case EXTRA:
								j->escreverPontos(pontuacao,pontoX,pontoY);
								j->pontos += pontuacao;
								if(j->vidas < 5) j->vidas++;
								t->sumir();
								break;
							case BONUS:
								j->escreverPontos(pontuacao,pontoX,pontoY);
								j->pontos += pontuacao;
								t->sumir();
								break;
							case VENENO:
								ficarVenenosa();
								j->escreverPontos(pontuacao,pontoX,pontoY);
								j->pontos += pontuacao;
								t->sumir();
							case NORMAL_MARCADO:
								t->sumir();
								break;
						}

						break;
					}
				}
			}
		}
	}

	//Verifica paredes, teto e chao
	if(countVerifica%3 == 2)
	{
		if(x > 1.0) velocidadeX = -modulo(velocidadeX);
		if(x < -1.0) velocidadeX = modulo(velocidadeX);
		if(y > 1.0) velocidadeY = -modulo(velocidadeY);

		if(y < -0.9) sumir(); //Sumir é igual a morrer
	}

	//Faz o movimento em si
	x += velocidadeX;
	y += velocidadeY;
}

void Bola::render()
{
	glPushMatrix();

	glTranslatef(-x,y,z);
	glColor4fv(cor);
	glutSolidSphere(tamanho,9,9);

	glPopMatrix();
}

void Bola::aumentarVelocidade()
{
	vaiAumentarVelocidade = true;
}


void Bola::imprimir()
{
	cout << "\n	Ciclo de verificacao (0 col. com raquete, 1 col. com fase, 2 col. com borda) -> " << countVerifica;
	cout << "\n	Numero de combinacao " << bolaCombo;
	cout << "\n	Se esta sumindo -> " << sumindo;
	cout << "\n	Cor da bola -> " << cor[0] << " " << cor[1] << " " << cor[2] << " " << cor[3];
	cout << "\n	Vai aumentar velocidade? " << vaiAumentarVelocidade;
	Entidade::imprimir();
}

void Bola::refletir(double normal[])
{
	normal = normalize2D(normal);

	double vetorVelocidade[] = {velocidadeX, velocidadeY};

	double angulo = escalarMult2D(normal,vetorVelocidade);

	//cout << "\n norm" << normal[1] << " ang" << angulo << " vel" << velocidadeY;
	
	velocidadeX -= 2 * normal[0] * (angulo);
	velocidadeY -= 2 * normal[1] * (angulo);
}
void Bola::sumir()
{
	sumindo = true;
	cor[3] -= 0.02;
}
void Bola::ficarNervosa()
{
	if(!nervosa)
	{
		nervosa = true;
		velocidadeX *= 2;
		velocidadeY *= 2;
		cor[0] = 1.0; cor[1] = 0.0; cor[2] = 0.0;
	}
}
void Bola::ficarCalma()
{
	if(nervosa)
	{
		nervosa = false;
		velocidadeX /= 2;
		velocidadeY /= 2;
	}
	if(venenosa)
	{
		venenosa = false;
	}
	cor[0] = 1.0; cor[1] = 1.0; cor[2] = 1.0;
}
void Bola::ficarVenenosa()
{
	if(!nervosa)
	{
		venenosa = true;
		cor[0] = 1.0; cor[1] = 0.0; cor[2] = 1.0;
	}
}

void Bola::init()
{
	countVerifica = 0;
	bateuRaqueteCount = 0;
	tamanho = TAMANHO_BOLA;
	velocidadeY = -VELOCIDADE_INICIAL;
	velocidadeX = 0.0;
	cor[0] = 1.0; cor[1] = 1.0; cor[2] = 1.0; cor[3] = 1.0;
	sumindo = false;
	nervosa = false;
	bolaCombo = 0;
	vaiAumentarVelocidade = false;
	
	y = 0.0;
	x = 0.0;
}
Caixa* Bola::caixaColisao()
{
	Caixa* c = new Caixa();

	c->minX = x - tamanho/2.0;
	c->maxX = x + tamanho/2.0;
	c->minY = y - tamanho/2.0;
	c->maxY = y + tamanho/2.0;
	
	return c;
}
