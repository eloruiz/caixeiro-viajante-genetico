#include "../include/cruzamento.h"
#include <cstddef>
#include <stdexcept>
#include <utility>

/* Confere se a rota é uma permutação válida dos pontos 0 a n-1 */
bool rotaValida(const std::vector<int> &rota, int n)
{
    /* Uma rota com tamanho diferente de n já perde ou repete algum ponto */
    if (rota.size() != static_cast<std::size_t>(n))
    {
        return false;
    }

    std::vector<bool> visitado(n, false);

    for (int ponto : rota)
    {
        /* Rejeita índices inexistentes e pontos visitados mais de uma vez */
        if (ponto < 0 || ponto >= n || visitado[ponto])
        {
            return false;
        }

        visitado[ponto] = true;
    }

    return true;
}

/* Order Crossover (OX): copia um trecho do pai1 e completa com a ordem do pai2 */
std::vector<int> cruzamentoOX(const std::vector<int> &pai1, const std::vector<int> &pai2, std::mt19937 &gerador)
{
    int n = static_cast<int>(pai1.size());

    if (n < 2 || pai2.size() != pai1.size())
    {
        throw std::invalid_argument("Os pais devem ter o mesmo tamanho (minimo 2).");
    }

    /* Sorteia dois cortes e garante que inicio <= fim */
    std::uniform_int_distribution<int> sorteio(0, n - 1);
    int inicio = sorteio(gerador);
    int fim = sorteio(gerador);

    if (inicio > fim)
    {
        std::swap(inicio, fim);
    }

    std::vector<int> filho(n, -1);
    std::vector<bool> usado(n, false);

    /* Passo 1: o filho herda do pai1 o trecho entre os cortes, na mesma posição */
    for (int i = inicio; i <= fim; i++)
    {
        filho[i] = pai1[i];
        usado[pai1[i]] = true;
    }

    /* Passo 2: percorre o pai2 a partir do corte final, dando a volta no vetor,
       e coloca no filho os pontos que ainda não foram usados, nessa ordem */
    int posicao = (fim + 1) % n;

    for (int k = 0; k < n; k++)
    {
        int ponto = pai2[(fim + 1 + k) % n];

        if (!usado[ponto])
        {
            filho[posicao] = ponto;
            posicao = (posicao + 1) % n;
        }
    }

    return filho;
}
