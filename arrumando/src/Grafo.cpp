#include "../include/Grafo.h"
#include <cmath>

Aresta::Aresta(Vertice *destino, double peso)
{
    this->destino = destino;
    this->peso = peso;
}

Vertice::Vertice(int id, int x, int y, int r, int g, int b)
{
    this->id = id; // Atribuição do ID
    this->r = r;
    this->g = g;
    this->b = b;
    this->y = y;
    this->x = x;
}

void Vertice::adicionarAresta(Vertice *destino)
{
    // Lógica original de cálculo de peso mantida
    double diferenca = std::sqrt(
        std::pow(this->r - destino->r, 2) +
        std::pow(this->g - destino->g, 2) +
        std::pow(this->b - destino->b, 2));
    Aresta nova(destino, diferenca);
    vizinhos.push_back(nova);
}

Grafo::Grafo(bool direcionado)
{
    this->direcionado = direcionado;
}

Grafo::~Grafo()
{
    for (Vertice *v : listaDeVertices)
    {
        delete v;
    }
    listaDeVertices.clear();
}

void Grafo::adicionarVertice(Vertice *v)
{
    listaDeVertices.push_back(v);
}

void Grafo::inserirAresta(Vertice *origem, Vertice *destino)
{
    origem->adicionarAresta(destino);
    if (!this->direcionado)
    {
        destino->adicionarAresta(origem);
    }
}