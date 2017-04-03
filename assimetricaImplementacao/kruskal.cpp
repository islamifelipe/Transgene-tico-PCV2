#include "Conjunto.h"
#include "kruskal.h"
#include "Grafo.h"
#include "margeSort.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <vector>
#include <cmath>
#include <stack>  
using namespace std;

#define PRECISAO 0.00001



bool kruskal (Grafo *g, Aresta **listaAresta,  vector <Aresta *> &A, float &custo){
	//cout<<"kruskal"<<endl;
	Conjunto conjunto(g->getQuantVertices());
	int i=0;
	for (int k=0; k<g->getQuantArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		if (g->getStatus(listaAresta[k]->getId())==1){ /*se for obrigatória*/
		 	//A[cont++] = listaAresta[k]->getId();
			A.push_back(g->getArestas(listaAresta[k]->getId()));
		 	conjunto.union1(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino());
			custo+=listaAresta[k]->getPeso();
		}
	}
	i=0;
	while (A.size()<g->getQuantVertices()-1 && i<g->getQuantArestas()){ 
	/*A condição "i<g->getQuantArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getQuantArestas(), porém, o cont não será será igual a g->getN()-1 */
		
		if (g->getStatus(listaAresta[i]->getId())==0 && !conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			//A[cont++] = listaAresta[i]->getId();
			A.push_back(g->getArestas(listaAresta[i]->getId()));
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			custo+=listaAresta[i]->getPeso();
		}
		i++;
	}
	conjunto.desaloca();
	
	if (A.size()==g->getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
}