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

O mecanismo evolutivo da parte 3 está implementado em **C++17**. As instruções de execução, os parâmetros e os testes estão neste README.

---

## 📌 Sumário
1. [Visão Geral do Projeto](#-visão-geral-do-projeto)
2. [Definição do Problema (TSP 2D)](#-definição-do-problema-tsp-2d)
3. [Cenários de Teste](#-cenários-de-teste)
4. [Estrutura do Trabalho (Frentes Técnicas)](#-estrutura-do-trabalho-frentes-técnicas)
5. [Critérios de Avaliação e Entregas](#-critérios-de-avaliação-e-entregas)
6. [Implementação da parte 3](#implementação-da-parte-3)
7. [Compilar e executar](#compilar-e-executar)
8. [Dados para a parte 4](#dados-para-a-parte-4)
9. [Testes e resultados](#testes-e-resultados)

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

## Implementação da parte 3

A evolução integra as funções de distância e aptidão da parte 1 com a representação por permutação e o cruzamento OX da parte 2:

1. Cria a população inicial embaralhando os índices das cidades.
2. Avalia e ordena as rotas pela distância do circuito fechado.
3. Preserva a elite sem alterações na próxima geração.
4. Seleciona pais por torneio com reposição: vence a menor distância, equivalente à maior aptidão `1 / (1 + distância)`.
5. Aplica cruzamento OX e mutação por troca de duas posições distintas, conforme as probabilidades configuradas.
6. Repete o processo, registrando a melhor rota e as distâncias de cada geração.
7. Encerra pelo limite de gerações ou por gerações consecutivas sem melhora estrita.

O elitismo impede que a melhor distância piore. A busca não garante o ótimo global; parar por estagnação significa apenas que não houve melhora durante o intervalo configurado.

### Arquivos

| Arquivo | Responsabilidade |
| --- | --- |
| `include/problema.h`, `src/problema.cpp` | Pontos, cenários, distância e aptidão — parte 1. |
| `include/cruzamento.h`, `src/cruzamento.cpp` | Validação de permutações e cruzamento OX — parte 2. |
| `include/evolucao.h`, `src/evolucao.cpp` | Parâmetros, seleção, mutação, elitismo, gerações e histórico — parte 3. |
| `src/main.cpp` | Execução pelo terminal e exportação dos CSVs. |
| `testes/teste_*.cpp` | Testes do problema, cruzamento, evolução e requisitos. |
| `testes/executar_testes.ps1` | Compilação e execução da suíte no PowerShell. |
| `testes/teste_integracao.ps1` | Verificação da linha de comando e dos CSVs. |
| `Makefile` | Compilação e execução dos testes C++ em ambiente com shell compatível. |

O algoritmo e seus testes funcionais são escritos em C++17. Os scripts PowerShell automatizam a compilação e verificam a integração com os arquivos exportados.

### Parâmetros padrão

| Campo de `ParametrosEvolucao` | Padrão | Justificativa inicial |
| --- | ---: | --- |
| `tamanhoPopulacao` | 100 | Equilíbrio inicial entre diversidade e custo de avaliação. |
| `tamanhoTorneio` | 3 | Favorece boas rotas mantendo variedade na seleção. |
| `quantidadeElite` | 1 | Preserva a melhor rota e permite substituir as demais. |
| `taxaCruzamento` | 0,90 | Favorece recombinação, mantendo a possibilidade de copiar um pai. |
| `taxaMutacao` | 0,10 | Cada filho tem 10% de chance de trocar duas cidades; a taxa é por filho. |
| `maxGeracoes` | 1000 | Limita o custo da busca. |
| `limiteSemMelhora` | 200 | Encerra buscas estagnadas; zero desativa esse critério. |
| `semente` | 42 | Permite repetir experimentos na mesma implementação da biblioteca C++. |

Esses valores são propostas iniciais. A escolha final deve ser apoiada por comparações de configurações e sementes nas partes 4 e 5. Pela API C++, todos os campos podem ser alterados antes de chamar `executarEvolucao(pontos, parametros)`.

São necessários ao menos 8 pontos finitos, população de pelo menos 2, torneio entre 1 e o tamanho da população e elite entre 1 e população menos 1. As taxas devem estar entre 0 e 1, e os limites de gerações não podem ser negativos. Zero gerações retorna somente a população inicial.

## Compilar e executar

É necessário um compilador com suporte a **C++17**. Os comandos abaixo partem da pasta `caixeiro-viajante-genetico`.

### Windows / PowerShell

Neste computador, o g++ está instalado pelo MSYS2 em `C:\msys64\ucrt64\bin`. Se o compilador já estiver no PATH, a primeira linha é dispensável.

```powershell
$env:PATH = 'C:\msys64\ucrt64\bin;' + $env:PATH
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror src/problema.cpp src/cruzamento.cpp src/evolucao.cpp src/main.cpp -o build/ag.exe
cd build
./ag.exe aleatorio 12 42
```

Para executar o cenário circular:

```powershell
./ag.exe circular 12 42
```

### Argumentos do programa

```text
ag [circular|aleatorio] [pontos] [semente] [populacao] [geracoes] [mutacao] [sem_melhora]
```

Todos os argumentos são opcionais, nessa ordem. Sem argumentos, o programa usa cenário circular, 12 pontos e os parâmetros padrão. A CLI aceita sementes inteiras de 0 a 2147483647 e taxas com ponto decimal, como `0.10`.

Exemplo com todos os argumentos:

```powershell
./ag.exe aleatorio 25 123 150 2000 0.15 300
```

Nesse exemplo, são usadas 25 cidades, semente 123, população 150, até 2000 gerações, mutação de 15% por filho e parada após 300 gerações sem melhora. Torneio, elite e taxa de cruzamento são configurados pela API C++, não por argumentos da CLI.

Na CLI, o círculo tem raio 10 e o cenário aleatório usa coordenadas entre 0 e 100 em cada eixo. Para modificar essas dimensões, use as funções de geração de pontos pela API C++ ou ajuste as chamadas em `src/main.cpp`.

## Dados para a parte 4

Cada execução grava dois arquivos **no diretório em que o programa foi executado**:

| Arquivo | Colunas | Uso |
| --- | --- | --- |
| `historico.csv` | `geracao,melhor_distancia,distancia_media,rota` | Gráfico de convergência e acompanhamento da melhor rota por geração. |
| `pontos.csv` | `cidade,x,y` | Coordenadas para desenhar as cidades e ligar a rota. |

A geração 0 é a população inicial. Uma execução de 20 gerações exporta 21 registros de dados. A coluna `rota` contém índices separados por espaços, com cada cidade uma vez; para desenhar o circuito fechado, conecte também a última cidade à primeira.

**Uma nova execução na mesma pasta substitui os CSVs anteriores.** Para comparar sementes e parâmetros, execute em pastas diferentes. Exemplo a partir da raiz do projeto, após compilar:

```powershell
New-Item -ItemType Directory -Force build/experimento_aleatorio_42 | Out-Null
Push-Location build/experimento_aleatorio_42
../ag.exe aleatorio 12 42 100 1000 0.10 200
Pop-Location
```

Na API C++, `ResultadoEvolucao` fornece `melhorRota`, `melhorDistancia`, `motivoParada` e `historico`. Cada `RegistroGeracao` contém o número da geração, a melhor distância, a distância média e a melhor rota daquela geração.

## Experimentos com o benchmark circular e bônus (parte 5)

A experimentação da parte 5 avalia o algoritmo genético no cenário circular com raio 10, comparando as soluções encontradas com a solução analítica ótima do polígono regular convexo:
$$D_{\text{ótimo}} = N \cdot 2R \sin\left(\frac{\pi}{N}\right)$$

### Geração das visualizações e execução automatizada

A execução do benchmark circular e do teste de escala é realizada pelo script Python `experimentos/executar_parte5.py`, dependendo apenas de Python 3 e `matplotlib` (sem necessidade de `pandas`):

```bash
# Execução padrão (gera gráficos, rotas e HTML com amostragem inteligente)
python3 experimentos/executar_parte5.py

# Opcional: gera também a versão HTML completa com todas as 3.000 gerações no bônus
python3 experimentos/executar_parte5.py --completo
```

### Análise dos experimentos circulares (N=12)

Foram executadas as sementes 42, 123 e 456 com 12 cidades em circunferência de raio 10 (parâmetros: população 100, mutação 0,10, parada após 200 gerações sem melhora).

| Semente | Distância inicial | Distância final | Ótimo teórico | Erro relativo | Gerações | Tempo (s) | Parada |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| 42 | 127,6268 | 62,1166 | 62,1166 | 0,0000% | 235 | ~0,01 s | Estagnação |
| 123 | 129,4887 | 62,1166 | 62,1166 | 0,0000% | 212 | ~0,01 s | Estagnação |
| 456 | 122,6670 | 62,1166 | 62,1166 | 0,0000% | 214 | ~0,01 s | Estagnação |

Todas as sementes atingiram o ótimo analítico global com erro relativo de **0,0000%**, confirmando que o operador de cruzamento OX e o elitismo encontram o circuito perimetral convexo ideal em poucas épocas.

Cada pasta de semente em `experimentos/resultados/circular/` contém:
- `historico.csv` e `pontos.csv`: Dados numéricos da evolução e coordenadas dos pontos.
- `grafico_convergencia.png`: Curva da melhor distância e média populacional com a linha de referência do ótimo teórico analítico.
- `rotas_evolucao.png`: Painel comparativo mostrando o circuito inicial desordenado e o polígono final regular.
- `rota_final.png`: Imagem isolada da melhor rota final encontrada.
- `evolucao_rotas.html`: Visualizador interativo offline com reprodução automática, controle de velocidade (0.5x a 5x), alternador de rótulos das cidades e exibição do ótimo teórico e erro relativo em tempo real.

### Bônus: Teste de estresse de escala (N=200 cidades)

Para avaliar o comportamento do algoritmo sob alta complexidade combinatória, executou-se o modelo circular com 200 cidades, população 300, até 3.000 gerações contínuas (`limiteSemMelhora = 0`) e semente 42.

| Métrica | Valor observado |
| :--- | :--- |
| Distância inicial (Geração 0) | 2.307,7720 |
| Distância final (Geração 3000) | 189,9840 |
| Ótimo teórico ($D_{\text{ótimo}}$) | 62,8293 |
| Redução da distância | **91,8%** |
| Tempo de processamento | **~1,07 s** (para 3.000 gerações) |

Os resultados do bônus estão em `experimentos/resultados/bonus/`:
- `grafico_convergencia_bonus.png`: Acompanhamento da curva de convergência ao longo de 3.000 épocas.
- `painel_rotas_intermediarias.png`: Painel sequencial com 5 marcos temporais (0%, 10%, 30%, 60% e 100%), evidenciando visualmente o processo de desatamento progressivo dos cruzamentos de arestas.
- `rota_final.png`: Traçado da melhor rota obtida na geração 3.000.
- `evolucao_rotas.html`: Visualizador interativo com amostragem inteligente (892 frames selecionados com base em melhorias estritas e passos regulares) para reprodução suave no navegador, com botão instrucional para abertura da versão completa caso desejado.

## Testes e resultados

Para compilar o programa e executar todos os testes no PowerShell, a partir da raiz do projeto:

```powershell
$env:PATH = 'C:\msys64\ucrt64\bin;' + $env:PATH
./testes/executar_testes.ps1
```

O script usa `-Wall -Wextra -Wpedantic -Werror` e interrompe em caso de falha. Os binários e as saídas dos testes ficam em `build/`, que é ignorada pelo Git. Eles podem ser recriados pelo script após uma limpeza.

| Conjunto | O que verifica |
| --- | --- |
| `teste_problema.cpp` | Distância, fechamento da rota, aptidão, círculo e pontos uniformes. |
| `teste_cruzamento.cpp` | Validação de permutações, 1000 cruzamentos e pais iguais. |
| `teste_evolucao.cpp` | Histórico, elitismo, reprodução por semente, ótimo circular de 8 cidades, critérios de parada, taxas 0 e 1, população ímpar, 20 sementes e entradas inválidas. |
| `teste_requisitos.cpp` | 48 combinações de cenário/tamanho/parâmetros/semente, cálculo independente do circuito, referência exata para 8 cidades, aptidão, 1000 cruzamentos adicionais e escala de 500 cidades. |
| `teste_integracao.ps1` | Execução dos dois cenários pela CLI, recálculo das distâncias nos CSVs, reprodução do histórico e rejeição de 11 argumentos inválidos. |

A suíte completa passou na validação de **23/10/2026**, usando GCC do MSYS2 e PowerShell. Também foi verificada a rejeição de 12 entradas inválidas na evolução e de 14 nos geradores de pontos.

Em ambientes com make e shell compatível, `make teste` inclui os quatro conjuntos C++; os testes de integração PowerShell são executados pelo script acima. Neste ambiente, `make.exe` abortou antes da compilação com erro de acesso do MSYS2 (`NtCreateDirectoryObject`, `0xC0000022`); a validação efetiva foi feita pelo PowerShell.

### Exemplos observados

Execuções com 12 cidades, semente 42 e demais parâmetros padrão:

| Cenário | Melhor distância inicial | Melhor distância final | Gerações executadas |
| --- | ---: | ---: | ---: |
| Circular, raio 10 | 127,6268 | 62,1166 | 213 |
| Aleatório, limite 100 | 519,3064 | 326,7457 | 223 |

O círculo atingiu o ótimo conhecido na geração 13 e encerrou após 200 gerações sem melhora. O resultado aleatório demonstra redução da distância, sem comprovar ótimo global.

O teste de escala com 500 cidades circulares, população 100 e 50 gerações levou aproximadamente 0,32 s no ambiente usado. A distância caiu de aproximadamente 6064,97 para 4548,47, ainda muito acima do ótimo de 62,8314. Esse teste valida a execução nessa escala; não mede tempo até convergência nem comprova o bônus do trabalho.
