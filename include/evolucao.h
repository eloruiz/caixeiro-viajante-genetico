#ifndef EVOLUCAO_H
#define EVOLUCAO_H

#include "problema.h"
#include <vector>

struct ParametrosEvolucao
{
    int tamanhoPopulacao = 100;
    int tamanhoTorneio = 3;
    int quantidadeElite = 1;
    double taxaCruzamento = 0.9;
    double taxaMutacao = 0.1; // Probabilidade por filho, nao por cidade.
    int maxGeracoes = 1000;
    int limiteSemMelhora = 200; // Zero desativa esta parada.
    unsigned int semente = 42;
};

struct RegistroGeracao
{
    int geracao;
    double melhorDistancia;
    double distanciaMedia;
    std::vector<int> melhorRota;
};

enum class MotivoParada { limiteGeracoes, estagnacao };

struct ResultadoEvolucao
{
    std::vector<int> melhorRota;
    double melhorDistancia;
    std::vector<RegistroGeracao> historico; // Inclui a populacao inicial (geracao 0).
    MotivoParada motivoParada;
};

ResultadoEvolucao executarEvolucao(const std::vector<Ponto> &pontos,
                                 const ParametrosEvolucao &parametros = {});

#endif
