#include "../include/problema.h"
#include <iostream>
#include <cmath>

int main()
{
    /* Confere se a distância entre (0, 0) e (3, 4) é 5 */
    Ponto a = {0.0, 0.0};
    Ponto b = {3.0, 4.0};

    double distancia = calcularDistancia(a, b);
    std::cout << "Distancia calculada: " << distancia << '\n';

    if (std::abs(distancia - 5.0) >= 0.000001)
    {
        std::cout << "Teste da distancia falhou!\n";
        return 1;
    }

    std::cout << "Teste da distancia passou!\n";

    /* Confere se a rota fechada de um quadrado de lado 2 mede 8 */
    std::vector<Ponto> pontos =
        {
            {0.0, 0.0},
            {2.0, 0.0},
            {2.0, 2.0},
            {0.0, 2.0}};

    std::vector<int> rota = {0, 1, 2, 3};

    double total = calcularDistanciaRota(pontos, rota);

    std::cout << "Distancia da rota: " << total << '\n';

    if (std::abs(total - 8.0) >= 0.000001)
    {
        std::cout << "Teste da rota falhou!\n";
        return 1;
    }

    std::cout << "Teste da rota passou!\n";

    /* Confere se a aptidão da rota de distância 8 corresponde a 1/9 */
    double aptidao = calcularAptidao(pontos, rota);
    double esperado = 1.0 / 9.0;

    std::cout << "Aptidao calculada: " << aptidao << '\n';

    if (std::abs(aptidao - esperado) >= 0.000001)
    {
        std::cout << "Teste da aptidao falhou!\n";
        return 1;
    }

    std::cout << "Teste da aptidao passou!\n";

    /* Confere a quantidade de pontos gerados no círculo */
    int quantidade = 8;
    double raio = 10.0;

    std::vector<Ponto> circulares = gerarPontosCirculares(quantidade, raio);

    if (circulares.size() != static_cast<std::size_t>(quantidade))
    {
        std::cout << "Quantidade de pontos incorreta!\n";
        return 1;
    }

    /* Verifica se todos os pontos estão à distância do raio em relação ao centro */
    Ponto centro = {0.0, 0.0};

    for (const Ponto &ponto : circulares)
    {
        if (std::abs(calcularDistancia(centro, ponto) - raio) >= 0.000001)
        {
            std::cout << "Ponto fora da circunferencia!\n";
            return 1;
        }
    }

    /*  Monta a rota seguindo a ordem dos pontos ao redor do círculo */
    std::vector<int> rotaCircular;

    for (int i = 0; i < quantidade; i++)
    {
        rotaCircular.push_back(i);
    }

    /* Compara a distância calculada com o perímetro teórico do polígono regular */
    double perimetro = calcularDistanciaRota(circulares, rotaCircular);

    const double pi = std::acos(-1.0);
    double perimetroEsperado = quantidade * 2.0 * raio * std::sin(pi / quantidade);

    std::cout << "Perimetro circular: " << perimetro << '\n';

    if (std::abs(perimetro - perimetroEsperado) >= 0.000001)
    {
        std::cout << "Teste do perimetro falhou!\n";
        return 1;
    }

    std::cout << "Teste circular passou!\n";

    /* Gera dois conjuntos com os mesmos parâmetros para conferir a reprodução */
    std::vector<Ponto> uniformes = gerarPontosUniformes(12, 100.0, 42);
    std::vector<Ponto> repetidos = gerarPontosUniformes(12, 100.0, 42);

    if (uniformes.size() != 12 || repetidos.size() != 12)
    {
        std::cout << "Quantidade de pontos uniformes incorreta!\n";
        return 1;
    }

    for (std::size_t i = 0; i < uniformes.size(); i++)
    {
        const Ponto &ponto = uniformes[i];

        /* Confere se as coordenadas são finitas e estão dentro da área definida */
        if (!std::isfinite(ponto.x) || !std::isfinite(ponto.y) || ponto.x < 0.0 || ponto.x > 100.0 || ponto.y < 0.0 || ponto.y > 100.0)
        {
            std::cout << "Coordenada invalida ou fora dos limites!\n";
            return 1;
        }

        /* Confere se a mesma semente reproduziu exatamente as coordenadas */
        if (ponto.x != repetidos[i].x || ponto.y != repetidos[i].y)
        {
            std::cout << "Mesma semente gerou pontos diferentes!\n";
            return 1;
        }
    }

    std::cout << "Teste dos pontos uniformes passou!\n";

    return 0;
}