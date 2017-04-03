#include <map>
#include "Aresta.h"
using namespace std;
 
class Vertice{
	private: 
		int id; /*identificador do v�rtice*/
		map<int, Aresta*> adjacentes; /*� um vetor (de ponteiros) de arestas que saem do v�rtice*/
		
	public:
		Vertice (int id2); 
		
		Vertice();
		
		int getId();
		
		void adicionaAresta(Aresta *a);
		
		Aresta* getAresta(int i);
		
		int getGrau();
		
};
