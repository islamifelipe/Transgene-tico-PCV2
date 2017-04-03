#ifndef _VERTICE_H_
#define _VERTICE_H_


#include "Vertice.h"
#include "Aresta.h"
#include <iostream>


Vertice::Vertice (int id2){ // n = número de vértices do grafo
	id = id2;
}
Vertice::Vertice(){};
		
int Vertice::getId(){
	return id;
}
void Vertice::adicionaAresta(Aresta *a){
	adjacentes[adjacentes.size()] = a;			
}
		
Aresta* Vertice::getAresta(int i){ /*retorna um ponteiro para a i-ésima aresta insidente no vértice*/
	return adjacentes[i];		
}
		
int Vertice::getGrau(){ 
	return adjacentes.size();
}

#endif