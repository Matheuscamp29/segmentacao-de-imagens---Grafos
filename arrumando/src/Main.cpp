#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "../include/Imagem.h"
#include "../include/Grafo.h"
#include "../include/Segmentacao.h"
#include "../include/Kruskal.h"
#include "../include/Edmonds.h"
#include "../Vendor/stb_image_write.h"

using namespace std;

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

template <typename TipoAresta>
void salvarSegmentacao(int width, int height, const vector<TipoAresta> &arestas, string nomeArquivo, double threshold)
{
    UnionFindColor uf(width * height);
    for (const auto &a : arestas)
    {
        if (a.peso <= threshold)
            uf.unite(a.u, a.v);
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
                cores[root] = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256)};
            }
            int idx = (y * width + x) * 3;
            buffer[idx] = cores[root][0];
            buffer[idx + 1] = cores[root][1];
            buffer[idx + 2] = cores[root][2];
        }
    }
    stbi_write_png(nomeArquivo.c_str(), width, height, 3, buffer.data(), width * 3);
    cout << "Imagem salva: " << nomeArquivo << endl;
}

vector<ArestaEdmonds> gerarArestasDireto(const Imagem &img)
{
    int w = img.getLargura();
    int h = img.getAltura();
    vector<ArestaEdmonds> arestas;
    arestas.reserve(w * h * 4);

    int id_count = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int u = y * w + x;
            unsigned char *pixU = img.getPixel(x, y);

            int dx[] = {1, 0, -1, 0};
            int dy[] = {0, 1, 0, -1};

            for (int k = 0; k < 4; k++)
            {
                int nx = x + dx[k];
                int ny = y + dy[k];

                if (nx >= 0 && nx < w && ny >= 0 && ny < h)
                {
                    int v = ny * w + nx;
                    unsigned char *pixV = img.getPixel(nx, ny);

                    float dist = sqrt(pow(pixU[0] - pixV[0], 2) +
                                      pow(pixU[1] - pixV[1], 2) +
                                      pow(pixU[2] - pixV[2], 2));

                    arestas.push_back({u, v, dist, id_count++});
                }
            }
        }
    }
    return arestas;
}

int main()
{
    string path = "./image.jpg";
    Imagem img;

    if (!img.carregar(path))
        return 1;

    cout << "\n--- Rodando Kruskal ---\n";
    {
        Grafo gKruskal(false);
        converterImagemParaGrafo(img, gKruskal);
        ResultadoKruskal resK = executarKruskal(gKruskal);
        cout << "Custo MST: " << resK.custoTotal << endl;
        salvarSegmentacao(img.getLargura(), img.getAltura(), resK.arestasEscolhidas, "saida_kruskal.png", 10.0);
    }

    /*
    // --- EDMONDS (Versão Otimizada Direta) ---
    cout << "\n--- Rodando Edmonds ---\n";

    // 1. Gera vetor de arestas leves direto da imagem
    vector<ArestaEdmonds> arestasEdmonds = gerarArestasDireto(img);
    cout << "Arestas geradas: " << arestasEdmonds.size() << endl;

    // 2. Executa algoritmo
    ResultadoEdmonds resE = executarEdmondsDireto(img.getLargura() * img.getAltura(), arestasEdmonds, 0);

    if (resE.custoTotal == -1)
        cout << "Erro: Arborescencia impossivel.\n";
    else
    {
        cout << "Custo Arborescencia: " << resE.custoTotal << endl;
        salvarSegmentacao(img.getLargura(), img.getAltura(), resE.arestas, "saida_edmonds.png", 150.0);
    }
    */

    return 0;
}