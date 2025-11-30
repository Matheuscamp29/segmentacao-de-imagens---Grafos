#include <iostream>
#include "arborescencia_edmonds.hpp"
#include "converter_arestas.cpp"
#include "Segmentacao.cpp"

using namespace std;

int main() {

    string path = "./imagem.jpg";
    Imagem img;

    Grafo* g = new Grafo(true); //grafo direcionado

    if (img.carregar(path)) {
        cout << "Imagem carregada.\n";
        converterImagemParaGrafo(img, *g);
    } else {
        cout << "Falha ao carregar imagem.\n";
        return 0;   
    }

    // converter para lista de arestas
    vector<Edge> edges = converterParaArestas(g);

    //resultado final
    vector<Edge> resultadoArbo;

    int raiz = 0; //vertice inicial da arborescência

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
