#ifndef _TRANSPLEX_H_
#define _TRANSPLEX_H_

#include "Grafo.h"
#include <vector>
#include "Informacao.h"
#include "gurobi_c++.h"

void init(Grafo *g);
Informacao simplexrelaxado(Grafo *g);
Informacao transplex(Grafo *g, Informacao cicloHamiltoniano);

#endif