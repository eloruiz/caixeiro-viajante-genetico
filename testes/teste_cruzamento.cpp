#include "../include/cruzamento.h"
#include <algorithm>
#include <iostream>
#include <numeric>

int main()
{
    /* Confere se rotaValida aceita uma permutação correta */
    if (!rotaValida({2, 0, 3, 1}, 4))
    {
        std::cout << "Rota valida foi rejeitada!\n";
        return 1;
    }

    /* Confere se rotaValida rejeita ponto repetido, ponto fora do intervalo e tamanho errado */
    if (rotaValida({3, 1, 3, 2}, 4) || rotaValida({0, 1, 2, 4}, 4) || rotaValida({0, 1, 2}, 4))
    {
        std::cout << "Rota invalida foi aceita!\n";
        return 1;
    }

    std::cout << "Teste da validacao passou!\n";

    /* A semente fixa permite repetir o teste com os mesmos resultados */
    std::mt19937 gerador(42);

    int n = 12;
    std::vector<int> pai1(n);
    std::iota(pai1.begin(), pai1.end(), 0);
    std::vector<int> pai2 = pai1;

    /* Cruza 1000 pares de pais embaralhados e confere se todos os filhos são rotas válidas */
    for (int tentativa = 0; tentativa < 1000; tentativa++)
    {
        std::shuffle(pai1.begin(), pai1.end(), gerador);
        std::shuffle(pai2.begin(), pai2.end(), gerador);

        std::vector<int> filho = cruzamentoOX(pai1, pai2, gerador);

        if (!rotaValida(filho, n))
        {
            std::cout << "O cruzamento gerou uma rota invalida!\n";
            return 1;
        }
    }

    std::cout << "Teste do cruzamento passou!\n";

    /* Dois pais iguais devem gerar um filho igual a eles */
    std::vector<int> filhoIgual = cruzamentoOX(pai1, pai1, gerador);

    if (filhoIgual != pai1)
    {
        std::cout << "Pais iguais deveriam gerar filho igual!\n";
        return 1;
    }

    std::cout << "Teste dos pais iguais passou!\n";

    return 0;
}
