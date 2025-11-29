#ifndef ARBORESCENCIA_EDMONDS_HPP
#define ARBORESCENCIA_EDMONDS_HPP

#include <vector>

struct Edge {
    int u, v;      
    double w;      
};

//função principal do algoritmo de Edmonds (Chu–Liu/Edmonds)
double edmonds(int root, int N, std::vector<Edge> edges, std::vector<Edge> &resultado);

#endif
