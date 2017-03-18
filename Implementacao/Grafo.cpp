#include "Grafo.h"
#include <iostream>
using namespace std;

Grafo::Grafo(){};
Grafo::~Grafo(){}
Grafo::Grafo(const Grafo& t){
	n = t.n;
	lista_vertices = t.lista_vertices;
	adjacencia = t.adjacencia; 
	
	for (int i=0; i<t.lista_allArestas.size(); i++){
		particao[i] = t.particao.at(i);
	}
	lista_allArestas = t.lista_allArestas;
	//cout<<"copieeeee"<<endl;
}
Grafo::Grafo(int n1){
	n = n1;
	lista_vertices = new Vertice*[n];
	// for (int i=0; i<lista_allArestas.size(); i++){
	// 	particao[i] = 0; /*inicialmente opcional*/
	// }
	adjacencia = new int*[n];
	for (int i=0; i<n; i++){
		adjacencia[i] = new int[n];
		for (int j=0; j<n; j++){
			adjacencia[i][j] = -1;
		}
	}
}

int Grafo::isAdjacente(int i, int j){
	return adjacencia[i][j];
}
void Grafo::addVertice(int id){
	Vertice *novo = new Vertice(id);
	lista_vertices[id] = novo; /*PADRÃO: A faixa de id's dos vértices é de 0 até n-1*/
}

Vertice *Grafo::getVertice(int id){
	return lista_vertices[id]; 		
}

Aresta *Grafo::addAresta(int id, int origem, int destino, float peso1){
	Aresta *nova = new Aresta(id, origem, destino, peso1);
	//lista_allArestas.push_back(nova); /*deve ser passado o ponteiro, isto é, a referência*/
	lista_allArestas[id] = nova;
	lista_vertices[origem]->adicionaAresta(nova);
	lista_vertices[destino]->adicionaAresta(nova);
	particao[id]=0;
	adjacencia[origem][destino] = id;
	adjacencia[destino][origem] = id;
	return nova;
}

Aresta * Grafo::getArestas(int i){ /*retorna a primeira posição do vetor lista_allArestas*/
	return lista_allArestas[i];
}
int Grafo::getQuantArestas(){
	return lista_allArestas.size();
}
map <int, Aresta *> Grafo::get_allArestas(){
	return lista_allArestas;

}

int Grafo::getQuantVertices(){	
	return n;	
}
Aresta ** Grafo::getAllArestasPtr(){
	Aresta **arestasPtr = new Aresta*[lista_allArestas.size()];
	for (int i=0; i<lista_allArestas.size(); i++){
		arestasPtr[i] = lista_allArestas[i];
	}
	return arestasPtr;
}
// void Grafo::gerarArestasPtr(){
// 	Aresta **arestasPtr = new Aresta*[lista_allArestas.size()];
// 	for (int i=0; i<lista_allArestas.size(); i++){
// 		arestasPtr[i] = lista_allArestas[i];
// 	}
// }
int Grafo::getStatus(int i){
	return particao[i];
}
void Grafo::setStatus(int i, int valor){
	particao[i] = valor;
}

// Grafo& Grafo::operator=( Grafo& d){
	
// 	if (this == &d) return *this;
// 	n = d.getQuantVertices();
// 	for (int i=0; i<d.lista_allArestas.size(); i++){
// 		particao[i] = d.getStatus(i);
// 	}
// 	lista_allArestas = d.lista_allArestas;
// 	return *this;
	
// }

