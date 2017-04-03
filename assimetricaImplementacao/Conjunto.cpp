/*Heurística para a manipulação de conjuntos. Esta implementação é importante que se saber, por exemplo, se uma aresta que é adcionada gera um grafo cíclico*/


#include "Conjunto.h"

using namespace std;

Conjunto::Conjunto(int n1){
	n = n1;
	pai = new int[n+1];
	ordem = new int[n+1];
	for (int i=0; i<=n; i++){
		pai[i] = i;
		ordem[i] = 0;
	}
}
	
void Conjunto::makeSet(int x){
	pai[x] = x;
	ordem[x] = 0;
}
	
/*Ao final da desta função, o nó x bem como todos os outros nós que constituem o caminho de localização (caminho percorrido de x até a raiz do conjunto/árvore)
* possuirão o mesmo pai, a saber, a raiz do conjunto/árvore*/
int Conjunto::find_set(int x){
	if (x!=pai[x]){
		pai[x] = find_set(pai[x]);
	}
	return pai[x];
}
	
/*A união sempre pegará a raiz de "menor altura" e a colocará como subárovre filha da de "maior altura" */
void Conjunto::link(int x, int y){
	if (ordem[x]>ordem[y]){  /*Note que caso aa ordens sejam diferentes, não há necessário de incremento da mesmas, pois a união 
	funcionaria como uma inserção de um nó numa árvore não-cheia, isto é, quando a inserção de um nó não aumenta a altura da árvore */
		pai[y] = x;
	}else{
		pai[x] = y;
		if (ordem[x]==ordem[y]){ 
			ordem[y] = ordem[y]+1;
		}
	}		
} /* O vetor ordem é importante para controlar a altura da subárvore e assim poder construir um estrutura de menor altura possível. Isso resulta 
		em um número menor de comparações.*/
	
void Conjunto::union1(int x, int y){
	link(find_set(x), find_set(y));
}
	
/*A raiz do conjunto o identifica; como a função find_set faz x ter como pai a raiz, e retorna este elemento, basta verificar se o valor
 * retornando para a chamada com x e y são iguais.*/
bool Conjunto::compare(int x, int y){
	return find_set(x)==find_set(y);
}
void Conjunto::desaloca(){
	delete[] pai;
	delete[] ordem;
}


// int* Conjunto::getPai(){
// 	return pai;
// }
// int* Conjunto::getOrdem(){
// 	return ordem;
// }
// int Conjunto::getN(){
// 	return n;
// }

// Conjunto& Conjunto::operator=( Conjunto& d){
	
// 	if (this == &d) return *this;
// 	n = d.getN();
// 	pai = new int[n+1];
// 	ordem = new int[n+1];
// 	for (int i=0; i<=n; i++){

// 		pai[i] = (d.getPai())[i];
// 		ordem[i] = (d.getOrdem())[i];
// 	} 

// 	return *this;
// }

