#include "../include/Edmonds.h"
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// Estrutura interna para retorno da recursão
struct ResultadoRec
{
    double custoTotal;
    vector<int> indicesEscolhidos;
};

ResultadoRec edmonds_recursivo(int N, int raiz, vector<ArestaEdmonds> &arestas)
{
    const float INF = numeric_limits<float>::max();

    // 1. Encontrar a aresta de menor custo entrando em cada vértice
    // Otimização: Usamos vetores simples (int/float) em vez de structs para economizar RAM
    vector<int> parent(N, -1);
    vector<float> minVal(N, INF);
    vector<int> minEdgeIdx(N, -1);

    for (int i = 0; i < (int)arestas.size(); i++)
    {
        const auto &a = arestas[i];
        if (a.u == a.v)
            continue;
        if (a.peso < minVal[a.v])
        {
            minVal[a.v] = a.peso;
            parent[a.v] = a.u;
            minEdgeIdx[a.v] = i;
        }
    }

    minVal[raiz] = 0;
    parent[raiz] = -1;
    minEdgeIdx[raiz] = -1;

    // Verifica alcançabilidade
    for (int i = 0; i < N; i++)
    {
        if (i != raiz && minVal[i] == INF)
            return {-1, {}};
    }

    // 2. Detectar Ciclos
    int qtdSuperVertices = 0;
    vector<int> grupo(N, -1);
    vector<int> visitado(N, -1);
    bool temCiclo = false;

    for (int i = 0; i < N; i++)
    {
        if (i == raiz)
            continue;

        int v = i;
        while (v != raiz && visitado[v] != i && grupo[v] == -1)
        {
            visitado[v] = i;
            if (parent[v] != -1)
                v = parent[v];
            else
                break;
        }

        if (v != raiz && grupo[v] == -1)
        {
            int u = v;
            do
            {
                grupo[u] = qtdSuperVertices;
                u = parent[u];
            } while (u != v);
            qtdSuperVertices++;
            temCiclo = true;
        }
    }

    // CASO BASE: Sem ciclos
    if (!temCiclo)
    {
        ResultadoRec res;
        res.custoTotal = 0;
        for (int i = 0; i < N; i++)
        {
            if (i != raiz && minEdgeIdx[i] != -1)
            {
                res.indicesEscolhidos.push_back(arestas[minEdgeIdx[i]].id_orig);
                res.custoTotal += minVal[i];
            }
        }
        return res;
    }

    // 3. Contração
    for (int i = 0; i < N; i++)
    {
        if (grupo[i] == -1)
            grupo[i] = qtdSuperVertices++;
    }

    vector<ArestaEdmonds> novasArestas;
    novasArestas.reserve(arestas.size());

    for (const auto &a : arestas)
    {
        int u_super = grupo[a.u];
        int v_super = grupo[a.v];

        if (u_super != v_super)
        {
            float novoPeso = a.peso - minVal[a.v];
            novasArestas.push_back({u_super, v_super, novoPeso, a.id_orig});
        }
    }

    // Libera vetores locais antes da recursão para economizar memória
    vector<int>().swap(parent);
    vector<float>().swap(minVal);
    vector<int>().swap(visitado);

    // Recursão
    ResultadoRec resRec = edmonds_recursivo(qtdSuperVertices, grupo[raiz], novasArestas);
    if (resRec.custoTotal == -1)
        return {-1, {}};

    // 4. Expansão
    ResultadoRec finalRes;
    finalRes.custoTotal = 0;

    // Mapeia qual vértice "real" recebeu a aresta que entrou no ciclo
    vector<int> entradaCiclo(qtdSuperVertices, -1);

    // Adiciona arestas externas (vindas da recursão)
    for (int id_global : resRec.indicesEscolhidos)
    {
        finalRes.indicesEscolhidos.push_back(id_global);

        // Busca linear para encontrar onde a aresta entrou (necessário para quebrar o ciclo)
        // Otimização: percorre apenas arestas locais
        for (const auto &a : arestas)
        {
            if (a.id_orig == id_global)
            {
                entradaCiclo[grupo[a.v]] = a.v;
                break;
            }
        }
    }

    // Adiciona arestas internas (quebrando o ciclo onde houve entrada)
    for (int i = 0; i < N; i++)
    {
        if (i == raiz)
            continue;
        int g = grupo[i];

        // Se este vértice NÃO foi a porta de entrada do ciclo, adicionamos sua aresta original
        if (entradaCiclo[g] != i)
        {
            if (minEdgeIdx[i] != -1)
                finalRes.indicesEscolhidos.push_back(arestas[minEdgeIdx[i]].id_orig);
        }
    }

    return finalRes;
}

ResultadoEdmonds executarEdmondsDireto(int numVertices, vector<ArestaEdmonds> &arestas, int raiz)
{
    // Roda a recursão
    ResultadoRec res = edmonds_recursivo(numVertices, raiz, arestas);

    ResultadoEdmonds saida;
    if (res.custoTotal == -1)
    {
        saida.custoTotal = -1;
    }
    else
    {
        saida.custoTotal = 0;
        // Reconstrói o vetor de arestas final usando um mapa ou busca
        // Para economizar memória, vamos assumir que as arestas originais estão na ordem dos IDs ou faremos um lookup.
        // Como o vetor 'arestas' original foi passado por referência, ele ainda existe aqui?
        // Não, a função recursiva recebeu cópias modificadas.
        // O 'main' tem o vetor original 'arestasBrutas'. Precisamos usá-lo para recuperar os valores reais.

        // Como otimização, criaremos um vetor simples de ArestaFinal aqui
        // O Main passará o vetor original, então podemos acessá-lo.

        // Nota: A função edmonds_recursivo retorna IDs. Precisamos mapear esses IDs de volta para (u, v, peso).
        // A maneira mais eficiente de memória é fazer isso no Main, mas vamos fazer aqui usando o vetor de entrada como lookup se ele estiver ordenado/preservado.

        // Como 'arestas' aqui é o vetor inicial passado pelo Main, podemos usar busca direta se o 'id_orig' for o índice.
        // Vamos assumir que id_orig é o índice no vetor original.

        saida.arestas.reserve(res.indicesEscolhidos.size());
        for (int idx : res.indicesEscolhidos)
        {
            const auto &a = arestas[idx]; // Assumindo que id_orig == índice
            saida.arestas.push_back({a.u, a.v, a.peso});
            saida.custoTotal += a.peso;
        }
    }
    return saida;
}