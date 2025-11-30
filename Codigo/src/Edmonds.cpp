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

    // 1. Detectar Ciclos
    int groupCount = 0;
    vector<int> group(N, -1);
    vector<int> visitado(N, -1);
    bool temCiclo = false;

    // Tenta encontrar ciclos usando as menores arestas escolhidas
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

    // --- CASO BASE: Se não tem ciclo, achamos a resposta! ---
    if (!temCiclo)
    {
        vector<int> resultado;
        for (int i = 0; i < N; i++)
        {
            if (i != root && menorArestaIdx[i] != -1)
            {
                // Retorna o ID original da aresta
                resultado.push_back(edges[menorArestaIdx[i]].id);
            }
        }
        return resultado;
    }

    // --- PASSO RECURSIVO: Contração do Ciclo ---

    // Todo nó que não está num ciclo ganha seu próprio grupo
    for (int i = 0; i < N; i++)
    {
        if (group[i] == -1)
            group[i] = groupCount++;
    }

    vector<Edge> novasArestas;

    // Mapeia qual aresta original corresponde à nova aresta contraída
    // newEdge -> originalEdgeIndex
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
            // A mágica do Edmonds: w_nova = w_velha - peso_da_aresta_no_ciclo
            // Isso penaliza arestas que tentam entrar no ciclo
            double custoNoCiclo = edges[menorArestaIdx[v]].w;

            novasArestas.push_back({gu, gv, w - custoNoCiclo, edges[i].id});
            mapParaOriginal.push_back(i);
        }
    }

    // Chamada Recursiva
    int novaRaiz = group[root];
    vector<int> arestasEscolhidasRecursao = edmonds(novaRaiz, groupCount, novasArestas);

    // --- EXPANSÃO (Reconstrução) ---
    vector<int> resultadoFinal;
    vector<bool> noComEntradaExterna(N, false);

    // 1. Adiciona as arestas que a recursão escolheu
    for (int idAresta : arestasEscolhidasRecursao)
    {
        // Precisamos achar qual aresta na lista ATUAL (edges) gerou essa aresta recursiva
        // Como o ID é único globalmente, podemos procurar ou usar o mapeamento.
        // Pela simplicidade aqui, vamos procurar na lista 'novasArestas' qual tem esse ID

        int idxOriginal = -1;
        // Procura linear (pode ser otimizada, mas funciona)
        for (size_t k = 0; k < novasArestas.size(); k++)
        {
            if (novasArestas[k].id == idAresta)
            {
                // Recupera o índice da aresta real na lista 'edges'
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

    // 2. Adiciona as arestas internas dos ciclos (que tínhamos guardado em menorArestaIdx)
    // A MENOS que aquele nó já tenha recebido uma conexão vinda de fora (passo acima)
    for (int i = 0; i < N; i++)
    {
        if (i != root && !noComEntradaExterna[i])
        {
            // Se o nó i faz parte de um ciclo que foi contraído, a gente restaura a aresta interna dele
            // Se ele não faz parte de ciclo contraído, ele já foi tratado pela recursão (ou é isolado)

            // Truque: A recursão resolve o grafo de "grupos".
            // Se o nó 'i' está dentro de um grupo que recebeu uma aresta, 'noComEntradaExterna' será false para ele,
            // a menos que ele seja EXATAMENTE o nó que recebeu a aresta.

            // Simplificação: Adicionamos as arestas do "ciclo original" para todos,
            // exceto para quem virou a "entrada" do ciclo.
            int u = edges[menorArestaIdx[i]].u;
            int v = edges[menorArestaIdx[i]].v;

            // Só adiciona se u e v estavam no mesmo grupo (era uma aresta interna do ciclo)
            if (group[u] == group[v])
            {
                resultadoFinal.push_back(edges[menorArestaIdx[i]].id);
            }
        }
    }

    return resultadoFinal;
}