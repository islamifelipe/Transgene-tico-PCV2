#include "dijkstra.h"

// caminho mais curto
void dijkstra(Grafo *g, int origem, vector< Informacao > &result,int maximo){ // da origem para todos (Teorema 4.1 Goldbarg e Goldbarg 2012)
	//vector< Informacao >  result;
	int rot[g->getQuantVertices()];
	float dt[g->getQuantVertices()];// pode ser uma heap
	int A[g->getQuantVertices()]; // ainda nao visitados
	for (int i=0; i<g->getQuantVertices(); i++){
		dt[i] = INT_MAX; // pode ser uma heap
		rot[i] = 0;
		A[i] = 1;
	}
	dt[origem] = 0;
	rot[origem] = 0;
	int cont = 0; // conta quanto vertices ja foram visitados

	while (cont != g->getQuantVertices()){
		int r = INT_MAX;
		float min_r = INT_MAX;
		for (int i=0; i<g->getQuantVertices(); i++){
			if (A[i]==1 && dt[i]<min_r){
				r = i;
				min_r = dt[i];
			}
		}
		if (min_r!=INT_MAX){
			cont++;
			A[r] = 0;
			for (int i=0; i<g->getQuantVertices(); i++){
				if (g->isAdjacente(r,i)!=-1) { // se a resta i inside no vertice r
					float p = g->getArestas(g->isAdjacente(r,i))->getPeso() + dt[r];
					if (p<dt[i]){
						dt[i] = p;
						rot[i] = r;
					}
				}
			}
		}
	}
	//cout<<"Terminou"<<endl;
	for (int destino=0; destino<g->getQuantVertices(); destino++){
		if (destino!=origem){
			vector<Aresta *> s;
			Informacao sol = {s, 0};
			vector<Aresta *>::iterator it =  sol.caminho.begin();
			int dest = destino;
			int antecessor;
			do{
				antecessor = rot[dest];
				Aresta *a = g->getArestas(g->isAdjacente(antecessor, dest));
				dest = antecessor;
				sol.caminho.insert(it, a);
				sol.custo+=a->getPeso();
				it =  sol.caminho.begin();
			}while (antecessor!=origem);
			if (result.size()<maximo)
				result.push_back(sol);
			else break;
		}

	}

	//return result;
}
