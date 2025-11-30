#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Imagem.h"
#include "../include/Grafo.h"



void converterImagemParaGrafo(const Imagem& img, Grafo& grafo) {
    int largura = img.getLargura();
    int altura = img.getAltura();
    int idVertice = 0;
    std::vector<std::vector<Vertice*>> mapaVertices(altura, std::vector<Vertice*>(largura));
    
   
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            
            unsigned char* pixel = img.getPixel(x, y);
            Vertice* v = new Vertice(idVertice++, x,  y, (int)pixel[0], (int)pixel[1], (int)pixel[2]);

            grafo.adicionarVertice(v);
            mapaVertices[y][x] = v;
        }
    }

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            Vertice* atual = mapaVertices[y][x];

            if (x + 1 < largura) {
                Vertice* direita = mapaVertices[y][x + 1];
                grafo.inserirAresta(atual, direita);
            }

            if (y + 1 < altura) {
                Vertice* baixo = mapaVertices[y + 1][x];
                grafo.inserirAresta(atual, baixo);
            }

            if (grafo.direcionado == 1 && x - 1 >= 0){
                Vertice* esquerda = mapaVertices[y][x-1];
                grafo.inserirAresta(atual, esquerda);
            }

            if (grafo.direcionado == 1 && y - 1 >= 0){
                Vertice* cima = mapaVertices[y-1][x];
                grafo.inserirAresta(atual, cima);
            }

            /* if (x + 1 < largura && y + 1 < altura){
                Vertice* dbaixo = mapaVertices[y+1][x+1];
                grafo.inserirAresta(atual, dbaixo, idAresta++);
            }

            if (x - 1 >= 0 && y + 1 < altura){
                Vertice* ebaixo = mapaVertices[y+1][x-1];
                grafo.inserirAresta(atual, ebaixo, idAresta++);
            }

            if (grafo.direcionado == 1 && x + 1 < largura && y - 1 >= 0){
                Vertice* dcima = mapaVertices[y-1][x+1];
                grafo.inserirAresta(atual, dcima, idAresta++);
            }

            if (grafo.direcionado == 1 && x - 1 >= 0 && y - 1 >= 0){
                Vertice* ecima = mapaVertices[y-1][x-1];
                grafo.inserirAresta(atual, ecima, idAresta++);
            }
                */
        } 
    }
    
    std::cout << "Grafo gerado com " << grafo.listaDeVertices.size() << " vertices." << std::endl;
}

