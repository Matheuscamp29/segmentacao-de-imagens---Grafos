#include "../include/Edmonds.h"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// Função auxiliar para encontrar a aresta de menor custo entrando em cada nó
void pegarMenoresArestas(int N, const vector<Edge> &edges, vector<int> &menorArestaIdx)
{
    vector<double> minVal(N, numeric_limits<double>::infinity());

    for (int i = 0; i < edges.size(); i++)
    {
        int u = edges[i].u;
        int v = edges[i].v;
        double w = edges[i].w;

        // Se achou uma aresta mais barata entrando em V, guarda o ÍNDICE dela
        if (u != v && w < minVal[v])
        {
            minVal[v] = w;
            menorArestaIdx[v] = i;
        }
    }
}

vector<int> edmonds(int root, int N, vector<Edge> edges)
{
    vector<int> menorArestaIdx(N, -1);
    pegarMenoresArestas(N, edges, menorArestaIdx);


    int groupCount = 0;
    vector<int> group(N, -1);
    vector<int> visitado(N, -1);
    bool temCiclo = false;

 
    for (int i = 0; i < N; i++)
    {
        if (i == root)
            continue;

        int v = i;
        while (v != root && visitado[v] != i && group[v] == -1)
        {
            visitado[v] = i;
            if (menorArestaIdx[v] == -1)
                break; // Nó inalcançável
            v = edges[menorArestaIdx[v]].u;
        }

        // Se achou um ciclo
        if (v != root && group[v] == -1 && menorArestaIdx[v] != -1)
        {
            temCiclo = true;
            int curr = v;
            while (group[curr] == -1)
            {
                group[curr] = groupCount;
                curr = edges[menorArestaIdx[curr]].u;
            }
            groupCount++;
        }
    }

    if (!temCiclo)
    {
        vector<int> resultado;
        for (int i = 0; i < N; i++)
        {
            if (i != root && menorArestaIdx[i] != -1)
            {
                resultado.push_back(edges[menorArestaIdx[i]].id);
            }
        }
        return resultado;
    }

    for (int i = 0; i < N; i++)
    {
        if (group[i] == -1)
            group[i] = groupCount++;
    }

    vector<Edge> novasArestas;


    vector<int> mapParaOriginal;

    for (int i = 0; i < edges.size(); i++)
    {
        int u = edges[i].u;
        int v = edges[i].v;
        double w = edges[i].w;

        int gu = group[u];
        int gv = group[v];

        if (gu != gv)
        {
            double custoNoCiclo = edges[menorArestaIdx[v]].w;

            novasArestas.push_back({gu, gv, w - custoNoCiclo, edges[i].id});
            mapParaOriginal.push_back(i);
        }
    }


    int novaRaiz = group[root];
    vector<int> arestasEscolhidasRecursao = edmonds(novaRaiz, groupCount, novasArestas);


    vector<int> resultadoFinal;
    vector<bool> noComEntradaExterna(N, false);


    for (int idAresta : arestasEscolhidasRecursao)
    {

        int idxOriginal = -1;
        for (size_t k = 0; k < novasArestas.size(); k++)
        {
            if (novasArestas[k].id == idAresta)
            {
                idxOriginal = mapParaOriginal[k];
                break;
            }
        }

        if (idxOriginal != -1)
        {
            resultadoFinal.push_back(edges[idxOriginal].id);
            noComEntradaExterna[edges[idxOriginal].v] = true;
        }
    }
    for (int i = 0; i < N; i++)
    {
        if (i != root && !noComEntradaExterna[i])
        {
            int u = edges[menorArestaIdx[i]].u;
            int v = edges[menorArestaIdx[i]].v;

           
            if (group[u] == group[v])
            {
                resultadoFinal.push_back(edges[menorArestaIdx[i]].id);
            }
        }
    }

    return resultadoFinal;
}
