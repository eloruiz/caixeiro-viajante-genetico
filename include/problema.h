#ifndef PROBLEMA_H
#define PROBLEMA_H

#include <vector>

struct Ponto /* representa um ponto no plano cartesiano */
{
    double x;
    double y;
};
/* Calcula a distância euclidiana entre dois pontos */
double calcularDistancia(const Ponto &a, const Ponto &b);

/* Calcula a distância da rota completa, incluindo a volta ao início */
double calcularDistanciaRota(const std::vector<Ponto> &pontos, const std::vector<int> &rota);

/* Calcula a aptidão: 1 / (1 + distância), sendo maior para rotas menores */
double calcularAptidao(const std::vector<Ponto> &pontos, const std::vector<int> &rota);

/* Gera pontos igualmente espaçados em um círculo centrado na origem */
std::vector<Ponto> gerarPontosCirculares(int quantidade, double raio);

/* Gera pontos em uma área quadrada, usando uma semente para reproduzir o sorteio */
std::vector<Ponto> gerarPontosUniformes(int quantidade, double limite, unsigned int semente);

#endif