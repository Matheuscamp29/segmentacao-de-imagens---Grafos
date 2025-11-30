#include "../include/Segmentacao.h"
#include "../include/Grafo.h"
#include "../include/Imagem.h"
#include <iostream>
#include <vector>

void converterImagemParaGrafo(const Imagem &img, Grafo &grafo)
{
    int largura = img.getLargura();
    int altura = img.getAltura();
    int idVertice = 0;

    std::vector<std::vector<Vertice *>> mapaVertices(altura, std::vector<Vertice *>(largura));

    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            unsigned char *pixel = img.getPixel(x, y);
            Vertice *v = new Vertice(idVertice++, x, y, (int)pixel[0], (int)pixel[1], (int)pixel[2]);
            grafo.adicionarVertice(v);
            mapaVertices[y][x] = v;
        }
    }

    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            Vertice *atual = mapaVertices[y][x];

            if (x + 1 < largura)
                grafo.inserirAresta(atual, mapaVertices[y][x + 1]);
            if (y + 1 < altura)
                grafo.inserirAresta(atual, mapaVertices[y + 1][x]);

            if (grafo.direcionado)
            {
                if (x - 1 >= 0)
                    grafo.inserirAresta(atual, mapaVertices[y][x - 1]);
                if (y - 1 >= 0)
                    grafo.inserirAresta(atual, mapaVertices[y - 1][x]);
            }
        }
    }
    std::cout << "Grafo gerado com " << grafo.listaDeVertices.size() << " vertices." << std::endl;
}