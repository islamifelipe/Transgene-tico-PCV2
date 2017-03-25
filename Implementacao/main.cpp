/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements Transgenic algorithm for the traveling salesman problem 
#=======================================================================
*/
//TODO : ver como vai funcionar pra instâncias grandes (LIMITA o tamanho do hospedeiro)
#include <iostream>
#include <map> 
#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
#include "margeSort.h"
#include <sys/times.h>
#include <unistd.h>
#include  "allMST.h"
#include <cstdlib>
#include <ctime>
#include <climits>
#include "dijkstra.h"
#include "Informacao.h"
#include <queue>          // std::queue
#include "transplex.h"
using namespace std;
 
/* 

vetores transgenéticos
*/

/*
** Endossimbiontes iniciais:
	OK --> Vizinho mais próximo (solucao completa)
** Hospedeiro inicial
 	OK --> Arvores Minimas
	OK --> Caminhos minimos (todos)
	OK --> resultado do simplex relaxado
** Pasmideo simples: 
	OK --> Seleciona uma arvore ou um caminho simples com probabilidade iguais, seleciona um alelo e monta um plasmideo de tamanho definido (O plasmideo deve ser um caminho)
	OK --> seleciona um caminho hamiltoniano com pesos de probabilidade, seleciona um alelo e monta um plasmideo de tamanho definido
	OK --> seleciona um alelo randômico do material genético do simplex, define randomicamente o tamanho do plasmideo. Gera o plasmideo com um busca em largura, sempre pegando as arestas de menor custo

Os transpossons a seguir serao aplicados da seguinte forma:
	caso um endossimbionte tenha resistido a dois ataques SEGUIDOS de plasmideos, entao sorteia-se um dos transpossns

** Transposson:
	OK --> Busca com 2-OPT (retorna o melhor vizinho). O ataque deste transposson consiste em verificar se o melhor vizinho é melhor que o endossimbionte atacado. Se sim, substitui. Se nao, deixa.
** TransPlex (Transposon que utiliza o simplex)
	--> O transposson toma um endossimbionte, e aplica o simplex relaxado (binarios->continuos) e limitado (numero de iteracoes) sobre o mesmo, a fim de melhorá-lo
	O intervalo do transposson é toda a cadeia do endossimbionte
*/

/*PARAMETROS PASSADOS NA ENTRADA*/

int quantPlasFromTree; // Quantidade de plasmideos simples formados por parte da cadeia genética de arvores 
int quantPlasFromPath; //  Quantidade de plasmideos simples formados por parte da cadeia genética de caminhos simples 
int quantPlasFromCicle; //  Quantidade de plasmideos simples formados por parte da cadeia genética de ciclos hamiltonianos 
int quantPlasFromCadeiaSimplex; // Quantidade de plasmideos simples formados por parte da cadeia genética oriunda do simplex
int iteracoes; // quantidade de iteracoes do transgenético
int quantDNAArvores = 5; // Quantidade de AGM armazenadas no hospedeiro
int quantDNACaminhos = 295; // Quantidade de caminhos curtos simples armazenadas no hospedeiro
int quantDNACiclo = 60; // Quantidade de ciclos hamiltonianos para o conjunto dos endossimbiontes
int sizeTranspossonIntervalo = 50; //trans2opt = a busca na vizinhança deve ser feita entre os alelos init e end
//sizeTranspossonIntervalo é o tamanho maximo do intervalo de busca do transposson. O trecho vai de (init, sizeTranspossonIntervalo), onde init é randômico
int n;
struct tms tempsInit, initArvores, tempsFinal1,tempsFinal2 ; // para medir o tempo

/*Usada para preencher o a populacao inciial de 
endossibiontes com solucoes puramente aleatorias
Retorna n caminhos. Onde n é o numero de vertices do grafo;
Cada caminho começa num vertice i 
O segundo item pair (int) é pra contar a quantidade de vezes que o endossimbionte foi atacado sem sucesso*/
vector< pair <Informacao, int> > vizinhoMaisProximo(Grafo *g, int maximo){// O(n^3)
	vector< pair <Informacao, int > > resul;
	int visitado[g->getQuantVertices()];
	int vez; // usado para o vertice da vez
	int cont=0;
	for (int i=0; i<g->getQuantVertices(); i++){ // para cada vertice i, gera uma solucao gulosa (Vizinho mais proximo) cujo caminho deve iniciar no vertice i
		for (int j=0; j<g->getQuantVertices(); j++) visitado[j] = 0;
		vector<Aresta *> s;
		Informacao sol = {s, 0};
		visitado[i] = 1;
		vez = i;
		cont=1;
		do {
			float min = INT_MAX;
			int ares_min = -1;
			int ver_des  = -1;
			for (int j=0; j<g->getQuantVertices(); j++){
				if (g->isAdjacente(vez, j)!=-1 && visitado[j]==0){
					if (g->getArestas(g->isAdjacente(vez, j))->getPeso()<min){
						min = g->getArestas(g->isAdjacente(vez, j))->getPeso();
						ares_min = g->isAdjacente(vez, j); // id
						ver_des = j;
					}
				}
			}
			//escolhido o proximo vertice
			if (ares_min == -1){ // fim
				cont=g->getQuantVertices();
			} else { // continua
				vez = ver_des;
				visitado[vez] = 1;
				sol.caminho.push_back(g->getArestas(ares_min));
				sol.custo+=g->getArestas(ares_min)->getPeso();
				cont++;
			}
		}while (cont!=g->getQuantVertices());
		// Atingimos todos os vertices. Pega o vertice da vez, e liga ao vertice inicial
		int idAresta = g->isAdjacente(vez, i);
		if (sol.caminho.size() == g->getQuantVertices()-1 && idAresta!=-1){ //se for igual a -1, entao o ultimo vertice nao é ligado ao primeiro. Aborta a solucao.
			Aresta *a = g->getArestas(idAresta);
			sol.caminho.push_back(a);
			sol.custo +=a->getPeso();
			if (resul.size()<maximo){
				resul.push_back(make_pair(sol,0));
			} else break;
		}

	}
	return resul;
}


// vector< Informacao >randomEndosibitontes(Grafo *g){ // O(n^3)
// 	vector< Informacao > resul;
// 	short v[g->getQuantVertices()];
// 	int vez; // usado para o vertice da vez
// 	int cont=0; // contar quantos vertices ja passaram
// 	for (int i=0; i<g->getQuantVertices(); i++){ // para cada vertice i, gera uma solucao aleatoria cujo caminho deve iniciar no vertice i
		
// 		for (int j=0; j<g->getQuantVertices(); j++) v[j] = 0;
// 		v[i] = 1;
// 		cont=1;
// 		vez = i;
// 		vector<Aresta *> s;
// 		Informacao solucao = {s, 0};
// 		//sorteia-se uma aresta adjacente ao vertice da vez, cujo vertice extremo nao tenha ainda sido utilizado
// 		do{
// 			if (cont!=g->getQuantVertices()){
// 				vector<int> apto;//contem os vertices aptos a serem sorteados
// 				for (int j=0; j<g->getQuantVertices(); j++){
// 					if (g->isAdjacente(vez, j)!=-1 && v[j]==0) apto.push_back(j);
// 				}
// 				//apto pode ser vazio: situacao de quando todo vertice j ligado ao vertice vez ja tiver sido utilizado
// 				if (apto.size()!=0){
// 					int b = apto[rand()%(apto.size()+1-1)]; // b = id do vertice sortado
// 					int idAresta = g->isAdjacente(vez, b);
// 					Aresta *a = g->getArestas(idAresta);
// 					solucao.caminho.push_back(a);
// 					solucao.custo+=a->getPeso();
// 					cont++;
// 					//cout<<vez<<", "<<b<<endl;
// 					vez = b;
// 					v[b] = 1;
// 				} else {cont=g->getQuantVertices();}
// 			}
// 		}while(cont!=g->getQuantVertices());
// 		// Atingimos todos os vertices. Pega o vertice da vez, e liga ao vertice inicial
// 		int idAresta = g->isAdjacente(vez, i);
// 		if (solucao.caminho.size() == g->getQuantVertices()-1 && idAresta!=-1){
// 			Aresta *a = g->getArestas(idAresta);
// 			solucao.caminho.push_back(a);
// 			solucao.custo+=a->getPeso();
// 			resul.push_back(solucao);
// 		} // se for igual a -1, entao o ultimo vertice nao é ligado ao primeiro. Aborta a solucao.
// 	}
// 	return resul;
// }


Hospedeiro initHospedeiro(Grafo *g){
	cout<<"Iniciando hospedeiro ... "<<endl;
	Aresta ** arestasPtr= g->getAllArestasPtr();
	times(&initArvores); 
	vector< Informacao > arvores = AllSpaningTree(g, arestasPtr,quantDNAArvores);
	
	times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time = (tempsFinal2.tms_utime - initArvores.tms_utime);
	float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
	cout<<arvores.size()<<" arvores computadas no hospedeiro"<<endl;
	cout<<"Tempo parcial em segundos: "<<sec<<endl;
	//vector< Informacao > cicloHamiltoniano; //= vizinhoMaisProximo(g,quantDNACiclo);
	//cout<<cicloHamiltoniano.size()<<" ciclos hamiltonianos (vizinho mais proximo) no hospedeiro"<<endl;
	vector< Informacao > caminhos;
	for (int c=0; c<g->getQuantVertices() && caminhos.size()<quantDNACaminhos; c++){ // caminho na origem c
		dijkstra(g,c,caminhos,quantDNACaminhos);
	}
	cout<<caminhos.size()<<" caminhos curtos no hospedeiro"<<endl;
	Informacao ret = simplexrelaxado(g);
	Hospedeiro resul = {arvores,caminhos, ret};
	cout<<"Uma cadeia de DNA gerada pelo simplex relaxado"<<endl;
	return resul;
}



void constroiPlasmideosFromCadeiaSimplex(Grafo *g, vector <Pasmideo> &plasmideosSimples, Informacao chaine){
	for (int i=0; i<quantPlasFromCadeiaSimplex; i++){
		vector<Aresta*> sol;
		queue <int> fila_de_vertices;
		int visitado[g->getQuantVertices()];
		vector<int> amostral;
		for (int jj = 0; jj<chaine.caminho.size(); jj++){
			visitado[chaine.caminho[jj]->getOrigem()] = 0;
			visitado[chaine.caminho[jj]->getDestino()] = 0;
			// antecessor[chaine.caminho[jj]->getOrigem()] = -1;
			// antecessor[chaine.caminho[jj]->getDestino()] = -1;
			amostral.push_back(chaine.caminho[jj]->getOrigem());
			amostral.push_back(chaine.caminho[jj]->getDestino());
		}
		int origem = amostral[rand()%amostral.size()];
		int max_size = 6; // comprimento maximo do plasmideo
		visitado[origem] = 1;
		while (max_size>0){
			float min = INT_MAX;
			int idA;
			int prox;
			for (int jj = 0; jj<chaine.caminho.size(); jj++){
				if ((chaine.caminho[jj]->getDestino()==origem && visitado[chaine.caminho[jj]->getOrigem()]==0)||(chaine.caminho[jj]->getOrigem()==origem && visitado[chaine.caminho[jj]->getDestino()]==0)){
					if (chaine.caminho[jj]->getPeso()<min){
						min = chaine.caminho[jj]->getPeso();
						idA = chaine.caminho[jj]->getId();
						if (chaine.caminho[jj]->getDestino()==origem) prox = chaine.caminho[jj]->getOrigem();
						else if (chaine.caminho[jj]->getOrigem()==origem) prox = chaine.caminho[jj]->getDestino();
					}
				}
			}
			if (min!=INT_MAX){
				visitado[prox] = 1;
				sol.push_back(g->getArestas(idA));
				origem = prox;
				max_size--;
			} else {
				max_size = 0;
			}
		}
		if (sol.size()>0) {
			plasmideosSimples.push_back((Pasmideo){sol});
		}
	}

}
/*	Esta funcao cronstroi quantPlasFromTree plamideos simples a partir das arvores
	Chamamos plasmideos simples as cadeias de genes oriundos de pedaços de arvores (ou de caminhos - ver funcao constroiPlasmideosFromPath)
	arvores sao sorteadas com igual probabilidade
	O plasmideo retornado deve ser um caminho 
*/
void constroiPlasmideosFromTree(Grafo *g, vector <Pasmideo> &plasmideosSimples, vector< Informacao > trees){

	for (int i=0; i<quantPlasFromTree; i++){

		queue <int> fila_de_vertices;
		int antecessor[g->getQuantVertices()];
		int visitado[g->getQuantVertices()];
		for (int i=0; i<g->getQuantVertices(); i++) {
			visitado[i] = 0;
			antecessor[i] = -1;
		}
		int id = rand()%trees.size();
		Informacao tree = trees[id];
		int origem = rand()%g->getQuantVertices();
		int destino = rand()%g->getQuantVertices();
		// origem e destino devem ser diferentes
		if (origem==destino){
			i--;
			continue;
		}
		fila_de_vertices.push(origem);
		while (fila_de_vertices.size()>0){
			int orig =  fila_de_vertices.front();
			fila_de_vertices.pop();
			if (orig!=destino){
				for (int jj = 0; jj<tree.caminho.size(); jj++){
					if (tree.caminho[jj]->getOrigem()==orig && visitado[tree.caminho[jj]->getDestino()]==0){
						antecessor[tree.caminho[jj]->getDestino()] = orig;
						fila_de_vertices.push(tree.caminho[jj]->getDestino());
						visitado[tree.caminho[jj]->getDestino()] = 1;
					} else if (tree.caminho[jj]->getDestino()==orig && visitado[tree.caminho[jj]->getOrigem()]==0){
						antecessor[tree.caminho[jj]->getOrigem()] = orig;
						fila_de_vertices.push(tree.caminho[jj]->getOrigem());
						visitado[tree.caminho[jj]->getOrigem()] = 1;
					}
				}
			} else break;
		}
		int dest = destino;
		vector<Aresta*> sol;
		while (dest != origem){
			sol.push_back(g->getArestas(g->isAdjacente(dest, antecessor[dest])));
			dest = antecessor[dest];
			//cout<<"aqui3"<<endl;
		}
		plasmideosSimples.push_back((Pasmideo){sol});
		//cout<<"Size Arvore = "<<sol.size()<<endl;
	}

}


/*	Esta funcao cronstroi quantPlasFromPath plamideos simples a partir dos caminhos simples
	Chamamos plasmideos simples as cadeias de genes oriundos de pedaços de caminhos (ou de arvores - ver funcao constroiPlasmideosFromTree)
	caminhos sao sorteados com igual probabilidade
	A quantidade de genes do plasmideo obtido de caminhos é um valor aleatorio entre d/3,...2*d/3, onde d é o comprimento do caminho
*/
void constroiPlasmideosFromPath(vector <Pasmideo> &plasmideosSimples, vector< Informacao > caminhos){
	
	for (int i=0; i<quantPlasFromPath; i++){
		int c = rand()%caminhos.size();
		Informacao path = caminhos[c];

		int size = path.caminho.size();
		if (size>=3){
			int low = (int)(path.caminho.size()/3);
			int high = (int)(2*path.caminho.size()/3);
			size = rand() % (high - low + 1) + low; // TAMANHO DO PLASMIDEO
		}

		int gene = rand()%(path.caminho.size()-size+1);
		vector <Aresta*> plasm;
		for (int j=gene; j<size+gene; j++){
			plasm.push_back(path.caminho[j]);
		}
		plasmideosSimples.push_back((Pasmideo){plasm}); 
		//cout<<"Size coaminho = "<<plasm.size()<<endl;
	}
}

// /*	Esta funcao cronstroi quantPlasFromCicle plamideos simples a partir de ciclos hamiltonianos do hospedeiro
// 	Chamamos plasmideos simples as cadeias de genes oriundos de ciclos hamiltonianos
// 	Os ciclos do hospedeiro sao selecionados com probabilidade inversamente propocional ao custo do ciclo (menor custo, mais chances de ser sorteados)
// 	A quantidade de genes do plasmideo obtido de ciclos é um valor aleatorio entre n/3,...,2*n/3, onde n é a quantidade de vertices do grafo
// */
// void constroiPlasmideosFromHamiltoniano(vector <Pasmideo> &plasmideosSimples, vector< Informacao > ciclos){
// 	float sum = 0;
// 	int low = (int)(n/3);
// 	int high = (int)(2*n/3);
// 	int size = rand() % (high - low + 1) + low; // TAMANHO DO PLASMIDEO

// 	for (int i=0; i<ciclos.size();i++) sum+=(1/ciclos[i].custo);
// 	int probabilidade[100]; // guarda id's dos ciclos
// 	int cont=0;
// 	for (int i=0; i<ciclos.size();i++){
// 		float quant = ((1/ciclos[i].custo)*100)/sum;
// 		for (int j=0; j<quant && cont<100; j++) probabilidade[cont++] = i;
// 	}

// 	for (int i=0; i<quantPlasFromCicle; i++){
// 		int c = rand()%cont;
// 		int id_ciclo = probabilidade[c];
// 		Informacao ciclo = ciclos[id_ciclo];
// 		int gene = rand()%(ciclo.caminho.size()-size+1);
// 		vector <Aresta*> plasm;
// 		for (int j=gene; j<size+gene; j++){
// 			plasm.push_back(ciclo.caminho[j]);
// 		}
// 		plasmideosSimples.push_back((Pasmideo){plasm});
// 	}
	
// }

/*Esta funcao recebe um plasmideo e um cromossomo. Retorna verdadeiro se o ataque foi bem sucessido. Falso, caso contrario
O plasmideo representa um "mini-caminho" (i.e, cada vertice deve ter cerdidade 1 (pontas) ou 2 (interior)) 
O plasmideo é transquito por inteiro no novo cromossomo. 
Arestas do cromossomo inicial serao inseridas no novo compromosso, respeitando a propriedade de que as pontas tenham grau 1 e o interior grau 2
No final, acrescenta-se no novo cromossomo a aresta pra fechar o ciclo */
bool manipula(Grafo *g, Pasmideo plasm, Informacao cromossomo, Informacao &resultado){
	int visitado[g->getQuantVertices()];
	resultado.custo = 0;
	for (int i=0; i<g->getQuantVertices();i++)visitado[i] = 0; 
	for (int i=0; i<plasm.cadeia.size(); i++){
		visitado[plasm.cadeia[i]->getOrigem()]+=1;
		visitado[plasm.cadeia[i]->getDestino()]+=1;
		resultado.custo+=plasm.cadeia[i]->getPeso();
		resultado.caminho.push_back(plasm.cadeia[i]);
	}	
	vector<Aresta *>::iterator it;;
	//cout<<visitado[0]<<" "<<visitado[1]<<" "<<visitado[2]<<" "<<visitado[3]<<" "<<visitado[4]<<visitado[5]<<endl;
			
	int size = resultado.caminho.size();
	int fim, init;
	if (size!=1){
		fim = visitado[resultado.caminho[size - 1]->getDestino()]==1?resultado.caminho[size - 1]->getDestino():resultado.caminho[size - 1]->getOrigem();
		init = visitado[resultado.caminho[0]->getDestino()]==1?resultado.caminho[0]->getDestino():resultado.caminho[0]->getOrigem();
	} else {
		fim = resultado.caminho[size - 1]->getDestino();
		init= resultado.caminho[size - 1]->getOrigem();
	}
	//cout<<"init = "<<init<<" fim = "<<fim<<endl;
	bool isAdded;
	do{
		isAdded = false;
		for (int i=0; i<cromossomo.caminho.size(); i++){ // para cada gene do cromossomo, verifica-se se ele pode ser inserido no fim ou no inicio
			//cout<<cromossomo.caminho[i]->getOrigem()<<", "<<cromossomo.caminho[i]->getDestino()<<endl;
			if (visitado[cromossomo.caminho[i]->getOrigem()]!=2 && visitado[cromossomo.caminho[i]->getDestino()]!=2 && visitado[cromossomo.caminho[i]->getOrigem()]!=visitado[cromossomo.caminho[i]->getDestino()]){
				if (cromossomo.caminho[i]->getOrigem()==fim || cromossomo.caminho[i]->getDestino()==fim){
					resultado.caminho.push_back(cromossomo.caminho[i]);
					resultado.custo +=cromossomo.caminho[i]->getPeso();
					visitado[cromossomo.caminho[i]->getOrigem()]+=1;
					visitado[cromossomo.caminho[i]->getDestino()]+=1;
					fim = cromossomo.caminho[i]->getOrigem()!=fim?cromossomo.caminho[i]->getOrigem():cromossomo.caminho[i]->getDestino();
					size++;
					isAdded = true;
					//cout<<"AQUI1"<<endl;
					//cout<<visitado[0]<<" "<<visitado[1]<<" "<<visitado[2]<<" "<<visitado[3]<<" "<<visitado[4]<<visitado[5]<<endl;
				} else if (cromossomo.caminho[i]->getOrigem()==init || cromossomo.caminho[i]->getDestino()==init){
					it =  resultado.caminho.begin();
					resultado.caminho.insert(it, cromossomo.caminho[i]);
					resultado.custo +=cromossomo.caminho[i]->getPeso();
					visitado[cromossomo.caminho[i]->getOrigem()]+=1;
					visitado[cromossomo.caminho[i]->getDestino()]+=1;
					init = cromossomo.caminho[i]->getOrigem()!=init?cromossomo.caminho[i]->getOrigem():cromossomo.caminho[i]->getDestino();
					size++;
					isAdded = true;
					//cout<<"AQUI2"<<endl;
					//cout<<visitado[0]<<" "<<visitado[1]<<" "<<visitado[2]<<" "<<visitado[3]<<" "<<visitado[4]<<visitado[5]<<endl;
				}
				//cout<<"init = "<<init<<" fim = "<<fim<<endl;
			}
		}
	}while (isAdded == true); // O(n^2)
	if (size==cromossomo.caminho.size()-1){
		fim = visitado[resultado.caminho[size - 1]->getDestino()]==1?resultado.caminho[size - 1]->getDestino():resultado.caminho[size - 1]->getOrigem();
		init = visitado[resultado.caminho[0]->getDestino()]==1?resultado.caminho[0]->getDestino():resultado.caminho[0]->getOrigem();
		if (g->isAdjacente(fim,init)!=-1){
			int id = g->isAdjacente(fim,init);
			resultado.caminho.push_back(g->getArestas(id));
			resultado.custo +=g->getArestas(id)->getPeso();
			if (resultado.custo<cromossomo.custo) return true;
			else {/*cout<<"false1\n";*/return false;}
		} else {/*cout<<"false2\n";*/return false;}
	} else { 
		// se chegar aqui, é porque nao conseguiu formar um caminho usando os genes do plasmideo e do cromossomo manipulado. 
		//Inserimos, pois, arestas aleatorias até formar o caminho. Se mesmo assim nao formar uma solucao valida, entao paramos
		///*cout<<"false4\n";*/return false;
		do{
			isAdded = false;
			for (int iiii=0; iiii<g->getQuantVertices(); iiii++){
				if (visitado[iiii]==0){
					int min = INT_MAX;
					int id_min = -1;
					bool no_init;
					if (g->isAdjacente(iiii, init)!=-1) {
						id_min = g->isAdjacente(iiii, init);
						min = g->getArestas(id_min)->getPeso();
						no_init = true;
					}
					if (g->isAdjacente(iiii, fim)!=-1){
						if (g->getArestas(g->isAdjacente(iiii, fim))->getPeso()<min){
							id_min = g->isAdjacente(iiii, fim);
							min = g->getArestas(id_min)->getPeso();
							no_init = false;
						}
					}
					if (id_min!=-1){
						if (no_init == false){
							resultado.caminho.push_back(g->getArestas(id_min));
							fim = iiii;
						} else {
							it =  resultado.caminho.begin();
							resultado.caminho.insert(it, g->getArestas(id_min));
							init = iiii;
						}
						resultado.custo +=g->getArestas(id_min)->getPeso();
						visitado[g->getArestas(id_min)->getOrigem()]+=1;
						visitado[g->getArestas(id_min)->getDestino()]+=1;
						size++;
						isAdded = true;
					}
				}
			}
		}while (isAdded == true);
		if (size==cromossomo.caminho.size()-1){
			if (g->isAdjacente(fim,init)!=-1){
				int id = g->isAdjacente(fim,init);
				resultado.caminho.push_back(g->getArestas(id));
				resultado.custo +=g->getArestas(id)->getPeso();
				if (resultado.custo<cromossomo.custo) return true;
				else {/*cout<<"false4\n";*/return false;}
			} else {/*cout<<"false5\n";*/return false;}
		} else  {/*cout<<"false6\n"*;*/return false;}
	}
}
/*	transponsson retorna um melhoramento 2-opt para o endossimbionte
	Caso nenhum vizinho 2-opt seja melhor que ele, o proprio endossimbionte da entrada é retornado
	NOTE QUE O ATAQUE DO TRANSPOSSON ESTÁ IMPLÍCITO
*/
void trans2opt(Grafo *g, Informacao &re_endossibionte){
	vector<int> representacao;
	int initIntervalorTransposson = rand()%(re_endossibionte.caminho.size()-sizeTranspossonIntervalo+1); // 0 1 2 3 4 5 6 7 8 9 10 11 
	// cout<<initIntervalorTransposson<<endl;
	// cout<<sizeTranspossonIntervalo+initIntervalorTransposson<<endl;
	//sizeTranspossonIntervalo = 50; // retirar;
	
	int init = re_endossibionte.caminho[0]->getOrigem();
	if (init == re_endossibionte.caminho[1]->getOrigem() || init == re_endossibionte.caminho[1]->getDestino()){
		init = re_endossibionte.caminho[0]->getDestino();
	}
	representacao.push_back(init);
	int previuos =  init;
	//cout<<init<<" ";
	for (int i=0; i<re_endossibionte.caminho.size(); i++){ //O(n)
		if (re_endossibionte.caminho[i]->getOrigem()!=previuos){
				//cout<<re_endossibionte.caminho[i]->getOrigem()<<" ";
			representacao.push_back(re_endossibionte.caminho[i]->getOrigem());
			previuos = re_endossibionte.caminho[i]->getOrigem();
		} else if (re_endossibionte.caminho[i]->getDestino()!=previuos){
			//cout<<re_endossibionte.caminho[i]->getDestino()<<" ";
			representacao.push_back(re_endossibionte.caminho[i]->getDestino());
			previuos = re_endossibionte.caminho[i]->getDestino();
		}
	}
	//cout<<endl;
	float minCusto = re_endossibionte.custo;
	bool melhoramento = true;
	while (melhoramento == true){
		melhoramento = false;
		for (int i=initIntervalorTransposson+1; i<sizeTranspossonIntervalo+initIntervalorTransposson-2 && melhoramento==false; i++){ // intervalo na representacao (nao dos vertices) O(n^2)
			for (int j=i+1; j<sizeTranspossonIntervalo+initIntervalorTransposson-1; j++){
				if (j==i+1){
					/* troca (i-1,i) por (i-1, j)  
					 * troca (j,j+1) por (i, j+1) 
					 * troca (i,j) por (j,i) */
					 if ((g->isAdjacente(representacao[i-1],representacao[j])!=-1) && (g->isAdjacente(representacao[i],representacao[j+1])!=-1) && (g->isAdjacente(representacao[j],representacao[i])!=-1)){ // se as novas arestas existem
						float novo = minCusto - g->getArestas(g->isAdjacente(representacao[i-1],representacao[i]))->getPeso() - g->getArestas(g->isAdjacente(representacao[j],representacao[j+1]))->getPeso() - g->getArestas(g->isAdjacente(representacao[i],representacao[j]))->getPeso();
						float novoadd = novo + g->getArestas(g->isAdjacente(representacao[i-1],representacao[j]))->getPeso() + g->getArestas(g->isAdjacente(representacao[i],representacao[j+1]))->getPeso() +g->getArestas(g->isAdjacente(representacao[j],representacao[i]))->getPeso();
						if (novoadd<minCusto){
							minCusto = novoadd;
							int aux = representacao[i];
							representacao[i] = representacao[j];
							representacao[j] = aux;
							melhoramento = true;
							break;
						}
					}
				 }else{
				 	/* troca (i-1,i) por (i-1, j) 
					 * troca (i, i+1) por (j, i+1) 
					 * troca (j-1,j) por (j-1, i) 
					 * troca (j,j+1) por (i, j+1) 
					 * portanto, as novas arestas devem existir
					 */
					if ((g->isAdjacente(representacao[i-1],representacao[j])!=-1) && (g->isAdjacente(representacao[j],representacao[i+1])!=-1) && (g->isAdjacente(representacao[j-1],representacao[i])!=-1) && (g->isAdjacente(representacao[i],representacao[j+1])!=-1)){ // se as novas arestas existem
						float novo = minCusto - g->getArestas(g->isAdjacente(representacao[i-1],representacao[i]))->getPeso() - g->getArestas(g->isAdjacente(representacao[i],representacao[i+1]))->getPeso() - g->getArestas(g->isAdjacente(representacao[j-1],representacao[j]))->getPeso() - g->getArestas(g->isAdjacente(representacao[j],representacao[j+1]))->getPeso();
						float novoadd = novo + g->getArestas(g->isAdjacente(representacao[i-1],representacao[j]))->getPeso() + g->getArestas(g->isAdjacente(representacao[j],representacao[i+1]))->getPeso() +g->getArestas(g->isAdjacente(representacao[j-1],representacao[i]))->getPeso()+g->getArestas(g->isAdjacente(representacao[i],representacao[j+1]))->getPeso();
						if (novoadd<minCusto){
							minCusto = novoadd;
							int aux = representacao[i];
							representacao[i] = representacao[j];
							representacao[j] = aux;
							melhoramento = true;
							break;
						}
					}
				}
			}
		}
	}
	int cont=0;
	float custo = 0;
	int i =0;
	//cout<<representacao.size()<<endl;
	for (; i< g->getQuantVertices(); i++){ //O(n)
		re_endossibionte.caminho[cont++] = g->getArestas(g->isAdjacente(representacao[i], representacao[i+1]));
		custo +=g->getArestas(g->isAdjacente(representacao[i], representacao[i+1]))->getPeso();
		//cout<<representacao[i]<<", "<<representacao[i+1]<<endl;
	}
	re_endossibionte.custo = custo;
	

}
// /*retorna um subconjunto de endossimbiontes (com iguais probabilidades)
// o "int" do "pair" é o id do cromossomo no vetor de endossimbiontes origial*/
// vector < pair<Informacao, int > > slecionaCromossomos(vector <Informacao> endossibiontes){
// 	vector < pair<Informacao, int > > ret;
// 	for (int i=0; i<endossibiontes.size(); i++){
// 		int d = 1;//rand()%2;
// 		if (d==1) ret.push_back(make_pair(endossibiontes[i], i));
// 	}
// 	return ret;
// }

void printEndossimbiontes(vector < pair <Informacao, int> > endossibiontes){
	for (int i=0; i<endossibiontes.size(); i++){
		cout<<"Endossibionte "<<i<<" (" << endossibiontes[i].first.custo <<") : ";
		for (int j=0; j<endossibiontes[i].first.caminho.size(); j++){
			cout<<"("<<endossibiontes[i].first.caminho[j]->getOrigem()<< ","<<endossibiontes[i].first.caminho[j]->getDestino()<<") ";
		}
		cout<<endl;
	}
}

// void atualizadaMaterialGenetico(vector <Informacao> novaPop, vector< Informacao > &cicloHamiltoniano){
// 	for (int i=0; i<novaPop.size(); i++){
// 		for (int id = 0; id<cicloHamiltoniano.size();id++){
// 			if (cicloHamiltoniano[id].custo>novaPop[i].custo){
// 				cicloHamiltoniano[id] = novaPop[i];
// 				break;
// 			}
// 		}
// 	}
// }

void transgenic(Grafo *g){
	Hospedeiro hospedeiro = initHospedeiro(g);
	vector <pair <Informacao, int > > endossibiontes;
	do{
		endossibiontes = vizinhoMaisProximo(g,quantDNACiclo);//randomEndosibitontes(g);
	}while(endossibiontes.size()==0);
	cout<<endossibiontes.size()<<" endossimbiontes computados ... ";
	// printEndossimbiontes(hospedeiro.cicloHamiltoniano);
	// printEndossimbiontes(endossibiontes);
	cout<<"Começa o transgenético ..."<<endl;
	for (int iii = 0; iii<iteracoes; iii++){
		// gera plasmideos
		vector <Pasmideo> plasmideosSimples;
		constroiPlasmideosFromTree(g, plasmideosSimples, hospedeiro.arvores);
		constroiPlasmideosFromPath(plasmideosSimples,hospedeiro.caminhos);
		//constroiPlasmideosFromHamiltoniano(plasmideosSimples,hospedeiro.cicloHamiltoniano);
		constroiPlasmideosFromCadeiaSimplex(g, plasmideosSimples, hospedeiro.chaineSimplex);
		//fim da geracao plasmideos
		//cout<<"AQUI1"<<endl;
		if (plasmideosSimples.size()>0){
			//vector < pair<Informacao, int > > subPop = slecionaCromossomos(endossibiontes);
			vector <Informacao> novaPop;
			//if (subPop.size()>0){
			for (int po = 0; po<endossibiontes.size(); po++){//subPop
				int id_plasm = rand()%(plasmideosSimples.size());
				Informacao resultado;
				//cout<<"Size = "<<plasmideosSimples[id_plasm].cadeia.size()<<endl;
				bool isValido = manipula(g, plasmideosSimples[id_plasm], endossibiontes[po].first, resultado);
				if (isValido==true){
					endossibiontes[po].first = resultado; // substitui no endossimbionte
					novaPop.push_back(resultado);
					endossibiontes[po].second = 0;
					//cout<<"OBA OBA OBA"<<endl;
				} else {
					endossibiontes[po].second +=1;
					if (endossibiontes[po].second==20){
						trans2opt(g, endossibiontes[po].first);
						//cout<<"Transpossons ... "<<endl;
					}
				}
			}
			//atualizadaMaterialGenetico(novaPop,hospedeiro.cicloHamiltoniano);
		//	}
		}
	}
	cout<<"Resultado final"<<endl;
	float min = endossibiontes[0].first.custo;
	int i_min = 0;
	for (int i=1; i<endossibiontes.size(); i++){
		if (endossibiontes[i].first.custo<min){
			min = endossibiontes[i].first.custo;
			i_min = i;
		}
	}
	cout<<"Melhor custo encontrado = "<<min<<endl;

	cout<<"Size: "<<endossibiontes[i_min].first.caminho.size()<<endl;
	cout<<"Solucao : " <<endl;
	for (int j=0; j<endossibiontes[i_min].first.caminho.size(); j++){
		cout<<"("<<endossibiontes[i_min].first.caminho[j]->getOrigem()<< ","<<endossibiontes[i_min].first.caminho[j]->getDestino()<<") ";
	}
	
	cout<<endl;
	// cout<<"Todos os endossinbiontes : "<<endl;
	// printEndossimbiontes(endossibiontes);
}
int main(){
	srand((unsigned)time(0));

	float peso1;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	Grafo my_grafo(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		my_grafo.addAresta(id, origem, destino, peso1);
		id++;
	}
	init(&my_grafo);
	int nA = id; // quantidade de arestas do grafo	
	cout<<"Instância lida com sucesso"<<endl;
	//Hospedeiro resul = initHospedeiro(&my_grafo);
	quantPlasFromPath = 35;
	quantPlasFromTree = 15;
	//quantPlasFromCicle = 12;
	quantPlasFromCadeiaSimplex =30;
	iteracoes = 90;

	times(&tempsInit);  // pega o tempo do clock inical
	//Aresta ** arestasPtr= my_grafo.getAllArestasPtr();
	// vector< Informacao > arvores = AllSpaningTree(&my_grafo, arestasPtr,quantDNAArvores);
	//Hospedeiro hospedeiro = initHospedeiro(&my_grafo);
	transgenic(&my_grafo);

	// Informacao ret = simplexrelaxado(&my_grafo);
	// cout<<"Size = "<<ret.caminho.size()<<endl;
	// cout<<"Custo "<<ret.custo<< " : ";
	// 	for (int j=0; j<ret.caminho.size(); j++){
	// 		cout<<"("<<ret.caminho[j]->getOrigem()<< ","<<ret.caminho[j]->getDestino()<<") ";
	// 	}
	// 	cout<<endl;
	// vector <Pasmideo> plasmideosSimples;
	// constroiPlasmideosFromCadeiaSimplex(&my_grafo, plasmideosSimples, ret);
	// for (int i=0; i<plasmideosSimples.size(); i++){
	// 	cout<<"Pasmideo "<<i<< " : ";
	// 	for (int j=0; j<plasmideosSimples[i].cadeia.size(); j++){
	// 		cout<<"("<<plasmideosSimples[i].cadeia[j]->getOrigem()<< ","<<plasmideosSimples[i].cadeia[j]->getDestino()<<") ";
	// 	}
	// 	cout<<endl;
	// }




 // 	int index = 40; //30
	// vector< pair<Informacao,int> > cicloHamiltoniano  = vizinhoMaisProximo(&my_grafo,quantDNACiclo);;
	// cout<<"Size = "<<cicloHamiltoniano[index].first.caminho.size()<<endl;
	// cout<<"Custo = "<<cicloHamiltoniano[index].first.custo<<endl;
	// for (int j=0; j<cicloHamiltoniano[index].first.caminho.size(); j++){
	// 	cout<<"("<<cicloHamiltoniano[index].first.caminho[j]->getOrigem()<< ","<<cicloHamiltoniano[index].first.caminho[j]->getDestino()<<") ";
	// }
	// cout<<endl;
	// trans2opt(&my_grafo, cicloHamiltoniano[index].first);
	// cout<<"Custo = "<<cicloHamiltoniano[index].first.custo<<endl;
	// for (int j=0; j<cicloHamiltoniano[index].first.caminho.size(); j++){
	// 	cout<<"("<<cicloHamiltoniano[index].first.caminho[j]->getOrigem()<< ","<<cicloHamiltoniano[index].first.caminho[j]->getDestino()<<") ";
	// }
	// cout<<endl;

	// Informacao ret = transplex(&my_grafo, cicloHamiltoniano[index]);
	// cout<<"Size = "<<ret.caminho.size()<<endl;
	// cout<<"Custo "<<ret.custo<< " : ";
	// 	for (int j=0; j<ret.caminho.size(); j++){
	// 		cout<<"("<<ret.caminho[j]->getOrigem()<< ","<<ret.caminho[j]->getDestino()<<") ";
	// 	}

	// 	vector <Pasmideo> plasmideosSimples;
	// constroiPlasmideosFromCadeiaSimplex(&my_grafo, plasmideosSimples, ret);
	// cout<<endl;
	// for (int i=0; i<plasmideosSimples.size(); i++){
	// 	cout<<"Pasmideo "<<i<< " : ";
	// 	// for (int j=0; j<plasmideosSimples[i].cadeia.size(); j++){
	// 	// 	cout<<"("<<plasmideosSimples[i].cadeia[j]->getOrigem()<< ","<<plasmideosSimples[i].cadeia[j]->getDestino()<<") ";
	// 	// }
	// 	Informacao resultado;
	// 	bool isValido = manipula(&my_grafo, plasmideosSimples[i], cicloHamiltoniano[index], resultado);
	// 	if (isValido) {
	// 		cicloHamiltoniano[index] = resultado;
	// 		cout<<"OOOOOOOOK plasmideo "<<i<<endl;
	// 	}
	// 	cout<<endl;
	// }
	// cout<<"NOVOOOO "<<endl;
	// cout<<"Custo = "<<cicloHamiltoniano[index].custo<<endl;
	// for (int j=0; j<cicloHamiltoniano[1].caminho.size(); j++){
	// 	cout<<"("<<cicloHamiltoniano[1].caminho[j]->getOrigem()<< ","<<cicloHamiltoniano[1].caminho[j]->getDestino()<<") ";
	// }
	// cout<<endl;
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
	cout<<"Tempo total em segundos : "<<sec<<endl;

	
		
	// constroiPlasmideosFromTree(&my_grafo,plasmideosSimples,resul.arvores);
	// vector <Informacao> endossibiontes =  randomEndosibitontes(&my_grafo);
	// for (int i=0; i<plasmideosSimples.size(); i++){
	// 	cout<<"Pasmideo "<<i<< " : ";
	// 	for (int j=0; j<plasmideosSimples[i].cadeia.size(); j++){
	// 		cout<<"("<<plasmideosSimples[i].cadeia[j]->getOrigem()<< ","<<plasmideosSimples[i].cadeia[j]->getDestino()<<") ";
	// 	}
	// 	cout<<endl;
	// }

	// for (int i=0; i<endossibiontes.size(); i++){
	// 	cout<<"Endossibionte "<<i<<" (" << endossibiontes[i].custo <<") : ";
	// 	for (int j=0; j<endossibiontes[i].caminho.size(); j++){
	// 		cout<<"("<<endossibiontes[i].caminho[j]->getOrigem()<< ","<<endossibiontes[i].caminho[j]->getDestino()<<") ";
	// 	}
	// 	cout<<endl;
	// }
	// Informacao info;
	// bool re = manipula(&my_grafo, plasmideosSimples[0], endossibiontes[0], info);
	// if (re){
	// 	cout<<"Resultado custo = "<<info.custo<<endl;
	// 	for (int j=0; j<info.caminho.size(); j++){
	// 		cout<<"("<<info.caminho[j]->getOrigem()<< ","<<info.caminho[j]->getDestino()<<") ";
	// 	}
	// 	cout<<endl;
	// } else cout<<re<<endl;
	 

	return 0;
}