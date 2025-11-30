#include "./include/Kruskal.h"
#include <algorithm>
#include <numeric>

std::vector<ArestaSimples> extrairArestasKruskal(Grafo &grafo)
{
    std::vector<ArestaSimples> arestas;
    for (Vertice *v : grafo.listaDeVertices)
    {
        for (auto &a : v->vizinhos)
        {
            if (grafo.direcionado || v->id < a.destino->id)
            {
                arestas.push_back({v->id, a.destino->id, a.peso});
            }
        }
    }
    return arestas;
}

struct UnionFind
{
    std::vector<int> pai;
    UnionFind(int n)
    {
        pai.resize(n);
        std::iota(pai.begin(), pai.end(), 0);
    }
    int find(int i)
    {
        if (pai[i] == i)
            return i;
        return pai[i] = find(pai[i]);
    }
    void unite(int i, int j)
    {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j)
            pai[root_i] = root_j;
    }
};

ResultadoKruskal executarKruskal(Grafo &grafo)
{
    ResultadoKruskal res;
    res.custoTotal = 0;

    std::vector<ArestaSimples> arestas = extrairArestasKruskal(grafo);

    std::sort(arestas.begin(), arestas.end(), [](const ArestaSimples &a, const ArestaSimples &b)
              { return a.peso < b.peso; });

    UnionFind uf(grafo.listaDeVertices.size());

    for (const auto &aresta : arestas)
    {
        if (uf.find(aresta.u) != uf.find(aresta.v))
        {
            uf.unite(aresta.u, aresta.v);
            res.custoTotal += aresta.peso;
            res.arestasEscolhidas.push_back(aresta);
        }
    }
    return res;
}