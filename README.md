# 🧬 Algoritmo Genético para o Problema do Caixeiro Viajante (TSP)

> **PI: Sistemas Inteligentes e Aprendizado de Máquina (SIAM)**  
> **Pontifícia Universidade Católica de Campinas (PUC-Campinas)**  
> **Docente:** Prof. Fernando S. de Aguiar Neto  

---

## 👥 Integrantes do Grupo
* **Pedro Henrique Coan Zin**
* **Lucas Leal Ibrahim**
* **Renato Henrique Ykutake Florencio**
* **Felipe Ishizawa Diniz**
* **Eloise Dos Santos Ruiz**

Repositório Oficial: [github.com/eloruiz/caixeiro-viajante-genetico](https://github.com/eloruiz/caixeiro-viajante-genetico)

---

## 📌 Sumário
1. [Visão Geral do Projeto](#-visão-geral-do-projeto)
2. [Definição do Problema (TSP 2D)](#-definição-do-problema-tsp-2d)
3. [Cenários de Teste](#-cenários-de-teste)
4. [Estrutura do Trabalho (Frentes Técnicas)](#-estrutura-do-trabalho-frentes-técnicas)
5. [Critérios de Avaliação e Entregas](#-critérios-de-avaliação-e-entregas)

---

## 🎯 Visão Geral do Projeto

Este projeto tem como objetivo conceber, implementar e avaliar um **Algoritmo Genético (AG)** para encontrar soluções locais para o **Problema do Caixeiro Viajante (Traveling Salesperson Problem - TSP)** em espaço bidimensional ($\mathbb{R}^2$).

O foco da atividade não é a garantia matemática de um ótimo global, mas sim explorar a dinâmica bioinspirada do algoritmo, avaliando o comportamento da convergência, a capacidade de desatamento de cruzamentos de rotas e o desempenho das soluções encontradas ao longo das gerações/épocas.

---

## 🗺️ Definição do Problema (TSP 2D)

Dado um conjunto arbitrário de $N$ pontos no plano cartesiano ($N \ge 8$), onde cada ponto $p_i$ possui coordenadas $(x_i, y_i) \in \mathbb{R}^2$:

1. **Circuito Fechado**: A rota deve partir de um ponto inicial, visitar cada um dos demais pontos exatamente uma vez e retornar ao ponto de origem.
2. **Distância Euclidiana**: A distância entre dois pontos $p_a$ e $p_b$ é dada por:
   $$d(p_a, p_b) = \sqrt{(x_a - x_b)^2 + (y_a - y_b)^2}$$
3. **Função Objetivo**: Minimizar a extensão total do circuito $I = [c_0, c_1, \dots, c_{N-1}]$:
   $$D(I) = \left( \sum_{i=0}^{N-2} d(p_{c_i}, p_{c_{i+1}}) \right) + d(p_{c_{N-1}}, p_{c_0})$$

---

## 🧪 Cenários de Teste

O algoritmo será avaliado sob duas configurações topológicas:

### 1. Pontos Uniformemente Distribuídos
* Pontos dispersos aleatoriamente em um plano bidimensional.
* **Propósito**: Avaliar a capacidade do algoritmo genético de explorar espaços menos estruturados, escapar de mínimos locais e desatar cruzamentos complexos de arestas.

### 2. Círculo de Pontos (Benchmark Analítico)
* $N$ pontos equidistantes dispostos ao longo do perímetro de uma circunferência de raio $R$:
  $$x_i = R \cdot \cos\left(\frac{2\pi i}{N}\right), \quad y_i = R \cdot \sin\left(\frac{2\pi i}{N}\right)$$
* **Propósito**: Como a solução ótima global é geometricamente conhecida (o perímetro do polígono regular convexo), este cenário funciona como **benchmark**. A distância mínima teórica é:
  $$D_{\text{ótimo}} = N \cdot 2R \sin\left(\frac{\pi}{N}\right)$$
* Permite calcular com precisão o erro relativo da solução encontrada pelo AG em relação à solução analítica ideal.

---

## 🧩 Estrutura do Trabalho (Frentes Técnicas)

Para assegurar uma abordagem modular e desacoplada, o projeto está organizado em 5 frentes de trabalho:

* **Frente 1 — Modelagem e Aptidão**: Geração das coordenadas para os dois cenários, implementação do cálculo de distâncias euclidianas do circuito fechado e definição da métrica de aptidão (*fitness*).
* **Frente 2 — Representação e Cruzamento**: Estruturação dos dados para representação válida de rotas (permutações sem repetição) e implementação de operadores de cruzamento (*crossover*) adequados ao TSP.
* **Frente 3 — Mecanismo Evolutivo e Parametrização**: Implementação da população inicial, operadores de seleção, taxas de mutação, transição entre gerações e definição dos critérios de parada.
* **Frente 4 — Experimentação no Cenário Aleatório**: Execução dos testes com pontos dispersos, acompanhamento do histórico de distâncias a cada época e geração de gráficos de convergência e visualização da rota.
* **Frente 5 — Experimentação no Benchmark Circular & Bônus**: Execução no cenário circular, comparação com o limite analítico, produção de visualizações por época e condução do teste de estresse de escala (alta quantidade de pontos com medição de tempo de convergência).

---

## 📋 Critérios de Avaliação e Entregas

Conforme especificado no enunciado da atividade:

| Critério | Peso | Descrição da Entrega |
| :--- | :---: | :--- |
| **Representação e Cruzamento** | 2,0 pts | Justificativa da codificação do gene e do operador de cruzamento adotado. |
| **Desempenho no Cenário Aleatório** | 2,5 pts | Acompanhamento e visualização da solução e do desempenho época a época. |
| **Desempenho no Cenário Circular** | 2,5 pts | Acompanhamento e visualização da evolução das rotas no benchmark circular. |
| **População e Critério de Parada** | 1,0 pt | Justificativa do dimensionamento da população e das regras de encerramento da busca. |
| **Taxa de Mutação** | 1,0 pt | Justificativa da escolha da frequência/probabilidade de mutação. |
| **Função de Aptidão** | 1,0 pt | Explicação matemática e conceitual da métrica de aptidão utilizada. |
| **Bônus (Opcional)** | Até 1,0 pt | Teste do modelo circular com alta quantidade de pontos, registrando tempo de convergência e rotas intermediárias. |