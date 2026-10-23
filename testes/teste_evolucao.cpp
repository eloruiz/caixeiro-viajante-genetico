#include "../include/evolucao.h"
#include "../include/cruzamento.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

void conferir(bool condicao, const char *mensagem)
{
    if (!condicao) throw std::runtime_error(mensagem);
}

void conferirHistorico(const std::vector<Ponto> &pontos, const ResultadoEvolucao &r)
{
    double anterior = std::numeric_limits<double>::infinity();
    int geracao = 0;
    for (const auto &h : r.historico) {
        conferir(h.geracao == geracao++, "Numeracao incorreta");
        conferir(rotaValida(h.melhorRota, static_cast<int>(pontos.size())), "Rota invalida");
        conferir(std::abs(calcularDistanciaRota(pontos, h.melhorRota) - h.melhorDistancia) < 1e-9, "Distancia incorreta");
        conferir(h.melhorDistancia <= anterior, "Elitismo perdeu a melhor rota");
        conferir(h.distanciaMedia + 1e-9 >= h.melhorDistancia, "Media incorreta");
        anterior = h.melhorDistancia;
    }
    conferir(r.melhorRota == r.historico.back().melhorRota, "Resultado divergente");
    conferir(r.melhorDistancia == r.historico.back().melhorDistancia, "Distancia final divergente");
}

int main()
{
    try {
        auto pontos = gerarPontosCirculares(8, 10);
        ParametrosEvolucao p;
        p.maxGeracoes = 150;
        p.limiteSemMelhora = 0;
        auto a = executarEvolucao(pontos, p), b = executarEvolucao(pontos, p);
        conferirHistorico(pontos, a);
        conferir(a.historico.size() == 151 && a.motivoParada == MotivoParada::limiteGeracoes, "Limite incorreto");
        for (std::size_t i = 0; i < a.historico.size(); ++i)
            conferir(a.historico[i].melhorRota == b.historico[i].melhorRota &&
                     a.historico[i].distanciaMedia == b.historico[i].distanciaMedia, "Semente nao reproduziu resultado");
        double otimo = 8 * 20 * std::sin(std::acos(-1.0) / 8);
        conferir(std::abs(a.melhorDistancia - otimo) < 1e-9, "Benchmark de 8 pontos nao atingido");
        conferir(a.melhorDistancia < a.historico.front().melhorDistancia, "Nao houve evolucao");
        std::cout << "PASSOU: historico, elitismo, reproducibilidade e benchmark circular\n";

        p.tamanhoPopulacao = 11; // Exercita populacao impar.
        p.taxaCruzamento = 0;
        p.taxaMutacao = 0;
        p.limiteSemMelhora = 5;
        auto parado = executarEvolucao(pontos, p);
        conferir(parado.historico.size() == 6 && parado.motivoParada == MotivoParada::estagnacao, "Estagnacao incorreta");
        p.maxGeracoes = 0;
        conferir(executarEvolucao(pontos, p).historico.size() == 1, "Geracao zero incorreta");
        std::cout << "PASSOU: estagnacao, taxas zero, populacao impar e geracao zero\n";

        p.maxGeracoes = 30;
        p.limiteSemMelhora = 0;
        p.taxaMutacao = 1;
        p.taxaCruzamento = 1;
        for (unsigned int s = 0; s < 20; ++s) {
            p.semente = s;
            auto aleatorios = gerarPontosUniformes(12, 100, s);
            conferirHistorico(aleatorios, executarEvolucao(aleatorios, p));
        }
        auto coincidentes = std::vector<Ponto>(8, {0, 0});
        conferirHistorico(coincidentes, executarEvolucao(coincidentes, p));
        std::cout << "PASSOU: 20 sementes aleatorias, taxas um e pontos coincidentes\n";

        int rejeitados = 0;
        auto rejeitar = [&](std::vector<Ponto> entrada, ParametrosEvolucao config) {
            try { executarEvolucao(entrada, config); }
            catch (const std::invalid_argument &) { ++rejeitados; return; }
            throw std::runtime_error("Entrada invalida aceita");
        };
        rejeitar({}, p);
        for (int caso = 0; caso < 10; ++caso) {
            auto invalido = p;
            switch (caso) {
                case 0: invalido.tamanhoPopulacao = 1; break;
                case 1: invalido.tamanhoTorneio = 0; break;
                case 2: invalido.tamanhoTorneio = 12; break;
                case 3: invalido.quantidadeElite = 0; break;
                case 4: invalido.quantidadeElite = 11; break;
                case 5: invalido.taxaMutacao = -0.1; break;
                case 6: invalido.taxaCruzamento = 1.1; break;
                case 7: invalido.taxaMutacao = std::numeric_limits<double>::quiet_NaN(); break;
                case 8: invalido.maxGeracoes = -1; break;
                case 9: invalido.limiteSemMelhora = -1; break;
            }
            rejeitar(pontos, invalido);
        }
        pontos[0].x = std::numeric_limits<double>::infinity();
        rejeitar(pontos, p);
        std::cout << "PASSOU: " << rejeitados << " entradas invalidas rejeitadas\n";
    } catch (const std::exception &e) {
        std::cerr << "FALHOU: " << e.what() << '\n';
        return 1;
    }
}
