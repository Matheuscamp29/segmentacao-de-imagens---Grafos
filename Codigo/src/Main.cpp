#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <chrono>
#include <ctime>

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

// --- Função Original: Cores Aleatórias ---
void salvarSegmentacaoMapa(int width, int height, const vector<ArestaSimples> &arestasMST, string nomeArquivo, double threshold)
{
    UnionFindColor uf(width * height);

    for (const auto &a : arestasMST)
    {
        if (a.peso <= threshold)
        {
            uf.unite(a.u, a.v);
        }
    }

    map<int, vector<unsigned char>> cores;
    srand(time(0));

    vector<unsigned char> buffer(width * height * 3);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int id = y * width + x;
            int root = uf.find(id);

            if (cores.find(root) == cores.end())
            {
                cores[root] = {
                    (unsigned char)(rand() % 256),
                    (unsigned char)(rand() % 256),
                    (unsigned char)(rand() % 256)};
            }

            int index = (y * width + x) * 3;
            buffer[index] = cores[root][0];     // R
            buffer[index + 1] = cores[root][1]; // G
            buffer[index + 2] = cores[root][2]; // B
        }
    }

    stbi_write_png(nomeArquivo.c_str(), width, height, 3, buffer.data(), width * 3);
    cout << "Imagem (MAPA) salva em: " << nomeArquivo << endl;
}

// --- Nova Função: Cores Reais ---
void salvarSegmentacaoResultado(const Imagem &img, const vector<ArestaSimples> &arestasMST, string nomeArquivo, double threshold)
{
    int width = img.getLargura();
    int height = img.getAltura();
    UnionFindColor uf(width * height);

    for (const auto &a : arestasMST)
    {
        if (a.peso <= threshold)
        {
            uf.unite(a.u, a.v);
        }
    }

    // Mapeia ID_Raiz -> {SomaR, SomaG, SomaB, Contador}
    map<int, vector<unsigned long long>> dadosCores;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int id = y * width + x;
            int root = uf.find(id);
            unsigned char *pixel = img.getPixel(x, y);

            if (dadosCores.find(root) == dadosCores.end())
            {
                dadosCores[root] = {0, 0, 0, 0};
            }

            dadosCores[root][0] += pixel[0];
            dadosCores[root][1] += pixel[1];
            dadosCores[root][2] += pixel[2];
            dadosCores[root][3]++;
        }
    }

    vector<unsigned char> buffer(width * height * 3);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int id = y * width + x;
            int root = uf.find(id);

            unsigned long long count = dadosCores[root][3];
            unsigned long long r = dadosCores[root][0];
            unsigned long long g = dadosCores[root][1];
            unsigned long long b = dadosCores[root][2];

            int index = (y * width + x) * 3;
            buffer[index] = (unsigned char)(r / count);
            buffer[index + 1] = (unsigned char)(g / count);
            buffer[index + 2] = (unsigned char)(b / count);
        }
    }

    stbi_write_png(nomeArquivo.c_str(), width, height, 3, buffer.data(), width * 3);
    cout << "Imagem (RESULTADO) salva em: " << nomeArquivo << endl;
}

void rodarEdmonds(Imagem &img, double threshold)
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

    vector<ArestaSimples> arestasVis;
    double custoTotal = 0;
    for (int id : idsEscolhidos)
    {
        Edge original = mapaArestas[id];
        custoTotal += original.w;
        // Converte Edge -> ArestaSimples para poder usar a função de salvar
        arestasVis.push_back({original.u, original.v, original.w});
    }

    cout << "--- Resultado Edmonds ---" << endl;
    cout << "Custo Total: " << custoTotal << endl;

    // Salva as duas versões usando o threshold passado
    salvarSegmentacaoMapa(img.getLargura(), img.getAltura(), arestasVis, "../edmonds_mapa.png", threshold);
    salvarSegmentacaoResultado(img, arestasVis, "../edmonds_resultado.png", threshold);

    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();

    cout << "Tempo de execucao (Edmonds): " << tempo << " segundos" << endl;

    delete g;
}

void rodarKruskal(Imagem &img, double threshold)
{
    auto inicio = chrono::high_resolution_clock::now();

    cout << "\n[KRUSKAL] Iniciando..." << endl;

    Grafo *g = new Grafo(false);
    converterImagemParaGrafo(img, *g);

    ResultadoKruskal resultado = executarKruskal(*g);

    cout << "--- Resultado Kruskal ---" << endl;
    cout << "Custo Total MST: " << resultado.custoTotal << endl;
    cout << "Arestas na MST: " << resultado.arestasEscolhidas.size() << endl;

    // Gera as duas imagens
    salvarSegmentacaoMapa(img.getLargura(), img.getAltura(), resultado.arestasEscolhidas, "../kruskal_mapa.png", threshold);
    salvarSegmentacaoResultado(img, resultado.arestasEscolhidas, "../kruskal_resultado.png", threshold);

    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();

    cout << "Tempo de execucao (Kruskal): " << tempo << " segundos" << endl;
    delete g;
}

int main()
{
    string path = "./image2.jpg";
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

    int n = 0;
    cout << "Insira um valor para o numero de blurs" << endl;
    cin >> n;
    img.aplicarBlur(&img, n);

    if (img.salvar("../img_blur.png"))
    {
        std::cout << "Sucesso! Verifique o arquivo 'img_blur.png' na pasta do projeto." << std::endl;
    }
    else
    {
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
        {
            int threshold = 0;
            cout << "Insira um valor para o threshold (Edmonds)" << endl;
            cin >> threshold;
            rodarEdmonds(img, threshold);
            break;
        }
        case 2:
        {
            int threshold = 0;
            cout << "Insira um valor para o threshold (Kruskal)" << endl;
            cin >> threshold;
            rodarKruskal(img, threshold);
            break;
        }
        case 0:
            cout << "Saindo..." << endl;
            break;

        default:
            cout << "Opcao invalida!" << endl;
        }

    } while (opcao != 0);

    return 0;
}