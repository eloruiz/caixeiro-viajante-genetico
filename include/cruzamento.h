#ifndef CRUZAMENTO_H
#define CRUZAMENTO_H

#include <random>
#include <vector>

/* Uma rota é uma permutação: rota[i] é o índice do ponto visitado na posição i */

/* Confere se a rota visita cada um dos n pontos (0 a n-1) exatamente uma vez */
bool rotaValida(const std::vector<int> &rota, int n);

/* Cruzamento OX (Order Crossover): gera um filho válido a partir de dois pais */
std::vector<int> cruzamentoOX(const std::vector<int> &pai1, const std::vector<int> &pai2, std::mt19937 &gerador);

#endif
