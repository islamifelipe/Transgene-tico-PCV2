#ifndef _CONJUNTO_H_
#define _CONJUNTO_H_


/*Heurística para a manipulação de conjuntos. Esta implementação é importante que se saber, por exemplo, se uma aresta que é adcionada gera um grafo cíclico*/
class Conjunto {
	private:
		int n;
		int *pai;
		int *ordem;
	
	public:
		void makeSet(int x);
		int find_set(int x);
		/*Ao final da função find_set, o nó x bem como todos os outros nós que constituem o caminho de localização (caminho percorrido de x até a raiz do conjunto/árvore)
		 * possuirão o mesmo pai, a saber, a raiz do conjunto/árvore*/
		void link(int x, int y);/*A união sempre pegará a raiz de "menor altura" e a colocará como subárovre filha da de "maior altura" */
		
		Conjunto(int n);
		void union1(int x, int y);
	
		/*A raiz do conjunto o identifica; como a função find_set faz x ter como pai a raiz, e retorna este elemento, basta verificar se o valor
		 * retornando para a chamada com x e y são iguais.*/
		bool compare(int x, int y);
		 void desaloca();

		// int* getPai();
		// int* getOrdem();
		// int getN();

		// Conjunto& operator=( Conjunto& d);
};
#endif