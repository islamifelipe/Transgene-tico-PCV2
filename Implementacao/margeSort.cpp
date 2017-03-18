#include "Conjunto.h"
#include "Grafo.h"
#include "margeSort.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>  
using namespace std;

bool equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

bool maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}

bool maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
}

/*Implementação do MargeSort para utilizar no algortimo de Kruskal*/
void intercala(int p, int q, int r, Aresta **v, int size){
 
	int i, j, k;
	Aresta **w = new Aresta *[size];
	float peso_i, peso_j; 
	i = p;
	j = q;
	k = 0;
	while (i < q && j < r) {
		if (!maiorIgualQuefloat(v[i]->getPeso(), v[j]->getPeso())) {
			w[k] = v[i];
			i++;
		} else {
			w[k] = v[j];
			j++;
		}
		k++;
	}
	while (i < q) {
		w[k] = v[i];
		i++;
		k++;
	}
	while (j < r) {
		w[k] = v[j];
		j++;
		k++;
	}
	for (i = p; i < r; i++) v[i] = w[i-p];
	delete[] w;
}

void mergesort(Aresta **v, int size){
//v é um vetor de ponteiros do tipo Aresta (as arestas são ponteitos)
//implementação interativa

	int p, r, b=1;;
	while (b<size){
		p=0;
		while (p+b < size){
			r = p + 2*b;
			if (r>size) r = size;
			intercala(p, p+b, r, v, size);
			p = p+2*b;
		}
		b = 2*b;
	}
}