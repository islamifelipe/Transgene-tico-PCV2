#include "transplex.h"
#include <iostream>
#include <map> 
#include <string>
#define MAXITERACOES 6000

GRBVar *u, **x;
void init(Grafo *g){ // deve ser chamada apenas uma vez
	//x binario 
	//u para cada vertice
	x = new GRBVar*[g->getQuantVertices()]; 
   	u = new GRBVar[g->getQuantVertices()];

   	for (int i=0; i<g->getQuantVertices();i++){
        x[i] = new GRBVar[g->getQuantVertices()];
   	}
}

//NAO FUNCIONA AINDA
Informacao transplex(Grafo *g, Informacao cicloHamiltoniano){
  GRBEnv env = GRBEnv();;

  //env.set("OutputFlag","0");
  //env.set(GRB_DoubleParam_IterationLimit,MAXITERACOES);
  //env.set(GRB_DoubleParam_Heuristics, 0); // 0% do tempo aplicado a heuristica do root
  //env.set(GRB_IntParam_NodeMethod, 0);
  env.set(GRB_IntParam_Method, 1); // primal
  env.set(GRB_IntParam_SiftMethod,1);
  GRBModel model = GRBModel(env);;

  //inicializa variaveis;
  for (int i=0; i<g->getQuantArestas(); i++){ // se o grafo for direcionado, exitira uma aresta ij e outra ji
    int origem = g->getArestas(i)->getOrigem();
    int destino = g->getArestas(i)->getDestino();
    x[origem][destino] = model.addVar(0.0, 1, 0.0, GRB_BINARY, "x"+std::to_string(origem)+std::to_string(destino)); // relaxacao linear
    x[destino][origem] = model.addVar(0.0, 1, 0.0, GRB_BINARY, "x"+std::to_string(destino)+std::to_string(origem));
  }///GRB_BINARY
  for (int i=1; i<g->getQuantVertices(); i++){
    u[i] = model.addVar(0.0, 10000000, 0.0, GRB_CONTINUOUS, "u"+std::to_string(i));
  }

   model.update();
   int constrCont = 0;
   GRBLinExpr exprObjet;
   GRBLinExpr rest1,nova;
   double um = 1;
   for (int i=0; i<g->getQuantArestas(); i++){
      double peso = g->getArestas(i)->getPeso();
      exprObjet.addTerms(&peso, &x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()],1); 
      exprObjet.addTerms(&peso, &x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()],1); //destino origem também ??????
      
      if (g->getArestas(i)->getDestino()==0 || g->getArestas(i)->getOrigem()==0){
        rest1.addTerms(&um, &x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()],1);
      
      }
   }
 
   model.setObjective(exprObjet,GRB_MINIMIZE); 
  model.addConstr(rest1, GRB_EQUAL, 1,std::to_string(constrCont++));

   for (int j=1; j<g->getQuantVertices(); j++){
      GRBLinExpr rest2;
      for (int i=0; i<g->getQuantVertices(); i++){
        if (i!=j){ 
          if (g->isAdjacente(j,i)!=-1){// sai de j
            rest2.addTerms(&um,&x[j][i],1);
          } 
        }
      }
      model.addConstr(rest2, GRB_EQUAL, 1,std::to_string(constrCont++));

    }
    for (int i=1; i<g->getQuantVertices(); i++){
      GRBLinExpr rest3;
      for (int j=0; j<g->getQuantVertices(); j++){
        if (i!=j){ 
          if (g->isAdjacente(j,i)!=-1){// sai de j
            rest3.addTerms(&um,&x[j][i],1);
          } 
        }
      }
       model.addConstr(rest3, GRB_EQUAL, 1,std::to_string(constrCont++));
     
    }
    
   double menos1=-1;
   double p = g->getQuantVertices()-1;
    for (int i=1; i<g->getQuantVertices(); i++){
      for (int j=1; j<g->getQuantVertices(); j++){
        if(i!=j && g->isAdjacente(i,j)!=-1){
          GRBLinExpr rest4;
          rest4.addTerms(&um,&u[i],1);
          rest4.addTerms(&menos1,&u[j],1);
          rest4.addTerms(&p,&x[i][j],1); // grafo nao completo
          model.addConstr(rest4, GRB_LESS_EQUAL, p-1,std::to_string(constrCont++));
        }
      }
    }
    model.update();
    // for (int ii=0; ii<constrCont; ii++){
    //   GRBConstr cons= model.getConstrByName(std::to_string(ii));
    //   cons.set(GRB_DoubleAttr_DStart,1);

    // }
    // for (int i=0; i<g->getQuantArestas(); i++){
    //      int origem = g->getArestas(i)->getOrigem();
    //     int destino = g->getArestas(i)->getDestino();
    //     x[origem][destino].set(GRB_DoubleAttr_Start,0);
    //     x[destino][origem].set(GRB_DoubleAttr_Start,0);
    // }
    // for (int i=1; i<g->getQuantVertices(); i++){
    //   u[i].set(GRB_DoubleAttr_Start,0);
    // }
    // for (int i=0; i<cicloHamiltoniano.caminho.size(); i++){
    //   int ori = cicloHamiltoniano.caminho[i]->getOrigem();
    //   int dest = cicloHamiltoniano.caminho[i]->getDestino();
    //   if (ori>dest){ // dest deve ser maior
    //     dest = cicloHamiltoniano.caminho[i]->getOrigem();
    //     ori = cicloHamiltoniano.caminho[i]->getDestino();
    //   }
    //   x[ori][dest].set(GRB_DoubleAttr_Start,1);
    //   u[dest].set(GRB_DoubleAttr_Start,1);
    // }

  model.optimize();
  int optimstatus = model.get(GRB_IntAttr_Status);
  vector<Aresta*> caminho;
  Informacao ret = {caminho,0};
  if (optimstatus != GRB_INFEASIBLE){
    for (int i=0; i<g->getQuantArestas(); i++){
      try{
        if (x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()].get(GRB_DoubleAttr_X)>=0.1 || x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()].get(GRB_DoubleAttr_X)>=0.1){
          cout<<"x"<<g->getArestas(i)->getOrigem()<<"_"<<g->getArestas(i)->getDestino()<<" = "<<x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()].get(GRB_DoubleAttr_X)<<endl;
         cout<<"x"<<g->getArestas(i)->getDestino()<<"_"<<g->getArestas(i)->getOrigem()<<" = "<<x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()].get(GRB_DoubleAttr_X)<<endl;
          
          ret.caminho.push_back(g->getArestas(i));
          ret.custo+=g->getArestas(i)->getPeso();
        }
      } catch(GRBException ex){

      }
    }
  } 

  return ret;
  
}

//p = n-1
/* Esta funcao executa um simplex relaxado (relaxacao linear) para o modelo do PCV de Miller,Tucker e Zemlin (1960) (apud Goldbarg e Luna, 2005)
 * O resultado desta funcao serve como limite inferior para o transgenético e é utilizado para gerar caminhos que serao armazenados no hospedeiro
 * O relaxamento é feito nas variaveis binarias x (que viram continuas) e na quantidade limitada de iteracoes a MAXITERACOES*/
Informacao simplexrelaxado(Grafo *g){
	GRBEnv env = GRBEnv();;
 env.set("OutputFlag","0");
 env.set(GRB_DoubleParam_IterationLimit,MAXITERACOES);
 env.set(GRB_DoubleParam_Heuristics, 0); // 0% do tempo aplicado a heuristica do root
  //env.set(GRB_IntParam_NodeMethod, 0);
  env.set(GRB_IntParam_Method, 1); // 1 é mais rapido //primal
  env.set(GRB_IntParam_SiftMethod,1);
	GRBModel model = GRBModel(env);;

	//inicializa variaveis;
	for (int i=0; i<g->getQuantArestas(); i++){ // se o grafo for direcionado, exitira uma aresta ij e outra ji
		int origem = g->getArestas(i)->getOrigem();
		int destino = g->getArestas(i)->getDestino();
		x[origem][destino] = model.addVar(0.0, 1, 0.0, GRB_CONTINUOUS, "x"+std::to_string(origem)+std::to_string(destino)); // relaxacao linear
		x[destino][origem] = model.addVar(0.0, 1, 0.0, GRB_CONTINUOUS, "x"+std::to_string(destino)+std::to_string(origem));
	}///GRB_BINARY
	for (int i=1; i<g->getQuantVertices(); i++){
		u[i] = model.addVar(0.0, 10000000, 0.0, GRB_CONTINUOUS, "u"+std::to_string(i));
	}

   model.update();
   int constrCont = 0;
   GRBLinExpr exprObjet;
   GRBLinExpr rest1,nova;
   double um = 1;
   for (int i=0; i<g->getQuantArestas(); i++){
   		double peso = g->getArestas(i)->getPeso();
   		exprObjet.addTerms(&peso, &x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()],1); 
   		exprObjet.addTerms(&peso, &x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()],1); //destino origem também ??????
   		
      if (g->getArestas(i)->getDestino()==0 || g->getArestas(i)->getOrigem()==0){
   			rest1.addTerms(&um, &x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()],1);
   		//	rest1.addTerms(&um, &x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()],1);
   		
      }
   }
 
   model.setObjective(exprObjet,GRB_MINIMIZE); 
  model.addConstr(rest1, GRB_EQUAL, 1,std::to_string(constrCont++));
   //model.update();

   for (int j=1; j<g->getQuantVertices(); j++){
  		GRBLinExpr rest2;
  		for (int i=0; i<g->getQuantVertices(); i++){
        if (i!=j){ 
          if (g->isAdjacente(j,i)!=-1){// sai de j
            rest2.addTerms(&um,&x[j][i],1);
          } 
        }
      }
      model.addConstr(rest2, GRB_EQUAL, 1,std::to_string(constrCont++));

    }
    //cout<<"\n\n"<<endl;
    for (int i=1; i<g->getQuantVertices(); i++){
      GRBLinExpr rest3;
      for (int j=0; j<g->getQuantVertices(); j++){
        if (i!=j){ 
          if (g->isAdjacente(j,i)!=-1){// sai de j
            rest3.addTerms(&um,&x[j][i],1);
           // cout<<"+1x"<<j<<i<<" ";
          } 
        }
      }
       model.addConstr(rest3, GRB_EQUAL, 1,std::to_string(constrCont++));
     
    }
  	
   double menos1=-1;
   double p = g->getQuantVertices()-1;
    for (int i=1; i<g->getQuantVertices(); i++){
    	 for (int j=1; j<g->getQuantVertices(); j++){
    	 	if(i!=j && g->isAdjacente(i,j)!=-1){
    	 		GRBLinExpr rest4;
          //cout<<"+ "<<um<<"u"<<i<<" ";
    			rest4.addTerms(&um,&u[i],1);
    	 		rest4.addTerms(&menos1,&u[j],1);
         // cout<<"- 1"<<"u"<<j<<" ";
    	 		rest4.addTerms(&p,&x[i][j],1); // grafo nao completo
         // cout<<"+ "<<p<<"x"<<i<<j<<" ";
    	 		model.addConstr(rest4, GRB_LESS_EQUAL, p-1,std::to_string(constrCont++));
  			  //cout<<"<= "<<p-1<<endl;
          }
    	 }
    }
     model.optimize();
      // model.update();
	int optimstatus = model.get(GRB_IntAttr_Status);
	vector<Aresta*> caminho;
	Informacao ret = {caminho,0};
	if (optimstatus != GRB_INFEASIBLE){
		for (int i=0; i<g->getQuantArestas(); i++){
			try{
        if (x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()].get(GRB_DoubleAttr_X)>=0.1 || x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()].get(GRB_DoubleAttr_X)>=0.1){
         //  cout<<"x"<<g->getArestas(i)->getOrigem()<<"_"<<g->getArestas(i)->getDestino()<<" = "<<x[g->getArestas(i)->getOrigem()][g->getArestas(i)->getDestino()].get(GRB_DoubleAttr_X)<<endl;
         // cout<<"x"<<g->getArestas(i)->getDestino()<<"_"<<g->getArestas(i)->getOrigem()<<" = "<<x[g->getArestas(i)->getDestino()][g->getArestas(i)->getOrigem()].get(GRB_DoubleAttr_X)<<endl;
          
          ret.caminho.push_back(g->getArestas(i));
          ret.custo+=g->getArestas(i)->getPeso();
        }
      } catch(GRBException ex){

      }
		}
	} 

	return ret;
	
}