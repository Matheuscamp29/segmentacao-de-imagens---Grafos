#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <iostream>

class Vertice;

class Aresta
{
public:
    Vertice *destino;
    double peso;

    Aresta(Vertice *destino, double peso);
};

class Vertice
{
public:
    int id;
    int r, g, b;
    int x, y;
    std::vector<Aresta> vizinhos;

    Vertice(int id, int x, int y, int r, int g, int b);

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