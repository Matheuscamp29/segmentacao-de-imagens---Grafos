#include <vector>
#include "arborescencia_edmonds.hpp"
#include "Grafo.cpp"  

using namespace std;

//converte o grafo (lista de adjacência) para lista de arestas pra q o Edmonds funcione
vector<Edge> converterParaArestas(Grafo* g) {
    vector<Edge> edges;
    int N = g->listaDeVertices.size();

    for (int u = 0; u < N; u++) {
        Vertice* atual = g->listaDeVertices[u];

        for (auto &a : atual->vizinhos) {
            Vertice* destino = a.destino;

            // Descobrir índice do destino
            int v = -1;
            for (int i = 0; i < N; i++) {
                if (g->listaDeVertices[i] == destino) {
                    v = i;
                    break;
                }
            }

            edges.push_back({u, v, a.peso});
        }
    }

    return edges;
}
