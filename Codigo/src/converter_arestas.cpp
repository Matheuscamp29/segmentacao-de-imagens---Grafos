#include <vector>
#include "./include/Grafo.h"
#include "./include/Edmonds.h"

using namespace std;

vector<Edge> converterParaArestas(Grafo *g)
{
    vector<Edge> edges;
    int N = g->listaDeVertices.size();

    int id_contador = 0;
    for (int u = 0; u < N; u++)
    {
        Vertice *atual = g->listaDeVertices[u];

        for (auto &arestaVizinha : atual->vizinhos)
        {
            int v = arestaVizinha.destino->id;
            edges.push_back({u, v, arestaVizinha.peso, id_contador++});
        }
    }
    return edges;
}