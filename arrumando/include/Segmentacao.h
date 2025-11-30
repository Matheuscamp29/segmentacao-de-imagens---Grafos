#ifndef SEGMENTACAO_H
#define SEGMENTACAO_H

#include "Imagem.h"
#include "Grafo.h"

// Declaração da função (o contrato)
// O Main.cpp vai ler isso e saber que pode chamar essa função.
void converterImagemParaGrafo(const Imagem &img, Grafo &grafo);

#endif