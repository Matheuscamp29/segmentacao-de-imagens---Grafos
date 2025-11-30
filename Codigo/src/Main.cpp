#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <chrono>
#include <ctime>

// Seus includes
#include "../include/Edmonds.h"
#include "../include/Kruskal.h"
#include "converter_arestas.cpp"
#include "../include/Segmentacao.h"
#include "../include/Imagem.h"
#include "../include/Grafo.h"

#include "../../Vendor/stb_image_write.h"

using namespace std;

// --- Estrutura Auxiliar para Colorir os Segmentos ---
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

// --- Função que Gera a Imagem Colorida ---
// threshold: Diferença de cor máxima aceita. Se for maior que isso, corta a aresta e separa os objetos.
void salvarSegmentacao(int width, int height, const vector<ArestaSimples> &arestasMST, string nomeArquivo, double threshold)
{
    UnionFindColor uf(width * height);

    // 1. Reconecta os componentes usando APENAS as arestas da MST que respeitam o threshold
    for (const auto &a : arestasMST)
    {
        if (a.peso <= threshold)
        {
            uf.unite(a.u, a.v);
        }
        // Se a.peso > threshold, a aresta é ignorada, criando uma "separação" na imagem
    }

    // 2. Gera cores aleatórias para cada componente (segmento)
    map<int, vector<unsigned char>> cores;
    srand(time(0));

    vector<unsigned char> buffer(width * height * 3); // Buffer RGB

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int id = y * width + x;
            int root = uf.find(id);

            // Se esse segmento ainda não tem cor, sorteia uma
            if (cores.find(root) == cores.end())
            {
                cores[root] = {
                    (unsigned char)(rand() % 256),
                    (unsigned char)(rand() % 256),
                    (unsigned char)(rand() % 256)};
            }

            // Pinta o pixel no buffer
            int index = (y * width + x) * 3;
            buffer[index] = cores[root][0];     // R
            buffer[index + 1] = cores[root][1]; // G
            buffer[index + 2] = cores[root][2]; // B
        }
    }

    // 3. Salva o arquivo
    stbi_write_png(nomeArquivo.c_str(), width, height, 3, buffer.data(), width * 3);
    cout << "Imagem segmentada salva em: " << nomeArquivo << " (Threshold: " << threshold << ")" << endl;
}

void rodarEdmonds(Imagem &img)
{
    auto inicio = chrono::high_resolution_clock::now();

    Grafo *g = new Grafo(true);
    converterImagemParaGrafo(img, *g);

    vector<Edge> edges = converterParaArestas(g);

    int maxId = 0;
    for (const auto &e : edges)
        if (e.id > maxId)
            maxId = e.id;
    vector<Edge> mapaArestas(maxId + 1);
    for (const auto &e : edges)
        mapaArestas[e.id] = e;

    int raiz = 0;
    int N = g->listaDeVertices.size();

    cout << "[EDMONDS] Executando..." << endl;
    vector<int> idsEscolhidos = edmonds(raiz, N, edges);

    double custoTotal = 0;
    for (int id : idsEscolhidos)
    {
        custoTotal += mapaArestas[id].w;
    }

    cout << "--- Resultado Edmonds ---" << endl;
    cout << "Custo Total: " << custoTotal << endl;

    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();

    cout << "Tempo de execucao (Endmonds): " << tempo << " segundos" << endl;

    delete g;
}

void rodarKruskal(Imagem &img)
{
    auto inicio = chrono::high_resolution_clock::now();

    cout << "\n[KRUSKAL] Iniciando..." << endl;

    Grafo *g = new Grafo(false);
    converterImagemParaGrafo(img, *g);

    ResultadoKruskal resultado = executarKruskal(*g);

    cout << "--- Resultado Kruskal ---" << endl;
    cout << "Custo Total MST: " << resultado.custoTotal << endl;
    cout << "Arestas na MST: " << resultado.arestasEscolhidas.size() << endl;

    double threshold = 10.0;

    salvarSegmentacao(img.getLargura(), img.getAltura(), resultado.arestasEscolhidas, "../resultado_segmentacao.png", threshold);

    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();

    cout << "Tempo de execucao (Kruskal): " << tempo << " segundos" << endl;
    delete g;
}

int main()
{
    string path = "../image2.jpg";
    Imagem img;

    if (img.carregar(path))
    {
        cout << "Imagem carregada: " << img.getLargura() << "x" << img.getAltura() << endl;
    }
    else
    {
        cout << "Falha ao carregar imagem.\n";
        return 1;
    }

    img.aplicarBlur(&img, 5);

    if (img.salvar("../img_blur.png")) {
        std::cout << "Sucesso! Verifique o arquivo 'saida_blur.png' na pasta do projeto." << std::endl;
    } else {
        std::cerr << "Erro ao salvar a imagem." << std::endl;
    }   

    int opcao = 0;
    do
    {
        cout << "\n================ MENU ================" << endl;
        cout << "1 - Edmonds (Arborescencia)" << endl;
        cout << "2 - Kruskal (Segmentacao)" << endl;
        cout << "0 - Sair" << endl;
        cout << "Escolha: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            rodarEdmonds(img);
            break;

        case 2:
            rodarKruskal(img);
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