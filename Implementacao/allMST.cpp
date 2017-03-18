#include "allMST.h" 

using namespace std;


///ALGORITMO DA SONRENSEN JANSSENS (2003)

/* O algoritmo foi adaptado pra retornar **apenas** as arvores geradoras minimas de um grafo (desconsiderando as outras)
*/

void Partition(Grafo P, vector<Aresta *> &Pa, Heap &List, map<int, pair< Informacao, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes, Aresta ** arestasPtr){
	/*Pa = vetor de arestas = correspondente à partição P (n-1 arestas)
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
	*/
	Grafo P1 = P, P2 = P;
	bool res = false;
	float custo;
	//int *A2;
	int m = P.getQuantArestas();
	int n = P.getQuantVertices();
	//cout<<List.getSize()<<endl;
	for (int i=0; i<n-1 && List.getSize()<MAX2-1; i++){
		int iddnovo = Pa[i]->getId();
		if (P.getStatus(iddnovo)==0){ /*Se a aresta for opcional*/
			vector <Aresta *> A2;
			P1.setStatus(iddnovo, 2); /*proibida*/
			P2.setStatus(iddnovo, 1); /*obrigatória*/
			custo=0;
			res = kruskal(&P1, arestasPtr,A2, custo);
			if (res){
				//cout<<"size = "<<List.getSize()<<endl;
				vetorParticoes.push_back(P1);
				list<Grafo>::iterator itt = vetorParticoes.end();
				itt--;
				int idMST = List.getSize();
				Informacao iff = {A2, custo};
				MSTs[idMST] = make_pair(iff,itt);//A2;
				List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
			}
			P1.setStatus(iddnovo, 1); 
		}
	}
}

vector< Informacao > AllSpaningTree(Grafo *g, Aresta ** arestasPtr,int maximo){ 
	//cout<<"MAX = "<<MAX2<<endl;
	vector< Informacao > result;
	Heap List(MAX2); 
	list<Grafo> vetorParticoes; //Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	map<int, pair< Informacao, list<Grafo>::iterator > > MSTs; // usada para lista de árvores do retorno
	
	vector <Aresta *> A;
	mergesort(arestasPtr, g->getQuantArestas());
	float custo = 0;
	bool res = kruskal(g, arestasPtr, A,custo);
	if (res){
		vetorParticoes.push_back(*g);
		list<Grafo>::iterator itt = vetorParticoes.end();
		itt--;
		int idMST = List.getSize();
		Informacao iff = {A, custo};
		MSTs[idMST] = make_pair(iff,itt);//A2;
		List.insert(idMST, custo); // o valor da variavel "custo" vem do kruskal
	}


	while (List.getSize()!=0){
			int id = List.getId();
			pair< Informacao, list<Grafo>::iterator > par = MSTs[id];
				
			Informacao it = par.first;//MSTs[id];
			if (it.custo == custo){
				if (result.size()<maximo-1){ // IMPOMOS UM LIMITE NA QUANTIDADE DE ARVORES MINIMAS 
					result.push_back(it);
					Grafo Ps = *par.second;
					//cout<<"apos Ps = "<<endl;
					List.extract();
					Partition(Ps,it.caminho, List,MSTs,vetorParticoes,arestasPtr);
				} else if (result.size()==maximo-1)result.push_back(it);
				else break;
			} else break;
	}		
	//List.desaloca();
	return result;
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)
