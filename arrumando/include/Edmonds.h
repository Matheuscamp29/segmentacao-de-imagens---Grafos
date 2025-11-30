#ifndef EDMONDS_H
#define EDMONDS_H

#include <vector>
#include "Imagem.h"

// Estrutura leve para o algoritmo (16 bytes)
struct ArestaEdmonds
{
    int u, v;
    float peso;
    int id_orig; // Índice da aresta original para recuperação
};

// Estrutura para o resultado final
struct ArestaFinal
{
    int u, v;
    float peso;
};

struct ResultadoEdmonds
{
    double custoTotal;
    std::vector<ArestaFinal> arestas;
};

// Nova função que aceita a lista de arestas diretamente, sem depender da classe Grafo
ResultadoEdmonds executarEdmondsDireto(int numVertices, std::vector<ArestaEdmonds> &arestas, int raiz);

#endif