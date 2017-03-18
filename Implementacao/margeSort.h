#ifndef _MARGESORT_H_
#define _MARGESORT_H_

#include "Aresta.h"
#define PRECISAO 0.00001

void intercala(int p, int q, int r, Aresta **v, int size);
void mergesort(Aresta **v, int size);



bool equalfloat(float a, float b);


bool maiorQuefloat(float a, float b);

bool maiorIgualQuefloat(float a, float b);

#endif