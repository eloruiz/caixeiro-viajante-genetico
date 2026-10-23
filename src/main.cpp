#include "../include/evolucao.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv)
{
    try
    {
        if (argc > 8)
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
        if (cenario != "circular" && cenario != "aleatorio")
            throw std::invalid_argument("Cenario deve ser circular ou aleatorio.");
        auto pontos = cenario == "circular" ? gerarPontosCirculares(quantidade, 10) :
                                             gerarPontosUniformes(quantidade, 100, p.semente);
        auto resultado = executarEvolucao(pontos, p);
        std::ofstream historico("historico.csv"), coordenadas("pontos.csv");
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
                  << "\nGeracoes: " << resultado.historico.back().geracao
                  << "\nParada: " << (resultado.motivoParada == MotivoParada::estagnacao ? "estagnacao" : "limite de geracoes")
                  << "\nRota: ";
        for (int cidade : resultado.melhorRota) std::cout << cidade << ' ';
        std::cout << resultado.melhorRota.front() << "\nArquivos: historico.csv e pontos.csv\n";
    }
    catch (const std::exception &erro)
    {
        std::cerr << "Erro: " << erro.what()
                  << "\nUso: ag [circular|aleatorio] [pontos] [semente] [populacao] [geracoes] [mutacao] [sem_melhora]\n";
        return 1;
    }
}
