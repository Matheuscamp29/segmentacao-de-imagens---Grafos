#include "../include/Kruskal.h"
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

struct UnionFindColor
{
    std::vector<int> pai;
    std::vector<int> tamanho;

    UnionFindColor(int n)
    {
        pai.resize(n);
        tamanho.resize(n, 1);
        for (int i = 0; i < n; i++)
            pai[i] = i;
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
        {
           
            if (tamanho[root_i] < tamanho[root_j])
            {
                pai[root_i] = root_j;
                tamanho[root_j] += tamanho[root_i];
            }
            else
            {
                pai[root_j] = root_i;
                tamanho[root_i] += tamanho[root_j];
            }
        }
    }

    int getSize(int i)
    {
        return tamanho[find(i)];
    }
};

ResultadoKruskal executarKruskal(Grafo &grafo)
{
    ResultadoKruskal res;
    res.custoTotal = 0;

    std::vector<ArestaSimples> arestas = extrairArestasKruskal(grafo);

    std::sort(arestas.begin(), arestas.end(), [](const ArestaSimples &a, const ArestaSimples &b)
              { return a.peso < b.peso; });

    UnionFindColor uf(grafo.listaDeVertices.size());

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