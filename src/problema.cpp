#include "../include/problema.h"
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <random>

/* Calcula a distância euclidiana entre dois pontos */
double calcularDistancia(const Ponto &a, const Ponto &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return std::sqrt(dx * dx + dy * dy);
}

/* Soma os trechos da rota, incluindo a volta ao ponto inicial */
double calcularDistanciaRota(const std::vector<Ponto> &pontos, const std::vector<int> &rota)
{
    if (rota.empty())
    {
        return 0.0;
    }

    double total = 0.0;

    for (std::size_t i = 0; i < rota.size(); i++)
    {
        int atual = rota[i];
        /* O módulo faz a última posição se conectar à primeira */
        int proximo = rota[(i + 1) % rota.size()];

        total += calcularDistancia(pontos.at(atual), pontos.at(proximo));
    }

    return total;
}

/* Atribui maior aptidão às rotas menores, evitando divisão por zero */
double calcularAptidao(const std::vector<Ponto> &pontos, const std::vector<int> &rota)
{
    double distancia = calcularDistanciaRota(pontos, rota);

    return 1.0 / (1.0 + distancia);
}

/* Gera pontos igualmente espaçados em um círculo centrado em (0, 0) */
std::vector<Ponto> gerarPontosCirculares(int quantidade, double raio)
{
    /*  Garante a quantidade mínima exigida pelo trabalho */
    if (quantidade < 8)
    {
        throw std::invalid_argument("Use pelo menos 8 pontos.");
    }

    if (!std::isfinite(raio) || raio <= 0.0)
    {
        throw std::invalid_argument("O raio deve ser positivo e finito.");
    }

    const double pi = std::acos(-1.0);
    std::vector<Ponto> pontos;
    pontos.reserve(quantidade);

    for (int i = 0; i < quantidade; i++)
    {
        /* Divide a volta completa em ângulos igualmente espaçados */
        double angulo = 2.0 * pi * i / quantidade;

        /* Converte o ângulo nas coordenadas do ponto*/
        Ponto ponto =
            {
                raio * std::cos(angulo), raio * std::sin(angulo)};

        pontos.push_back(ponto);
    }

    return pontos;
}

/* Sorteia coordenadas uniformes em uma área quadrada de 0 a limite */
std::vector<Ponto> gerarPontosUniformes(int quantidade, double limite, unsigned int semente)
{
    if (quantidade < 8)
    {
        throw std::invalid_argument("Use pelo menos 8 pontos.");
    }

    if (!std::isfinite(limite) || limite <= 0.0)
    {
        throw std::invalid_argument("O limite deve ser positivo e finito.");
    }

    /* A semente permite repetir o sorteio usando a mesma implementação */
    std::mt19937 gerador(semente);
    std::uniform_real_distribution<double> distribuicao(0.0, limite);

    std::vector<Ponto> pontos;
    pontos.reserve(quantidade);

    for (int i = 0; i < quantidade; i++)
    {
        /* Sorteia separadamente as coordenadas x e y */
        Ponto ponto =
            {
                distribuicao(gerador), distribuicao(gerador)};

        pontos.push_back(ponto);
    }

    return pontos;
}