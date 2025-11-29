#include "arborescencia_edmonds.hpp"
#include <iostream>
#include <vector>
#include <limits>

using namespace std;

double edmonds(int root, int N, vector<Edge> edges, vector<Edge> &result) {
    const double INF = 1e18;
    double total_cost = 0;

    while (true) {

        vector<double> in(N, INF);
        vector<int> parent(N, -1);

        //menor aresta que entra em cada nó
        for (auto &e : edges) {
            if (e.u != e.v && e.w < in[e.v]) {
                in[e.v] = e.w;
                parent[e.v] = e.u;
            }
        }

        in[root] = 0;

        //verificar se existe nó inalcançável
        for (int i = 0; i < N; i++) {
            if (in[i] == INF) {
                return -1; // não existe arborescência
            }
        }

        //detectar ciclos
        int qtdCiclos = 0;
        vector<int> visitado(N, -1), idCiclo(N, -1);

        for (int i = 0; i < N; i++) {
            total_cost += in[i];

            int v = i;
            while (v != root && idCiclo[v] == -1 && visitado[v] != i) {
                visitado[v] = i;
                v = parent[v];
            }

            if (v != root && idCiclo[v] == -1) {
                //ciclo encontrado
                for (int u = parent[v]; u != v; u = parent[u])
                    idCiclo[u] = qtdCiclos;

                idCiclo[v] = qtdCiclos;
                qtdCiclos++;
            }
        }

        //se não tem ciclos acabou
        if (qtdCiclos == 0) {
            result.clear();
            for (int i = 0; i < N; i++) {
                if (i != root)
                    result.push_back({ parent[i], i, in[i] });
            }
            return total_cost;
        }

        //nós fora de ciclos recebem id único
        for (int i = 0; i < N; i++) {
            if (idCiclo[i] == -1) {
                idCiclo[i] = qtdCiclos++;
            }
        }

        //construir novo grafo contraído
        int novoN = qtdCiclos;
        vector<Edge> novasArestas;

        for (auto &e : edges) {
            int u = idCiclo[e.u];
            int v = idCiclo[e.v];
            double w = e.w;

            if (u != v) {
                w -= in[e.v];
                novasArestas.push_back({u, v, w});
            }
        }

        root = idCiclo[root];
        N = novoN;
        edges = novasArestas;
    }
}
