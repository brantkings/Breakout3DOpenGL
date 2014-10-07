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

//General
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define FPS 60
#define GAME_NAME "Fantastic Breakout"
#define SCREEN_BORDER 1.0

//Ball
#define BALL_SIZE 0.025

//Racket
#define MAXIMUM_VELOCITY 0.03
#define MOUSE_CONTROL_MIN 0.4
#define MOUSE_CONTROL_MAX 0.6
#define AIR_RESISTANCE 0.9

using namespace std;

//General Functions
double* normalize2D(double vetor[]);
double scalarMult2D(double vetor1[], double vetor2[]);
double module(double d);

//Funções texto
void writeText(char* texto, double position[3], double size[3], float color[4]);

#endif
