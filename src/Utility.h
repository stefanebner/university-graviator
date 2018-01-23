//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Utility
//	Various calculations aiding to generate some positions
//
//*********************************************************

#ifndef UTILITY
#define UTILITY

#include "vec3f.h"
#include <cmath>


#define ARENA_RADIUS 100.0
#define PLAYER_SIZE 1.0

// 2D Calculations used in QPT2a
#define USE_2D_CALCULATION true


class Utility
{
public:
	Utility(void);
	~Utility(void);
};

vec3f setYtoArenaHeight(vec3f vector);	
vec3f transformToCalculationCoordinates(vec3f position);
vec3f transformToRenderingCoordinates(vec3f position);
vec3f getArenaNormalAtPosition(vec3f position);

double getPi();

#endif