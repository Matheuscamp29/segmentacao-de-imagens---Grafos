#ifndef IMAGEM_H
#define IMAGEM_H

#include <string>

class Imagem
{
public:
    Imagem();
    ~Imagem();

    
    bool carregar(const std::string &caminho);

 
    bool salvar(const std::string &caminho);


    int getLargura() const;
    int getAltura() const;
    int getCanais() const;

    
    unsigned char *getData() const;

    void blur();

    unsigned char *getPixel(int x, int y) const;

private:
    int largura;
    int altura;
    int canais;           // 3 para RGB, 1 para Cinza
    unsigned char *dados; // O array de bytes da imagem
};

#endif