#include <iostream>
#include "arborescencia_edmonds.hpp"
#include "converter_arestas.cpp"

using namespace std;

int main() {

    Grafo* g = new Grafo(true); //true = direcionado

    // ... construir vértices e arestas ...




    //tudo necessario para edmonds
    //converter para lista de arestas
    vector<Edge> edges = converterParaArestas(g);

    //resultado da arborescencia
    vector<Edge> resultadoArbo;

    //raiz da arborescência (ex: primeiro vértice)
    int raiz = 0;

    double custo = edmonds(raiz, g->listaDeVertices.size(), edges, resultadoArbo);

    if (custo == -1) {
        cout << "Nao existe arborescencia geradora!\n";
    } else {
        cout << "Custo final = " << custo << "\n";
        cout << "Arestas da arborescencia:\n";
        for (auto &e : resultadoArbo) {
            cout << e.u << " -> " << e.v << " (peso = " << e.w << ")\n";
        }
    }

    return 0;
}
