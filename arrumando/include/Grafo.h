#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <iostream>

class Vertice;

class Aresta
{
public:
    // ID removido daqui
    Vertice *destino;
    double peso;

    Aresta(Vertice *destino, double peso);
};

class Vertice
{
public:
    int id; // ID movido para cá
    int r, g, b;
    int x, y;
    std::vector<Aresta> vizinhos;

    Vertice(int id, int x, int y, int r, int g, int b);

    // Método mantém a lógica original de calcular peso euclidiano
    void adicionarAresta(Vertice *destino);
};

class Grafo
{
public:
    std::vector<Vertice *> listaDeVertices;
    bool direcionado;

    Grafo(bool direcionado);
    ~Grafo();

    void adicionarVertice(Vertice *v);
    void inserirAresta(Vertice *origem, Vertice *destino);
};

#endif