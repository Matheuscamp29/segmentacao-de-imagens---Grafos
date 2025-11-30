#ifndef ARBORESCENCIA_EDMONDS_HPP
#define ARBORESCENCIA_EDMONDS_HPP

#include <vector>

struct Edge
{
    int u, v;
    double w;
    int id;
};

std::vector<int> edmonds(int root, int N, std::vector<Edge> edges);

#endif
