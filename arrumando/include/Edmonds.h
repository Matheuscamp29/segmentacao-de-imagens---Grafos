#ifndef EDMONDS_H
#define EDMONDS_H

#include "Grafo.h"
#include <vector>

// Estrutura necessária para o algoritmo
struct Edge
{
    int u, v;
    double w;
};

// Recebe o grafo, converte internamente e calcula
double executarEdmonds(Grafo &grafo, int raiz);

#endif