#include "../include/evolucao.h"
#include "../include/cruzamento.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>

namespace {
void exigir(bool valor, const char *mensagem) {
    if (!valor) throw std::runtime_error(mensagem);
}

// Calculo independente: inclui explicitamente a aresta final e usa hypot.
double circuito(const std::vector<Ponto> &pontos, const std::vector<int> &rota) {
    double soma = 0;
    for (std::size_t i = 1; i < rota.size(); ++i) {
        const auto &a = pontos.at(rota[i - 1]);
        const auto &b = pontos.at(rota[i]);
        soma += std::hypot(a.x - b.x, a.y - b.y);
    }
    const auto &a = pontos.at(rota.back());
    const auto &b = pontos.at(rota.front());
    return soma + std::hypot(a.x - b.x, a.y - b.y);
}

void validarResultado(const std::vector<Ponto> &pontos, const ResultadoEvolucao &r, int geracoes) {
    exigir(r.historico.size() == static_cast<std::size_t>(geracoes + 1), "Historico incompleto");
    double anterior = std::numeric_limits<double>::infinity();
    for (int i = 0; i <= geracoes; ++i) {
        const auto &h = r.historico.at(i);
        exigir(h.geracao == i && rotaValida(h.melhorRota, static_cast<int>(pontos.size())), "Geracao ou rota invalida");
        double esperado = circuito(pontos, h.melhorRota);
        exigir(std::isfinite(h.melhorDistancia) && std::abs(esperado - h.melhorDistancia) <= 1e-8 * std::max(1.0, esperado), "Circuito fechado divergente");
        exigir(std::isfinite(h.distanciaMedia) && h.distanciaMedia + 1e-8 >= h.melhorDistancia, "Media invalida");
        exigir(h.melhorDistancia <= anterior, "Melhor solucao perdida");
        anterior = h.melhorDistancia;
    }
    exigir(r.melhorRota == r.historico.back().melhorRota && r.melhorDistancia == anterior, "Resultado final divergente");
}
}

int main() {
    try {
        const double pi = std::acos(-1.0);
        int execucoes = 0;
        // 2 cenarios x 4 quantidades x 3 configuracoes x 2 sementes.
        for (int n : {8, 9, 25, 100}) for (bool circular : {false, true})
        for (int config = 0; config < 3; ++config) for (unsigned int s : {7u, 99u}) {
            auto pontos = circular ? gerarPontosCirculares(n, 7.5) : gerarPontosUniformes(n, 80, s);
            ParametrosEvolucao p;
            p.semente = s;
            p.tamanhoPopulacao = config == 0 ? 2 : (config == 1 ? 17 : 30);
            p.tamanhoTorneio = config == 0 ? 1 : (config == 1 ? 3 : 30);
            p.quantidadeElite = config == 2 ? 29 : 1;
            p.taxaMutacao = config == 0 ? 0 : (config == 1 ? 1 : 0.25);
            p.taxaCruzamento = config == 0 ? 1 : (config == 1 ? 0 : 0.9);
            p.maxGeracoes = 25;
            p.limiteSemMelhora = 0;
            auto r = executarEvolucao(pontos, p);
            validarResultado(pontos, r, 25);
            if (circular) exigir(r.melhorDistancia + 1e-8 >= n * 15 * std::sin(pi / n), "Distancia menor que o otimo circular");
            ++execucoes;
        }
        std::cout << "PASSOU: " << execucoes << " execucoes em matriz de cenarios, tamanhos e parametros\n";

        // Confere o otimo de um problema aleatorio pequeno por enumeracao exata.
        auto pontos = gerarPontosUniformes(8, 100, 17);
        std::vector<int> rota(8);
        std::iota(rota.begin(), rota.end(), 0);
        double otimo = std::numeric_limits<double>::infinity();
        do { otimo = std::min(otimo, circuito(pontos, rota)); }
        while (std::next_permutation(rota.begin() + 1, rota.end()));
        ParametrosEvolucao p;
        p.maxGeracoes = 60;
        p.limiteSemMelhora = 0;
        auto r = executarEvolucao(pontos, p);
        exigir(r.melhorDistancia + 1e-8 >= otimo, "Resultado menor que o otimo enumerado");
        auto invertida = r.melhorRota;
        std::reverse(invertida.begin(), invertida.end());
        exigir(std::abs(calcularDistanciaRota(pontos, invertida) - r.melhorDistancia) < 1e-8, "Sentido alterou o circuito");
        auto deslocada = r.melhorRota;
        std::rotate(deslocada.begin(), deslocada.begin() + 3, deslocada.end());
        exigir(std::abs(calcularDistanciaRota(pontos, deslocada) - r.melhorDistancia) < 1e-8, "Origem alterou o circuito");
        auto ruim = r.melhorRota;
        double maior = r.melhorDistancia;
        std::sort(ruim.begin(), ruim.end());
        do {
            double d = circuito(pontos, ruim);
            exigir(std::abs(calcularAptidao(pontos, ruim) - 1 / (1 + d)) < 1e-12, "Aptidao divergente");
            maior = std::max(maior, d);
        } while (std::next_permutation(ruim.begin(), ruim.end()));
        exigir(1 / (1 + maior) < calcularAptidao(pontos, r.melhorRota), "Aptidao nao favorece menor distancia");
        std::cout << "PASSOU: enumeracao exata de 8 cidades, aptidao e invariancia do circuito\n";

        std::mt19937 gerador(123);
        int cruzamentos = 0;
        for (int n : {8, 9, 25, 100, 500}) {
            std::vector<int> a(n), b(n);
            std::iota(a.begin(), a.end(), 0);
            b = a;
            for (int i = 0; i < 200; ++i) {
                std::shuffle(a.begin(), a.end(), gerador);
                std::shuffle(b.begin(), b.end(), gerador);
                exigir(rotaValida(cruzamentoOX(a, b, gerador), n), "OX perdeu cidades");
                ++cruzamentos;
            }
        }
        std::cout << "PASSOU: " << cruzamentos << " cruzamentos OX entre 8 e 500 cidades\n";

        int rejeitados = 0;
        for (double limite : {0.0, -1.0, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()}) {
            try { gerarPontosCirculares(8, limite); } catch (const std::invalid_argument &) { ++rejeitados; }
            try { gerarPontosUniformes(8, limite, 42); } catch (const std::invalid_argument &) { ++rejeitados; }
        }
        for (int n : {-1, 0, 7}) {
            try { gerarPontosCirculares(n, 10); } catch (const std::invalid_argument &) { ++rejeitados; }
            try { gerarPontosUniformes(n, 10, 42); } catch (const std::invalid_argument &) { ++rejeitados; }
        }
        exigir(rejeitados == 14, "Geradores aceitaram parametros invalidos");
        // Todas as distancias sao iguais: verifica empate e precedencia das paradas.
        p.maxGeracoes = 4;
        p.limiteSemMelhora = 4;
        auto empate = executarEvolucao(std::vector<Ponto>(8, {-2, -3}), p);
        validarResultado(std::vector<Ponto>(8, {-2, -3}), empate, 4);
        exigir(empate.motivoParada == MotivoParada::estagnacao && empate.melhorDistancia == 0, "Empate de parada incorreto");
        std::cout << "PASSOU: 14 entradas invalidas dos geradores e empate de parada\n";

        auto grandes = gerarPontosCirculares(500, 10);
        p.maxGeracoes = 50;
        p.limiteSemMelhora = 0;
        auto inicio = std::chrono::steady_clock::now();
        auto escala = executarEvolucao(grandes, p);
        double segundos = std::chrono::duration<double>(std::chrono::steady_clock::now() - inicio).count();
        validarResultado(grandes, escala, 50);
        std::cout << "PASSOU: escala de 500 cidades, 100 individuos, 50 geracoes; tempo=" << segundos
                  << "s; inicial=" << escala.historico.front().melhorDistancia << "; final=" << escala.melhorDistancia
                  << "; otimo=" << 10000 * std::sin(pi / 500) << "\n";
    } catch (const std::exception &e) {
        std::cerr << "FALHOU: " << e.what() << '\n';
        return 1;
    }
}
