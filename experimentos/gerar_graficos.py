
from pathlib import Path

import csv
import json
import matplotlib.pyplot as plt

PASTA_PROJETO = Path(__file__).resolve().parent.parent
PASTA_RESULTADOS = PASTA_PROJETO / "experimentos" / "resultados"
sementes = [42, 123, 456]

for semente in sementes:
    pasta = PASTA_RESULTADOS / f"semente_{semente}"
    arquivo_historico = pasta / "historico.csv"
    arquivo_pontos = pasta / "pontos.csv"

    if not arquivo_historico.exists() or not arquivo_pontos.exists():
        print(f"Arquivos ausentes na pasta: {pasta}")
        continue

    with arquivo_historico.open(encoding="utf-8", newline="") as entrada:
        registros = list(csv.DictReader(entrada))
    with arquivo_pontos.open(encoding="utf-8", newline="") as entrada:
        pontos = list(csv.DictReader(entrada))
    historico = {chave: [float(r[chave]) for r in registros]
                 for chave in ("geracao", "melhor_distancia", "distancia_media")}

    if not registros or not pontos:
        print(f"Dados vazios na pasta: {pasta}")
        continue

    # Gráfico de convergência
    plt.figure(figsize=(10, 6))
    plt.plot(
        historico["geracao"],
        historico["melhor_distancia"],
        label="Melhor distância",
        linewidth=2
    )
    plt.plot(
        historico["geracao"],
        historico["distancia_media"],
        label="Distância média da população",
        linewidth=1.5
    )
    plt.title(f"Evolução do Algoritmo Genético - Semente {semente}")
    plt.xlabel("Geração")
    plt.ylabel("Distância da rota")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(pasta / "grafico_convergencia.png", dpi=300)
    plt.close()

    # Recupera a melhor rota registrada na última geração
    registro_final = dict(registros[-1])
    registro_final["melhor_distancia"] = float(registro_final["melhor_distancia"])
    rota = [
        int(cidade)
        for cidade in str(registro_final["rota"]).split()
    ]

    # Fecha o percurso retornando à primeira cidade
    rota_fechada = rota + [rota[0]]

    coordenadas = {int(p["cidade"]): (float(p["x"]), float(p["y"])) for p in pontos}
    x = [coordenadas[cidade][0] for cidade in rota_fechada]
    y = [coordenadas[cidade][1] for cidade in rota_fechada]

    # Desenha a rota final
    plt.figure(figsize=(9, 7))
    plt.plot(x, y, "-o", linewidth=1.8, markersize=6)

    for cidade in rota:
        cx, cy = coordenadas[cidade]
        plt.annotate(
            str(cidade),
            (cx, cy),
            xytext=(5, 5),
            textcoords="offset points"
        )

    plt.title(
        f"Melhor Rota Final - Semente {semente}\n"
        f"Distância: {registro_final['melhor_distancia']:.4f}"
    )
    plt.xlabel("Coordenada X")
    plt.ylabel("Coordenada Y")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")
    plt.tight_layout()
    plt.savefig(pasta / "rota_final.png", dpi=300)
    plt.close()

    # Apenas a apresentação HTML; os gráficos e CSVs permanecem iguais.
    from visualizacao import gerar_visualizacao
    gerar_visualizacao(coordenadas, registros, None,
                       f"Cenário aleatório — Semente {semente}",
                       pasta / "evolucao_rotas.html")

    print(f"\nSemente: {semente}")
    print(f"Distância final: {registro_final['melhor_distancia']:.4f}")
    print(f"Rota final: {' -> '.join(map(str, rota_fechada))}")
    print(f"Arquivos gerados em: {pasta}")

print("\nGráficos e rotas gerados com sucesso.")
