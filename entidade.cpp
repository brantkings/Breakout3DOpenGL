#include "entidade.h"

Entidade::Entidade()
{
	x = 0.0;
	y = 0.0;
	z = 1.0; //Z mínima para aparecer no Frustum
	altura = 0.0;
	largura = 0.0;
	tamanho = 0.0;
	velocidadeX = 0.0; velocidadeY = 0.0;
}

bool Entidade::colide(Entidade* e)
{
	if(e==NULL) return false;

	bool bateuX;
	bool bateuY;
	bool bateu;	

	Caixa* c1 = this->caixaColisao();
	Caixa* c2 = e->caixaColisao();
	
	//Verifica se o max ou o min de cada um está entre os limites do outro, e vice-versa, para o X e para o Y
	bateuX = (c2->minX > c1->minX && c2->minX < c1->maxX) || (c2->maxX > c1->minX && c2->maxX < c1->maxX)
	|| (c1->minX > c2->minX && c1->minX < c2->maxX) || (c1->maxX > c2->minX && c1->maxX < c2->maxX);
	bateuY = (c2->minY > c1->minY && c2->minY < c1->maxY) || (c2->maxY > c1->minY && c2->maxY < c1->maxY)
	|| (c1->minY > c2->minY && c1->minY < c2->maxY) || (c1->maxY > c2->minY && c1->maxY < c2->maxY);
	bateu = bateuX && bateuY;

	delete c1; delete c2;

	return bateu;
}
void Entidade::imprimir()
{
	cout << "\n		Entidade: Pos -> {" << x << " , " << y << " , " << z << "};";
	cout << "\n		Entidade: Largura -> " << largura << " | Altura -> " << altura << " | Tamanho -> " << tamanho;
}


Entidade::~Entidade()
{
}
