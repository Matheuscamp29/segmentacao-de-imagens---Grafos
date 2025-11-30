// Estrutura dos grafos
#include <iostream>
#include <vector>
#include <cmath>
#include "./include/Grafo.h"
#include "./include/imagem.h"
using namespace std;


Aresta::Aresta(Vertice* destino, double peso){
    this->destino = destino;
    this->peso = peso;
};


Vertice::Vertice(int id, int x, int y, int r, int g, int b) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
}
        
void Vertice::adicionarAresta(Vertice* destino){
    double diferenca = std::sqrt(
        std::pow(this->r - destino->r, 2) +
        std::pow(this->g - destino->g, 2) +
        std::pow(this->b - destino->b, 2)
    );
    Aresta nova (destino, diferenca);
    vizinhos.push_back(nova);
}

void Grafo::adicionarVertice(Vertice* v) {
    listaDeVertices.push_back(v);
}

Grafo::Grafo(bool direcionado) {
    this->direcionado = direcionado;
}


Grafo::~Grafo() {
}

void Grafo::inserirAresta(Vertice* origem, Vertice* destino) {
    
    origem->adicionarAresta(destino);
    
    if (!this->direcionado) {
        destino->adicionarAresta(origem);
    }
}



