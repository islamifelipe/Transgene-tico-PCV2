#include <map>
#include "Aresta.h"
using namespace std;
 
class Vertice{
	private: 
		int id; /*identificador do vértice*/
		map<int, Aresta*> adjacentes; /*é um vetor (de ponteiros) de arestas que saem do vértice*/
		
	public:
		Vertice (int id2); 
		
		Vertice();
		
		int getId();
		
		void adicionaAresta(Aresta *a);
		
		Aresta* getAresta(int i);
		
		int getGrau();
		
};
