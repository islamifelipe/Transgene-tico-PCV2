#include "Aresta.h"
using namespace std;

Aresta::Aresta(int id2, int origem1, int destino1, float peso_1){
	origem = origem1;
	destino = destino1;
	peso = peso_1;
	id= id2;
	//pesoUnique = 0;
} 

Aresta::Aresta(){}

int Aresta::getId(){
	return id;
}
int Aresta::getOrigem(){
	return origem;
}
int Aresta::getDestino(){
	return destino;
}
float Aresta::getPeso(){
	return peso;
}
void Aresta::setId(int novo){
	id = novo;
}
void Aresta::setOrigem(int n){
	origem = n;
}
void Aresta::setDestino(int n){	
	destino = n;	
}