// Estrutura dos grafos
#include <iostream>
#include <vector>
#include <cmath>
#include "imagem.cpp"
using namespace std;


class Vertice;

class Aresta{
    public:
        int id;
        Vertice* destino;
        double peso;

    Aresta(int id, Vertice* destino, int peso){
        this->id = id;
        this->destino = destino;
        this->peso = peso;
    }
};

class Vertice{
    public:
        int r, g, b;
        int x, y;
        vector<Aresta> vizinhos;
        
        Vertice(int x, int y, int r, int g, int b){
            this->r = r;
            this->g = g;
            this->b = b;
            this->y = y;
            this->x = x;
        }
        
        void adicionarAresta(Vertice* destino, int id){
            double diferenca = std::sqrt(
                std::pow(this->r - destino->r, 2) +
                std::pow(this->g - destino->g, 2) +
                std::pow(this->b - destino->b, 2)
            );
            Aresta nova (id, destino, diferenca);
            vizinhos.push_back(nova);
        }

};

class Grafo{
   public:
        vector<Vertice*> listaDeVertices;
        bool direcionado; 

        Grafo(bool direcionado){
            this->direcionado = direcionado;
        }

        void adicionarVertice(Vertice* v) {
            listaDeVertices.push_back(v);
        }

        void inserirAresta(Vertice* origem, Vertice* destino, int id) {
        
            origem->adicionarAresta(destino, id);
            if (!this->direcionado) {
                destino->adicionarAresta(origem, id);
            }
        }
    };



