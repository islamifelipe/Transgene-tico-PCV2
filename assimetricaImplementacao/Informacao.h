#ifndef _INFORMACAO_H_
#define _INFORMACAO_H_

/*Esse struct representa um caminho Hamiltoniano
É utilizado para representar todos os endossimbiontes e as informacoes dos hospedeiros */
typedef struct{ 
	vector <Aresta *> caminho;
	float custo;
//	bool isHamilton; // true se a estrutura representa um ciclo Hamiltoniano. False, se representa uma arvore ou um caminho simples
} Informacao;


typedef struct{
	vector< Informacao > arvores;
	vector< Informacao > caminhos;
	//vector< Informacao > cicloHamiltoniano;
	vector< Informacao > chaineSimplex;
} Hospedeiro;


typedef struct{
	vector <Aresta *> cadeia;
} Pasmideo;

/* TA BOM! EU SEI QUE PODERIA IMPLEMENTAR COM CLASSE E HERANÇA; FAREI ISSO EM BREVE
*/

#endif