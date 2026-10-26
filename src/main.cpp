#include "../include/evolucao.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv)
{
    try
    {
        if (argc > 9)
            throw std::invalid_argument("Argumentos em excesso.");
        std::string cenario = argc > 1 ? argv[1] : "circular";
        auto inteiro = [](const char *texto) {
            std::size_t fim;
            int valor = std::stoi(texto, &fim);
            if (texto[fim] != '\0') throw std::invalid_argument("Inteiro invalido.");
            return valor;
        };
        int quantidade = argc > 2 ? inteiro(argv[2]) : 12;
        ParametrosEvolucao p;
        if (argc > 3) {
            int semente = inteiro(argv[3]);
            if (semente < 0) throw std::invalid_argument("Semente negativa.");
            p.semente = static_cast<unsigned int>(semente);
        }
        if (argc > 4) p.tamanhoPopulacao = inteiro(argv[4]);
        if (argc > 5) p.maxGeracoes = inteiro(argv[5]);
        if (argc > 6) {
            std::size_t fim;
            p.taxaMutacao = std::stod(argv[6], &fim);
            if (argv[6][fim] != '\0') throw std::invalid_argument("Taxa invalida.");
        }
        if (argc > 7) p.limiteSemMelhora = inteiro(argv[7]);
        std::string pastaSaida = argc > 8 ? argv[8] : ".";
        if (cenario != "circular" && cenario != "aleatorio")
            throw std::invalid_argument("Cenario deve ser circular ou aleatorio.");
        auto pontos = cenario == "circular" ? gerarPontosCirculares(quantidade, 10.0) :
                                             gerarPontosUniformes(quantidade, 100.0, p.semente);

        auto inicio = std::chrono::high_resolution_clock::now();
        auto resultado = executarEvolucao(pontos, p);
        auto fim = std::chrono::high_resolution_clock::now();
        double tempoSegundos = std::chrono::duration<double>(fim - inicio).count();

        std::string caminhoHistorico = (pastaSaida == "." ? "" : pastaSaida + "/") + "historico.csv";
        std::string caminhoPontos = (pastaSaida == "." ? "" : pastaSaida + "/") + "pontos.csv";

        std::ofstream historico(caminhoHistorico), coordenadas(caminhoPontos);
        historico.exceptions(std::ios::failbit | std::ios::badbit);
        coordenadas.exceptions(std::ios::failbit | std::ios::badbit);
        historico << std::setprecision(17) << "geracao,melhor_distancia,distancia_media,rota\n";
        for (const auto &registro : resultado.historico) {
            historico << registro.geracao << ',' << registro.melhorDistancia << ',' << registro.distanciaMedia << ",\"";
            for (int cidade : registro.melhorRota) historico << cidade << ' ';
            historico << "\"\n";
        }
        coordenadas << std::setprecision(17) << "cidade,x,y\n";
        for (std::size_t i = 0; i < pontos.size(); ++i)
            coordenadas << i << ',' << pontos[i].x << ',' << pontos[i].y << '\n';
        historico.close();
        coordenadas.close();
        std::cout << std::setprecision(12)
                  << "Distancia inicial: " << resultado.historico.front().melhorDistancia
                  << "\nDistancia final: " << resultado.melhorDistancia
                  << "\nTempo: " << std::fixed << std::setprecision(4) << tempoSegundos << " s"
                  << "\nGeracoes: " << resultado.historico.back().geracao
                  << "\nParada: " << (resultado.motivoParada == MotivoParada::estagnacao ? "estagnacao" : "limite de geracoes");
        if (cenario == "circular") {
            const double pi = std::acos(-1.0);
            double otimo = quantidade * 2.0 * 10.0 * std::sin(pi / quantidade);
            double erro = (resultado.melhorDistancia - otimo) / otimo * 100.0;
            std::cout << "\nOtimo teorico: " << std::setprecision(6) << otimo
                      << "\nErro relativo: " << std::setprecision(4) << erro << "%";
        }
        std::cout << "\nRota: ";
        for (int cidade : resultado.melhorRota) std::cout << cidade << ' ';
        std::cout << resultado.melhorRota.front() << "\nArquivos: " << caminhoHistorico << " e " << caminhoPontos << "\n";
    }
    catch (const std::exception &erro)
    {
        std::cerr << "Erro: " << erro.what()
                  << "\nUso: ag [circular|aleatorio] [pontos] [semente] [populacao] [geracoes] [mutacao] [sem_melhora] [pasta_saida]\n";
        return 1;
    }
}
