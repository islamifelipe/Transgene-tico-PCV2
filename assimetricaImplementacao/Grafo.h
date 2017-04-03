#ifndef _GRAFO_H_
#define _GRAFO_H_

#include "Vertice.h"
#include <map>

//#define MAX1 500000
class Grafo{
	private:
		int n;
		Vertice **lista_vertices; /*Lista (fixa) de vértice. É um vetor de ponteiros. Cada vértice possui uma lista de arestas adjacentes.*/
		map <int, Aresta *> lista_allArestas; /*lista de todas as arestas do Grafo.*/
		map <int, int> particao; // Size: quantidade de arestas 
		/*Este vetor (partição) é construído e interpretado da seguinte maneira:
			particao[i] = 0  ---> indica que a aresta de id=i é opcional (open)
			particao[i] = 1  ---> indica que a aresta de id=i é obrigatória 
			particao[i] = 2  ---> indica que a aresta de id=i é proibida
		*/
		int **adjacencia; // se adjacencia[i][j] = id, entao exite uma aresta de i pra j com id. Se adjacencia[i][j]=adjacencia[j][i], para todo i,j, entao o grafo nao é direcionado  
		//se adjacencia[i][j] = -1, entao nao existe aresta
	public:
		Grafo(int n1);
		Grafo();
		~Grafo();
		//Grafo(const Grafo& t);
		void addVertice(int id); // lista_vertices[id] = vertice de identificador id (entre 0 e n-1)
		Vertice *getVertice(int id);
		Aresta *addAresta(int id, int origem, int destino, float peso1);
		Aresta * getArestas(int i);
		map <int, Aresta *> get_allArestas();
		Aresta ** getAllArestasPtr();
		void gerarArestasPtr();
		int getQuantArestas();
		int getQuantVertices();
		int getStatus(int i);
		void setStatus(int i, int valor);
		Grafo(const Grafo& t);
		int isAdjacente(int i, int j);

};

#endif