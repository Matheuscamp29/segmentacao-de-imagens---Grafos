#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "Grafo.h"
#include <vector>

struct ArestaSimples
{
    int u, v;
    double peso;
};

struct ResultadoKruskal
{
    double custoTotal;
    std::vector<ArestaSimples> arestasEscolhidas;
};

ResultadoKruskal executarKruskal(Grafo &grafo);

#endif