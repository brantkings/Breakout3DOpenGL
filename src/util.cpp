#include "util.h"

double* normalize2D(double vetor[])
{
	double module = sqrt(vetor[0] * vetor[0] + vetor[1] * vetor[1]);
	vetor[0] /= module;
	vetor[1] /= module;
	return vetor;
}
double scalarMult2D(double vetor1[], double vetor2[])
{
	return vetor1[0] * vetor2[0] + vetor1[1] * vetor2[1];
}
double module(double d)
{
	if(d<0.0) d = -d;
	return d;
}

void writeText(char* texto, double posicao[3], double tamanho[3], float cor[4])
{
	GLFONT font;

	//Enable text
	glEnable(GL_TEXTURE_2D);

	//Create font
	glFontCreate(&font, "biondi.glf", 0);

	//Inverte a fonte pro lugar certo
	//tamanho[0] = -tamanho[0];

	//Draw font
	glFontBegin(&font);
		glPushMatrix();
		glColor4fv(cor);
		glTranslatef(posicao[0],posicao[1],posicao[2]);
		glScalef(tamanho[0],tamanho[1],tamanho[2]);
		glFontTextOut(texto, 0, 0, 0);
		glPopMatrix();
	glFontEnd();

	//Destroy font.
	glFontDestroy(&font);

	//Disable texture or else nothing else appears.
	glDisable(GL_TEXTURE_2D);
}
