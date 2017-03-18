#ifndef _ARESTA_H_
#define _ARESTA_H_
//using namespace std;

class Aresta{ //OK
	private:
		int origem, destino; /*se o grafo for não direcionado, não há diferença prática entre os vértices a origem e o destino*/
		int id; /*identificador da aresta*/
		float peso; /*pesos da aresta*/
		
	public:
		Aresta(int id2, int origem1, int destino1, float peso_1); 
		Aresta();	
		int getId();
		int getOrigem();
		int getDestino();
		float getPeso();
		void setId(int novo);
		void setOrigem(int n);
		void setDestino(int n);
};

#endif

