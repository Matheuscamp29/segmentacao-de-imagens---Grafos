#include "../include/Edmonds.h"
#include <vector>
#include <limits>

// Função auxiliar para converter Grafo -> Lista de Arestas (como no seu arquivo original)
std::vector<Edge> converterParaArestasEdmonds(Grafo &grafo)
{
    std::vector<Edge> edges;
    for (Vertice *u : grafo.listaDeVertices)
    {
        for (auto &a : u->vizinhos)
        {
            edges.push_back({u->id, a.destino->id, a.peso});
        }
    }
    return edges;
}

double edmonds_impl(int root, int N, std::vector<Edge> &edges)
{
    const double INF = 1e18;
    double total_cost = 0;

    while (true)
    {
        std::vector<double> in(N, INF);
        std::vector<int> parent(N, -1);
        std::vector<int> idCiclo(N, -1);
        std::vector<int> visitado(N, -1);

        // 1. Menor aresta de entrada
        for (const auto &e : edges)
        {
            if (e.u != e.v && e.w < in[e.v])
            {
                in[e.v] = e.w;
                parent[e.v] = e.u;
            }
        }

        in[root] = 0;

        // Verificar conexidade
        for (int i = 0; i < N; i++)
        {
            if (i != root && in[i] == INF)
                return -1;
        }

        // 2. Detectar Ciclos
        int qtdCiclos = 0;
        for (int i = 0; i < N; i++)
        {
            if (i == root)
                continue;
            total_cost += in[i];

            int v = i;
            while (v != root && idCiclo[v] == -1 && visitado[v] != i)
            {
                visitado[v] = i;
                v = parent[v];
            }

            if (v != root && idCiclo[v] == -1)
            {
                for (int u = parent[v]; u != v; u = parent[u])
                    idCiclo[u] = qtdCiclos;
                idCiclo[v] = qtdCiclos;
                qtdCiclos++;
            }
        }

        if (qtdCiclos == 0)
            return total_cost;

        // 3. Contrair Ciclos
        for (int i = 0; i < N; i++)
        {
            if (idCiclo[i] == -1)
            {
                idCiclo[i] = qtdCiclos++;
            }
        }

        int novoN = qtdCiclos;
        std::vector<Edge> novasArestas;

        for (const auto &e : edges)
        {
            int u = idCiclo[e.u];
            int v = idCiclo[e.v];
            if (u != v)
            {
                novasArestas.push_back({u, v, e.w - in[e.v]});
            }
        }

        root = idCiclo[root];
        N = novoN;
        edges = novasArestas;
    }
}

double executarEdmonds(Grafo &grafo, int raiz)
{
    std::vector<Edge> edges = converterParaArestasEdmonds(grafo);
    return edmonds_impl(raiz, grafo.listaDeVertices.size(), edges);
}