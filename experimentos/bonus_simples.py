"""Bônus simples: executa o C++, lê os CSVs e gera uma imagem e um resumo.
Uso, na raiz do projeto: python experimentos/bonus_simples.py
Dependência para o gráfico: python -m pip install matplotlib
"""
import csv
import math
import os
from pathlib import Path
import shutil
import subprocess
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Os únicos parâmetros do experimento ficam aqui.
CIDADES = 100
SEMENTE = 52
POPULACAO = 100
MUTACAO = 0.10
SEM_MELHORA = 500
MAX_GERACOES = 20000  # Segurança caso a estagnação não aconteça antes.
RAIZ = Path(__file__).resolve().parent.parent
SAIDA = RAIZ / 'experimentos/resultados/bonus_100'


def main():
    # 1. Compila o mesmo algoritmo C++ usado nos testes normais.
    compilador = shutil.which('g++')
    if not compilador and Path('C:/msys64/ucrt64/bin/g++.exe').exists():
        compilador = 'C:/msys64/ucrt64/bin/g++.exe'
    if not compilador:
        raise SystemExit('Instale g++ com suporte a C++17 e coloque-o no PATH.')
    ambiente = os.environ.copy()
    ambiente['PATH'] = str(Path(compilador).parent) + os.pathsep + ambiente.get('PATH', '')
    (RAIZ / 'build').mkdir(exist_ok=True)
    executavel = RAIZ / 'build' / ('ag.exe' if os.name == 'nt' else 'ag')
    subprocess.run([compilador, '-std=c++17', '-O2', 'src/main.cpp',
                    'src/problema.cpp', 'src/cruzamento.cpp', 'src/evolucao.cpp',
                    '-o', str(executavel)], cwd=RAIZ, env=ambiente, check=True)

    # 2. Executa o círculo de raio 10. O C++ mede o tempo do algoritmo.
    SAIDA.mkdir(parents=True, exist_ok=True)
    argumentos = ['circular', str(CIDADES), str(SEMENTE), str(POPULACAO),
                  str(MAX_GERACOES), str(MUTACAO), str(SEM_MELHORA), str(SAIDA)]
    resultado = subprocess.run([str(executavel)] + argumentos, env=ambiente,
                                capture_output=True, text=True, check=True)
    (SAIDA / 'execucao.txt').write_text('Argumentos: ' + ' '.join(argumentos) +
                                       '\n' + resultado.stdout, encoding='utf-8')
    print(resultado.stdout)

    # 3. Lê e confere as rotas: todas as cidades uma vez e retorno à origem.
    with (SAIDA / 'historico.csv').open(newline='') as f:
        historico = list(csv.DictReader(f))
    with (SAIDA / 'pontos.csv').open(newline='') as f:
        pontos = {int(p['cidade']): (float(p['x']), float(p['y'])) for p in csv.DictReader(f)}
    anterior = math.inf
    for g, registro in enumerate(historico):
        rota = list(map(int, registro['rota'].split()))
        distancia = sum(math.dist(pontos[a], pontos[b]) for a, b in zip(rota, rota[1:] + rota[:1]))
        melhor = float(registro['melhor_distancia'])
        assert int(registro['geracao']) == g
        assert len(rota) == CIDADES and set(rota) == set(pontos)
        assert math.isclose(distancia, melhor, rel_tol=1e-10)
        assert melhor <= anterior + 1e-9
        anterior = melhor

    # 4. Uma imagem: gráfico de desempenho e três rotas para explicar a evolução.
    geracoes = [int(r['geracao']) for r in historico]
    melhores = [float(r['melhor_distancia']) for r in historico]
    otimo = CIDADES * 20 * math.sin(math.pi / CIDADES)
    ultima_melhora = max([0] + [g for g in range(1, len(melhores)) if melhores[g] < melhores[g-1]])
    estagnou = 'Parada: estagnacao' in resultado.stdout
    if estagnou:
        assert geracoes[-1] - ultima_melhora == SEM_MELHORA
    tempo = next(l.split(':', 1)[1].strip() for l in resultado.stdout.splitlines() if l.startswith('Tempo:'))
    reducao = (1 - melhores[-1] / melhores[0]) * 100
    fig = plt.figure(figsize=(12, 8), layout='constrained')
    grade = fig.add_gridspec(2, 3)
    ax = fig.add_subplot(grade[0, :])
    ax.plot(geracoes, melhores, label='Melhor distância', color='#2563eb')
    ax.axhline(otimo, ls='--', color='#16a34a', label=f'Ótimo teórico: {otimo:.2f}')
    if estagnou:
        ax.axvspan(ultima_melhora, geracoes[-1], color='#f59e0b', alpha=.2, label='500 gerações sem melhora')
    ax.set(xlabel='Geração', ylabel='Distância', title=f'Tempo do algoritmo: {tempo} | Redução: {reducao:.2f}%')
    ax.legend(); ax.grid(alpha=.2)
    for coluna, (nome, g) in enumerate([('Inicial', 0), ('Intermediária', geracoes[-1] // 2), ('Final', geracoes[-1])]):
        ax = fig.add_subplot(grade[1, coluna])
        rota = list(map(int, historico[g]['rota'].split()))
        rota.append(rota[0])
        ax.plot([pontos[c][0] for c in rota], [pontos[c][1] for c in rota], lw=1, color='#2563eb')
        ax.scatter(*zip(*pontos.values()), s=8, color='#0f172a')
        ax.set_aspect('equal'); ax.set_xticks([]); ax.set_yticks([])
        ax.set_title(f'{nome} · geração {g}\nDistância: {melhores[g]:.2f}')
    fig.suptitle(f'Bônus simples — {CIDADES} cidades no círculo — semente {SEMENTE}', fontsize=16)
    fig.savefig(SAIDA / 'bonus.png', dpi=150)
    plt.close(fig)

    # Visualização opcional: reproduz os mesmos dados, sem nova execução.
    from visualizacao import gerar_visualizacao
    gerar_visualizacao(pontos, historico, otimo,
                       f'Bônus simples — {CIDADES} cidades — Semente {SEMENTE}',
                       SAIDA / 'evolucao_rotas.html')

    # 5. Resumo curto para a equipe apresentar.
    parada = '500 gerações consecutivas sem melhora' if estagnou else 'teto de gerações; estagnação não confirmada'
    resumo = f'''# Bônus simples: {CIDADES} cidades

## O que fizemos
Usamos o mesmo algoritmo genético em C++, aumentando de 12 para {CIDADES} cidades
em um círculo de raio 10. O Python apenas executa o programa, lê os resultados e gera a imagem.

## Como executar
Na raiz do projeto, com Python, matplotlib e g++ instalados:

```powershell
python -m pip install matplotlib
python experimentos/bonus_simples.py
```

O script compila o C++ automaticamente. Os parâmetros ficam no início do script.
População: {POPULACAO}; semente: {SEMENTE}; mutação: {MUTACAO:.0%} por filho;
OX: 90%; torneio: 3; elite: 1. A semente permite repetir o sorteio no mesmo ambiente.
Mantivemos população e operadores do teste normal para facilitar a comparação.

## Quando termina
Para após {SEM_MELHORA} gerações sem melhora estrita, ou no teto de {MAX_GERACOES} gerações.
Neste teste, o motivo foi: **{parada}**.
Chamamos essa estabilização de convergência operacional; ela não garante o ótimo global.

## Resultado desta execução
- Distância inicial: {melhores[0]:.4f}.
- Distância final: {melhores[-1]:.4f}.
- Redução: {reducao:.2f}%.
- Última melhora: geração {ultima_melhora}; parada: geração {geracoes[-1]}.
- Tempo do algoritmo até a parada: **{tempo}**, medido pelo C++ antes da exportação.
- Ótimo teórico: {otimo:.4f}; erro relativo final: {(melhores[-1]/otimo-1)*100:.2f}%.

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
'''
    (SAIDA / 'LEIA-ME.md').write_text(resumo, encoding='utf-8')
    print(f'Resultados em: {SAIDA}')


if __name__ == '__main__':
    main()
