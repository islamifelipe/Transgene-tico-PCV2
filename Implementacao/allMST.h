#ifndef _ALLMST_H_
#define _ALLMST_H_

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
#include <vector>
#include "Informacao.h"
using namespace std;

#define MAX2 5

void Partition(Grafo P, vector<Aresta *> &Pa, Heap &List, map<int, pair< Informacao, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes, Aresta ** arestasPtr);
vector< Informacao > AllSpaningTree(Grafo *g, Aresta ** arestasPtr, int maximo);

#endif