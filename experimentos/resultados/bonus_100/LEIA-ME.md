# Bônus simples: 100 cidades

## O que fizemos
Usamos o mesmo algoritmo genético em C++, aumentando de 12 para 100 cidades
em um círculo de raio 10. O Python apenas executa o programa, lê os resultados e gera a imagem.

## Como executar
Na raiz do projeto, com Python, matplotlib e g++ instalados:

```powershell
python -m pip install matplotlib
python experimentos/bonus_simples.py
```

O script compila o C++ automaticamente. Os parâmetros ficam no início do script.
População: 100; semente: 52; mutação: 10% por filho;
OX: 90%; torneio: 3; elite: 1. A semente permite repetir o sorteio no mesmo ambiente.
Mantivemos população e operadores do teste normal para facilitar a comparação.

## Quando termina
Para após 500 gerações sem melhora estrita, ou no teto de 20000 gerações.
Neste teste, o motivo foi: **500 gerações consecutivas sem melhora**.
Chamamos essa estabilização de convergência operacional; ela não garante o ótimo global.

## Resultado desta execução
- Distância inicial: 1150.7070.
- Distância final: 184.0140.
- Redução: 84.01%.
- Última melhora: geração 1691; parada: geração 2191.
- Tempo do algoritmo até a parada: **0.1766 s**, medido pelo C++ antes da exportação.
- Ótimo teórico: 62.8215; erro relativo final: 192.92%.

O tempo inclui a população inicial e a evolução, mas exclui compilação, gráficos e exportação.
Varia com o computador e a compilação. O resultado final deve ser comparado com o ótimo,
sem afirmar que a estagnação comprova a melhor solução possível.

![Gráfico e rotas](bonus.png)

## Como explicar à turma
“Usamos 100 cidades no círculo com o mesmo algoritmo do teste normal.
A cada geração, selecionamos, cruzamos e eventualmente mutamos as rotas, preservando a melhor.
Paramos quando a melhor distância ficou 500 gerações sem diminuir.
O gráfico mostra a melhora e as três rotas mostram a evolução visual.
O tempo foi medido no C++; os gráficos foram feitos em Python.”

Os CSVs guardam todas as épocas; `execucao.txt` registra parâmetros e saída.
O script confere as rotas e recalcula as distâncias incluindo o retorno à origem.
