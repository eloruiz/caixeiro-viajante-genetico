#include "../include/evolucao.h"
#include "../include/cruzamento.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>

namespace
{
struct Individuo
{
    std::vector<int> rota;
    double distancia;
};

void validar(const std::vector<Ponto> &pontos, const ParametrosEvolucao &p)
{
    if (pontos.size() < 8)
        throw std::invalid_argument("Use pelo menos 8 pontos.");
    for (const auto &ponto : pontos)
        if (!std::isfinite(ponto.x) || !std::isfinite(ponto.y))
            throw std::invalid_argument("As coordenadas devem ser finitas.");
    if (p.tamanhoPopulacao < 2 || p.tamanhoTorneio < 1 ||
        p.tamanhoTorneio > p.tamanhoPopulacao || p.quantidadeElite < 1 ||
        p.quantidadeElite >= p.tamanhoPopulacao || p.maxGeracoes < 0 ||
        p.limiteSemMelhora < 0)
        throw std::invalid_argument("Parametros de populacao ou parada invalidos.");
    if (!std::isfinite(p.taxaMutacao) || p.taxaMutacao < 0 || p.taxaMutacao > 1 ||
        !std::isfinite(p.taxaCruzamento) || p.taxaCruzamento < 0 || p.taxaCruzamento > 1)
        throw std::invalid_argument("As taxas devem estar entre 0 e 1.");
}

Individuo avaliar(const std::vector<Ponto> &pontos, std::vector<int> rota)
{
    double distancia = calcularDistanciaRota(pontos, rota);
    if (!std::isfinite(distancia))
        throw std::invalid_argument("A distancia excedeu o intervalo numerico.");
    return {std::move(rota), distancia};
}

const Individuo &selecionar(const std::vector<Individuo> &populacao,
                           int tamanho, std::mt19937 &gerador)
{
    // Torneio com reposicao. Menor distancia equivale a maior aptidao.
    std::uniform_int_distribution<int> sorteio(0, static_cast<int>(populacao.size()) - 1);
    int melhor = sorteio(gerador);
    for (int i = 1; i < tamanho; ++i)
    {
        int candidato = sorteio(gerador);
        if (populacao[candidato].distancia < populacao[melhor].distancia)
            melhor = candidato;
    }
    return populacao[melhor];
}
}

ResultadoEvolucao executarEvolucao(const std::vector<Ponto> &pontos,
                                 const ParametrosEvolucao &p)
{
    validar(pontos, p);
    std::mt19937 gerador(p.semente);
    std::bernoulli_distribution cruzar(p.taxaCruzamento), mutar(p.taxaMutacao);
    std::uniform_int_distribution<int> posicao(0, static_cast<int>(pontos.size()) - 1);
    std::uniform_int_distribution<int> outraPosicao(0, static_cast<int>(pontos.size()) - 2);
    std::vector<int> rota(pontos.size());
    std::iota(rota.begin(), rota.end(), 0);
    std::vector<Individuo> populacao;
    for (int i = 0; i < p.tamanhoPopulacao; ++i)
    {
        std::shuffle(rota.begin(), rota.end(), gerador);
        populacao.push_back(avaliar(pontos, rota));
    }

    ResultadoEvolucao resultado;
    resultado.motivoParada = MotivoParada::limiteGeracoes;
    auto registrar = [&](int geracao)
    {
        std::stable_sort(populacao.begin(), populacao.end(),
                         [](const Individuo &a, const Individuo &b) { return a.distancia < b.distancia; });
        double media = 0;
        for (const auto &individuo : populacao)
            media += individuo.distancia / p.tamanhoPopulacao;
        resultado.historico.push_back({geracao, populacao.front().distancia,
                                      media, populacao.front().rota});
    };
    registrar(0);
    int semMelhora = 0;
    for (int geracao = 1; geracao <= p.maxGeracoes; ++geracao)
    {
        double anterior = populacao.front().distancia;
        // A elite e copiada sem cruzamento nem mutacao.
        std::vector<Individuo> proxima(populacao.begin(), populacao.begin() + p.quantidadeElite);
        while (static_cast<int>(proxima.size()) < p.tamanhoPopulacao)
        {
            const auto &pai1 = selecionar(populacao, p.tamanhoTorneio, gerador);
            const auto &pai2 = selecionar(populacao, p.tamanhoTorneio, gerador);
            auto filho = cruzar(gerador) ? cruzamentoOX(pai1.rota, pai2.rota, gerador) : pai1.rota;
            if (mutar(gerador))
            {
                int a = posicao(gerador);
                int b = outraPosicao(gerador);
                if (b >= a) ++b; // Garante duas posicoes diferentes.
                std::swap(filho[a], filho[b]);
            }
            proxima.push_back(avaliar(pontos, std::move(filho)));
        }
        populacao = std::move(proxima);
        registrar(geracao);
        semMelhora = populacao.front().distancia < anterior ? 0 : semMelhora + 1;
        if (p.limiteSemMelhora > 0 && semMelhora >= p.limiteSemMelhora)
        {
            resultado.motivoParada = MotivoParada::estagnacao;
            break;
        }
    }
    resultado.melhorRota = populacao.front().rota;
    resultado.melhorDistancia = populacao.front().distancia;
    return resultado;
}
