#include <iostream>
#include <vector>
#include <map>
#include <cstdlib> // rand
#include <ctime>   // time

#include "../include/Imagem.h"
#include "../include/Grafo.h"
#include "../include/Segmentacao.h"
#include "../include/Kruskal.h"
#include "../include/Edmonds.h"

// Inclui a biblioteca de escrita apenas como cabeçalho (a implementação está no Imagem.cpp)
#include "../vendor/stb_image_write.h"

using namespace std;

// Estrutura Union-Find simples para ajudar a colorir os segmentos
struct UnionFindColor
{
    vector<int> pai;
    UnionFindColor(int n)
    {
        pai.resize(n);
        for (int i = 0; i < n; i++)
            pai[i] = i;
    }
    int find(int i)
    {
        if (pai[i] == i)
            return i;
        return pai[i] = find(pai[i]);
    }
    void unite(int i, int j)
    {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j)
            pai[root_i] = root_j;
    }
};

// Função para gerar a imagem segmentada
// threshold: Diferença máxima de cor para considerar parte do mesmo segmento
void salvarSegmentacao(int width, int height, const vector<ArestaSimples> &arestas, string nomeArquivo, double threshold)
{
    UnionFindColor uf(width * height);

    // 1. Formar os componentes conexos (Segmentos)
    // Só unimos se a aresta foi escolhida pelo Kruskal E se o peso for baixo (similaridade)
    for (const auto &a : arestas)
    {
        if (a.peso <= threshold)
        {
            uf.unite(a.u, a.v);
        }
    }

    // 2. Atribuir cores aleatórias para cada segmento raiz
    map<int, vector<unsigned char>> cores;
    srand(time(0));

    vector<unsigned char> buffer(width * height * 3); // Buffer RGB

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int id = y * width + x;
            int root = uf.find(id);

            // Se essa raiz (segmento) ainda não tem cor, cria uma aleatória
            if (cores.find(root) == cores.end())
            {
                cores[root] = {
                    (unsigned char)(rand() % 256),
                    (unsigned char)(rand() % 256),
                    (unsigned char)(rand() % 256)};
            }

            // Pinta o pixel com a cor do segmento
            int index = (y * width + x) * 3;
            buffer[index] = cores[root][0];     // R
            buffer[index + 1] = cores[root][1]; // G
            buffer[index + 2] = cores[root][2]; // B
        }
    }

    // 3. Salvar no disco
    stbi_write_png(nomeArquivo.c_str(), width, height, 3, buffer.data(), width * 3);
    cout << "Segmentacao salva em: " << nomeArquivo << endl;
}

int main()
{
    string path = "./imagem.jpg"; // Certifique-se que image.jpg está na pasta raiz onde você roda o executável
    Imagem img;

    if (!img.carregar(path))
    {
        cout << "Erro ao carregar imagem (verifique o caminho!).\n";
        return 1;
    }

    // --- KRUSKAL (Segmentação) ---
    cout << "\n--- Rodando Kruskal ---\n";
    Grafo gKruskal(false);
    converterImagemParaGrafo(img, gKruskal);

    ResultadoKruskal resK = executarKruskal(gKruskal);
    cout << "Custo MST: " << resK.custoTotal << endl;

    // AQUI: Gerar a imagem visual
    // Threshold = 30.0 (exemplo): Arestas com diferença de cor > 30 serão cortadas, separando os objetos.
    // Teste valores diferentes (10, 50, 100) para ver o resultado mudar.
    salvarSegmentacao(img.getLargura(), img.getAltura(), resK.arestasEscolhidas, "saida_kruskal.png", 30.0);

    // --- EDMONDS ---
    cout << "\n--- Rodando Edmonds ---\n";
    Grafo gEdmonds(true);
    converterImagemParaGrafo(img, gEdmonds);

    // O Edmonds é mais complexo de visualizar como segmentação simples de regiões,
    // geralmente ele é usado para encontrar estruturas de fluxo ou hierarquia.
    // Vamos apenas rodar o cálculo por enquanto.
    int raiz = 0;
    double custoE = executarEdmonds(gEdmonds, raiz);

    if (custoE == -1)
        cout << "Nao foi possivel gerar arborescencia.\n";
    else
        cout << "Custo Arborescencia: " << custoE << endl;

    return 0;
}