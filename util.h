#ifndef UTIL_H
#define UTIL_H

#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <iostream>
#include <list>
#include "glfont.h"

//Constantes gerais
#define TELA_WIDTH 800
#define TELA_HEIGHT 640
#define FPS 60
#define JOGO_NOME "Fantastic Breakout"
#define BORDA_TELA 1.0

//Constantes da bola
#define TAMANHO_BOLA 0.025

//Constantes da raquete
#define VELOCIDADE_MAXIMA 0.03
#define MOUSE_CONTROLE_MENOR 0.4
#define MOUSE_CONTROLE_MAIOR 0.6
#define RESISTENCIA_AR 0.9

using namespace std;

//Funções gerais
double* normalize2D(double vetor[]);
double escalarMult2D(double vetor1[], double vetor2[]);
double* vetorMult2D(double vetor1[], double vetor2[]); //Retorna um vetor 3D
double modulo(double d);

//Funções texto
void escreverTexto(char* texto, double posicao[3], double tamanho[3], float cor[4]);

#endif
