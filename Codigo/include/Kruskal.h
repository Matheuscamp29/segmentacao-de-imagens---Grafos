#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include "Grafo.h"


struct ArestaSimples {
    int u;
    int v;
    double peso;
};


struct ResultadoKruskal {
    double custoTotal;
    std::vector<ArestaSimples> arestasEscolhidas;
};


ResultadoKruskal executarKruskal(Grafo &grafo);

#endif