#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_


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
#include "Informacao.h"
using namespace std;

void dijkstra(Grafo *g, int origem, vector< Informacao > &result,int maximo); // da origem para todos (Teorema 4.1 Goldbarg e Goldbarg 2012)

#endif