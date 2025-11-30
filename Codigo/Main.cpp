#include <iostream>
#include <vector>
#include <algorithm> 

// Seus includes
#include "arborescencia_edmonds.hpp"
#include "./include/Kruskal.h"            // Não esqueça do Kruskal
#include "converter_arestas.cpp" 
#include "./include/Segmentacao.h"
#include "./include/Imagem.h"
#include "./include/Grafo.h"

using namespace std;


void rodarEdmonds(Imagem& img) {
  
    Grafo* g = new Grafo(true); 
    converterImagemParaGrafo(img, *g);


    vector<Edge> edges = converterParaArestas(g);


    int maxId = 0;
    for(const auto& e : edges) if(e.id > maxId) maxId = e.id;
    
    vector<Edge> mapaArestas(maxId + 1); 
    for (const auto& e : edges) {
        mapaArestas[e.id] = e;
    }

    int raiz = 0; 
    int N = g->listaDeVertices.size();

   
    cout << "[EDMONDS] Executando logica recursiva..." << endl;
    vector<int> idsEscolhidos = edmonds(raiz, N, edges);


    vector<Edge> resultadoArbo;
    double custoTotal = 0;

    for (int id : idsEscolhidos) {
        Edge original = mapaArestas[id];
        resultadoArbo.push_back(original);
        custoTotal += original.w;
    }


    int contagem = 0;
    cout << "--- Resultado Edmonds ---" << endl;
    for (auto &e : resultadoArbo) {
        if (e.w > 0.001) { 
            //cout << e.u << " -> " << e.v << " (peso = " << e.w << ")\n";
            contagem++;
        }
    }
    
    if (contagem == 0) cout << "Nenhuma aresta com peso > 0 (Imagem solida?).\n";
    cout << "Custo Total: " << custoTotal << endl;

    delete g;
}


void rodarKruskal(Imagem& img) {
    cout << "\n[KRUSKAL] Iniciando configuracao..." << endl;

    Grafo* g = new Grafo(false); 
    converterImagemParaGrafo(img, *g);

    ResultadoKruskal resultado = executarKruskal(*g);

    cout << "--- Resultado Kruskal ---" << endl;
    
    int arestasVisiveis = 0;

    for (const auto& aresta : resultado.arestasEscolhidas) {
        if (aresta.peso > 0.001) {
            arestasVisiveis++;
        }
    }

    if (arestasVisiveis == 0) cout << "Nenhuma aresta com peso > 0 (Imagem solida?).\n";
    
    cout << "Custo Total MST: " << resultado.custoTotal << endl;
    cout << "Total arestas na MST: " << resultado.arestasEscolhidas.size() << endl;

    delete g;
}

int main() {
    string path = "./image.jpg";
    Imagem img;

    if (img.carregar(path)) {
        cout << "Imagem carregada com sucesso.\n";
    } else {
        cout << "Falha ao carregar imagem.\n";
        return 1;   
    }

    int opcao = 0;
    do {
        cout << "\n================ MENU ================" << endl;
        cout << "1 - Gerar grafo direcionado + Edmonds" << endl;
        cout << "2 - Gerar grafo nao direcionado + Kruskal" << endl;
        cout << "0 - Sair" << endl;
        cout << "Escolha: ";
        cin >> opcao;

        switch(opcao) {
            case 1:
                rodarEdmonds(img); // Passa por referência
                break;
            
            case 2:
                rodarKruskal(img); // Passa por referência
                break;
            
            case 0:
                cout << "Saindo..." << endl;
                break;
            
            default:
                cout << "Opcao invalida!" << endl;
        }

    } while (opcao != 0);

    return 0;
}