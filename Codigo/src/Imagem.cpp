#include "../include/Imagem.h"
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../../Vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../Vendor/stb_image_write.h"


Imagem::Imagem() : largura(0), altura(0), canais(0), dados(nullptr) {}

Imagem::~Imagem()
{
    if (dados)
    {
        stbi_image_free(dados); // Libera a memória da imagem ao destruir o objeto
    }
}

bool Imagem::carregar(const std::string &caminho)
{
    if (dados)
    {
        stbi_image_free(dados); // Limpa imagem anterior, se houver
    }
    dados = stbi_load(caminho.c_str(), &largura, &altura, &canais, 3);

    if (!dados)
    {
        std::cerr << "Erro ao carregar a imagem: " << caminho << std::endl;
        return false;
    }
    std::cout << "Imagem carregada: " << largura << "x" << altura << " (" << canais << " canais)" << std::endl;
    return true;
}

bool Imagem::salvar(const std::string &caminho)
{
    if (!dados)
    {
        std::cerr << "Nenhum dado de imagem para salvar." << std::endl;
        return false;
    }
    return stbi_write_png(caminho.c_str(), largura, altura, canais, dados, largura * canais);
}

int Imagem::getLargura() const { return largura; }
int Imagem::getAltura() const { return altura; }
int Imagem::getCanais() const { return canais; }
unsigned char *Imagem::getData() const { return dados; }

unsigned char *Imagem::getPixel(int x, int y) const
{
    if (x < 0 || x >= largura || y < 0 || y >= altura)
    {
        return nullptr;
    }
    return dados + (y * largura + x) * canais;
}

void Imagem::blur(Imagem *img)
{
    if (!img)
        return;
    int altura = img->altura;
    int largura = img->largura;
    int canais = img->canais;
    

    std::vector<unsigned char> buffer(largura * altura * canais);

    const int Gauss[3][3] = {{1, 2, 1},
                             {2, 4, 2},
                             {1, 2, 1}};
    int soma = 16;

    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++){
            int inicioy = -1; 
            int iniciox = -1;
            int fimy = 1; 
            int fimx = 1;
            int r = 0;
            int g = 0; 
            int b = 0;

            if (y == 0) inicioy = 0;
            if (x == 0) iniciox = 0;
            if (y == altura-1) fimy = 0;
            if (x == largura-1) fimx = 0;


            for (int ky = inicioy; ky <= fimy; ky++){
                for (int kx = iniciox; kx <= fimx; kx++){
                    unsigned char* pixel = img->getPixel(x + kx, y + ky);
                    int peso = Gauss[ky+1][kx+1];

                    r += pixel[0] * peso;
                    g += pixel[1] * peso;
                    b += pixel[2] * peso;
                }
            }
            int index = (y * largura + x) * canais;
            buffer[index]     = r / soma;
            buffer[index + 1] = g / soma;
            buffer[index + 2] = b / soma;
        }
    }
    if (img->dados) {
        // Copia byte a byte do buffer para a memória da imagem
        for (size_t i = 0; i < buffer.size(); i++) {
            img->dados[i] = buffer[i];
        }
    }
}

void Imagem::aplicarBlur (Imagem* img, int n){
    for (int i = 0; i<n; i++){
        blur(img);
    }
}