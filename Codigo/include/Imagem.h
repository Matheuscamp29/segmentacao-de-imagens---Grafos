#ifndef IMAGEM_H
#define IMAGEM_H

#include <string>

class Imagem
{
public:
    Imagem();
    ~Imagem();

    // Carrega uma imagem do disco
    bool carregar(const std::string &caminho);

    // Salva a imagem processada no disco
    bool salvar(const std::string &caminho);

    // Getters
    int getLargura() const;
    int getAltura() const;
    int getCanais() const;

    // Retorna o ponteiro direto para os dados (array de pixels)
    unsigned char *getData() const;

    // Retorna o ponteiro para um pixel específico (x, y)
    unsigned char *getPixel(int x, int y) const;

    void blur(Imagem *img);

    void aplicarBlur(Imagem *img, int n);

private:
    int largura;
    int altura;
    int canais;           // 3 para RGB, 1 para Cinza
    unsigned char *dados; // O array de bytes da imagem
};

#endif