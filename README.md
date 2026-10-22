# 🧬 Otimização do Problema do Caixeiro Viajante com Algoritmos Genéticos (TSP-GA)

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
3. [Fundamentação Teórica e Modelagem Genética](#-fundamentação-teórica-e-modelagem-genética)
   - [Representação do Genótipo e Genes](#1-representação-do-genótipo-e-genes)
   - [Operadores de Cruzamento (Crossover)](#2-operadores-de-cruzamento-crossover)
   - [Operadores de Mutação e Taxa](#3-operadores-de-mutação-e-taxa)
   - [Função de Aptidão (Fitness)](#4-função-de-aptidão-fitness)
   - [Seleção e Elitismo](#5-estratégia-de-seleção-e-elitismo)
   - [Tamanho da População e Critérios de Parada](#6-tamanho-da-população-e-critérios-de-parada)
4. [Cenários de Teste e Benchmarks](#-cenários-de-teste-e-benchmarks)
   - [Cenário 1: Pontos Uniformemente Distribuídos](#cenário-1-pontos-uniformemente-distribuídos)
   - [Cenário 2: Círculo de Pontos (Benchmark Analítico)](#cenário-2-círculo-de-pontos-benchmark-analítico)
   - [Cenário Bônus: Teste de Escala em Círculo](#cenário-bônus-alta-quantidade-de-pontos)
5. [Métricas de Desempenho e Acompanhamento](#-métricas-de-desempenho-e-acompanhamento)
6. [Arquitetura de Código Proposta](#-arquitetura-de-código-proposta)
7. [Tecnologias Utilizadas](#️-tecnologias-utilizadas)
8. [Como Executar o Projeto](#-como-executar-o-projeto)
9. [Referências Bibliográficas](#-referências-bibliográficas)

---

## 🎯 Visão Geral do Projeto

Este projeto consiste na concepção, implementação, parametrização e análise empírica de um **Algoritmo Genético (AG)** bioinspirado aplicado ao **Problema do Caixeiro Viajante (Traveling Salesperson Problem - TSP)** em espaço bidimensional ($\mathbb{R}^2$).

O TSP é um problema clássico de otimização combinatória classificado como **NP-difícil**. Para $N$ cidades em um grafo completo simétrico, o espaço de busca de rotas possíveis é dado por:
$$\frac{(N-1)!}{2}$$

Para $N = 8$, existem $2.520$ rotas possíveis; para $N = 20$, são aproximadamente $6 \times 10^{16}$ rotas; e para $N = 100$, ultrapassa $4 \times 10^{155}$, inviabilizando qualquer busca exaustiva (força bruta). Os Algoritmos Genéticos oferecem uma abordagem heurística estocástica eficiente para explorar esse vasto espaço combinatório, combinando **exploração** (*exploration* - busca global) com **refinamento** (*exploitation* - busca local).

---

## 🗺️ Definição do Problema (TSP 2D)

Dado um conjunto arbitrário de $N$ pontos ($N \ge 8$) em duas dimensões $P = \{p_0, p_1, \dots, p_{N-1}\}$, onde cada ponto possui coordenadas $p_i = (x_i, y_i) \in \mathbb{R}^2$:

1. **Caminho Cíclico**: O caixeiro deve partir de uma cidade inicial, visitar cada uma das demais cidades exatamente uma vez (circuito hamiltoniano) e retornar à cidade inicial.
2. **Função Objetivo**: Minimizar a distância euclidiana total percorrida pelo circuito. A distância euclidiana entre dois pontos $p_a$ e $p_b$ é dada por:
   $$d(p_a, p_b) = \sqrt{(x_a - x_b)^2 + (y_a - y_b)^2}$$

A distância total de um circuito ordenado $I = [c_0, c_1, \dots, c_{N-1}]$ é:
$$D(I) = \left( \sum_{i=0}^{N-2} d(p_{c_i}, p_{c_{i+1}}) \right) + d(p_{c_{N-1}}, p_{c_0})$$

---

## 🧬 Fundamentação Teórica e Modelagem Genética

O algoritmo segue o ciclo clássico de computação evolutiva apresentado em aula:
```
[População Inicial] ➔ [Avaliação de Fitness] ➔ [Critério de Parada?] 
       ▲                                               │ Não
       │                      Sim                      ▼
   [Nova Geração] ◄── [Mutação] ◄── [Crossover] ◄── [Seleção]
       │
       ▼
[Solução Ótima Encontrada]
```

### 1. Representação do Genótipo e Genes
* **Formulação**: Vetor unidimensional de números inteiros representando uma permutação sem repetição das cidades:
  $$I = [c_0, c_1, c_2, \dots, c_{N-1}], \quad c_i \in \{0, 1, \dots, N-1\}$$
* **Justificativa da Escolha**: 
  - Codificações binárias tradicionais (*bitstrings*) geram soluções inválidas frequentes (cidades repetidas ou omitidas), demandando operadores complexos de reparação e desperdiçando ciclos de processamento.
  - A representação por permutação garante naturalmente que toda cidade exista no vetor. A restrição fundamental passa a ser respeitada pelos próprios operadores genéticos (crossover e mutação específicos para permutações).

### 2. Operadores de Cruzamento (Crossover)
Como os indivíduos são permutações de índices únicos, operadores de crossover simples (como ponto único tradicional) criariam rotas ilegais com duplicatas.
* **Operador Principal: *Ordered Crossover (OX)***
  1. Seleciona-se aleatoriamente uma subsequência contígua do Pai 1 e copia-se para o Filho nas mesmas posições.
  2. Os genes restantes são preenchidos a partir do Pai 2 (começando logo após o ponto final da subsequência selecionada), preservando sua ordem relativa e descartando elementos que já foram herdados do Pai 1.
* **Por que o OX?**: O Ordered Crossover preserva simultaneamente **adjacência** e a **ordem relativa** entre grupos de cidades, fundamentais para a boa formação de rotas curtas.

### 3. Operadores de Mutação e Taxa
* **Operadores de Mutação**:
  - **Swap Mutation (Troca)**: Selecionam-se duas posições aleatórias no cromossomo e invertem-se os seus valores.
  - **Inversion Mutation (2-opt Genético)**: Seleciona-se um segmento da rota e inverte-se a ordem dos elementos naquele intervalo. É comprovadamente o operador de mutação mais eficaz para TSP geométrico, pois desfaz cruzamentos em "X" de arestas.
* **Escolha e Justificativa da Taxa de Mutação ($\mu$)**:
  - Uma taxa muito baixa ($\mu \approx 0\%$) resulta em rápida perda de diversidade alélica e convergência prematura em mínimos locais (*estagnação*).
  - Uma taxa excessivamente alta ($\mu > 20\%$) degenera o algoritmo em uma busca aleatória (*random walk*), destruindo os blocos construtores (*building blocks*) adquiridos pela seleção e crossover.
  - **Taxa recomendada**: $1\%$ a $5\%$ de probabilidade de mutação por indivíduo, ou $1/N$ por gene, permitindo perturbações suficientes para escapar de mínimos locais sem perder o progresso obtido.

### 4. Função de Aptidão (Fitness)
* No TSP, o objetivo natural é de **minimização** da distância total $D(I)$.
* Como a maioria dos operadores de seleção probabilística (como roleta) assume que aptidões maiores têm maior probabilidade de reprodução, adotamos:
  $$Fitness(I) = \frac{1}{D(I)} \quad \text{ou} \quad Fitness(I) = \frac{K}{D(I)}$$
  *(onde $K$ é uma constante de escala para estabilidade numérica)*.
* Em seleção baseada em torneio ou ranking direto, pode-se avaliar diretamente o custo $D(I)$, onde o menor valor vence.

### 5. Estratégia de Seleção e Elitismo
* **Seleção por Torneio ($k=3$ a $5$)**:
  - Sorteiam-se $k$ indivíduos aleatórios da população e o indivíduo de maior aptidão vence.
  - Oferece controle simples e direto sobre a **pressão seletiva** sem os problemas de saturação ou degeneração de escala da roleta pura.
* **Elitismo**:
  - Preservação do(s) melhor(es) $1$ ou $2$ indivíduos de cada geração diretamente para a próxima população, impedindo a regressão da melhor solução encontrada.

### 6. Tamanho da População e Critérios de Parada
* **Tamanho da População ($N_{pop}$)**:
  - Definido entre $100$ e $300$ indivíduos para instâncias padrão ($8$ a $50$ cidades).
  - Garante variabilidade genética e diversidade de rotas sem elevar demasiadamente a complexidade temporal por época ($O(N_{pop} \cdot N)$).
* **Critérios de Parada**:
  1. **Número máximo de épocas ($E_{max}$)**: ex.: 500 a 1000 gerações.
  2. **Estagnação / Tolerância de Ganho**: Se o melhor fitness não apresentar melhoria significativa por $k_{estag}$ gerações consecutivas (ex.: 50 épocas).
  3. **Convergência Teórica no Benchmark**: Atingimento do valor de perímetro ótimo conhecido no círculo dentro de um limiar $\epsilon$.

---

## 🧪 Cenários de Teste e Benchmarks

### Cenário 1: Pontos Uniformemente Distribuídos
* **Geração**: $N$ pontos distribuídos aleatoriamente no plano 2D sob uma distribuição contínua $U(0, 100) \times U(0, 100)$.
* **Objetivo**: Avaliar a capacidade do algoritmo em desatar nós, evitar cruzamento de arestas e convergir para um circuito harmônico em terrenos irregulares.
* **Acompanhamento**:
  - Curva de convergência: Menor distância vs. Distância média da população por época.
  - Renderização visual das rotas intermediárias (geração 0, gerações intermediárias e geração final).

### Cenário 2: Círculo de Pontos (Benchmark Analítico)
* **Geração**: $N$ pontos equidistantes dispostos ao longo do perímetro de um círculo de raio $R$:
  $$x_i = R \cdot \cos\left(\frac{2\pi i}{N}\right), \quad y_i = R \cdot \sin\left(\frac{2\pi i}{N}\right), \quad i \in \{0, 1, \dots, N-1\}$$
* **Vantagem Científica**: A solução global ótima é **conhecida analiticamente** a priori (é o polígono regular convexo formado pela sequência consecutiva dos pontos).
* **Distância Ótima Conhecida**:
  $$D_{ótimo} = N \cdot 2R \sin\left(\frac{\pi}{N}\right)$$
* **Métrica**: Permite calcular com exatidão a **taxa de erro residual** do AG em relação ao ótimo global:
  $$\text{Erro} = \frac{D_{encontrada} - D_{ótimo}}{D_{ótimo}} \times 100\%$$

### Cenário Bônus: Alta Quantidade de Pontos
* Teste de estresse computacional com grande número de pontos (ex.: $N = 50$, $80$ ou $100$) em configuração circular.
* Anotação rigorosa de:
  - Tempo de execução total até a convergência (segundos).
  - Evolução do circuito em épocas intermediárias (ex.: $10\%$, $25\%$, $50\%$, $75\%$ e $100\%$).
  - Comportamento de escalabilidade temporal e eficácia de desatamento de cruzamentos.

---

## 📊 Métricas de Desempenho e Acompanhamento

A cada execução, o algoritmo exportará e plotará:
1. **Gráfico de Evolução de Fitness / Distância**:
   - Curva da melhor distância (Best Fitness).
   - Curva da distância média da população (Mean Fitness).
   - Desvio padrão das distâncias ao longo das gerações.
2. **Visualização Geométrica da Rota**:
   - Visualização estática em etapas-chave (Geração Inicial, Intermediária e Final).
   - Animação em GIF/MP4 mostrando a evolução da rota se ajustando ao longo das épocas.
3. **Tabela Comparativa de Métricas**:
   - Tempo total decorrido.
   - Época em que a melhor solução foi estabilizada.
   - Distância final vs. Distância teórica ótima (no círculo).

---

## 📂 Arquitetura de Código Proposta

```text
caixeiro-viajante-genetico/
├── README.md                      # Documentação completa do projeto
├── requirements.txt               # Dependências Python (numpy, matplotlib, etc.)
├── .gitignore                     # Arquivos ignorados pelo git
├── src/                           # Código-fonte modular do Algoritmo Genético
│   ├── __init__.py
│   ├── city.py                    # Classe de Ponto/Cidade (coordenadas, distâncias)
│   ├── fitness.py                 # Funções de custo e cálculo de aptidão
│   ├── crossover.py               # Ordered Crossover (OX) e alternativas
│   ├── mutation.py                # Swap e Inversion (2-opt) mutation
│   ├── selection.py               # Torneio, Roleta e Elitismo
│   ├── genetic_algorithm.py       # Orquestrador do ciclo evolutivo
│   └── visualizer.py              # Gráficos de convergência e plot de rotas
├── notebooks/                     # Experimentos e relatórios interativos
│   ├── 01_distribuicao_aleatoria.ipynb
│   ├── 02_benchmark_circular.ipynb
│   └── 03_experimento_bonus_alta_escala.ipynb
└── results/                       # Gráficos, animações e tabelas geradas
    ├── plots/
    └── animations/
```

---

## 🛠️ Tecnologias Utilizadas
* **Linguagem**: Python 3.10+
* **Computação Numérica**: [NumPy](https://numpy.org/)
* **Visualização de Dados**: [Matplotlib](https://matplotlib.org/) / [Seaborn](https://seaborn.pydata.org/)
* **Ambiente Interativo**: [Jupyter Notebook](https://jupyter.org/)

---

## 🚀 Como Executar o Projeto

### Pré-requisitos
Certifique-se de possuir Python 3.10 ou superior instalado em seu ambiente.

### 1. Clonar o repositório
```bash
git clone https://github.com/eloruiz/caixeiro-viajante-genetico.git
cd caixeiro-viajante-genetico
```

### 2. Criar e ativar o ambiente virtual
```bash
python -m venv .venv
source .venv/bin/activate  # Linux / macOS
# ou no Windows: .venv\Scripts\activate
```

### 3. Instalar dependências
```bash
pip install -r requirements.txt
```

---

## 📚 Referências Bibliográficas
1. GOLDBERG, David E. *Genetic Algorithms in Search, Optimization, and Machine Learning*. Addison-Wesley, 1989.
2. AGUIAR NETO, Fernando S. de. *Notas de Aula e Apresentações da disciplina SIAM - Algoritmos Genéticos*. PUC-Campinas.
3. EIBEN, A. E.; SMITH, J. E. *Introduction to Evolutionary Computing*. Springer, 2015.
4. LARRANAGA, P. et al. *Genetic Algorithms for the Travelling Salesman Problem: A Review of Representations and Operators*. Artificial Intelligence Review, 1999.