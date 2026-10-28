"""Executa comparacoes reproduziveis; requer build/ag.exe previamente compilado."""
import csv
import math
from pathlib import Path
import statistics
import subprocess

RAIZ = Path(__file__).resolve().parent.parent
DESTINO = RAIZ / "experimentos" / "resultados" / "comparacao"
CONFIGURACOES = [(50, .10), (100, .10), (200, .10), (100, .05), (100, .20)]


def validar(pasta):
    with (pasta / "pontos.csv").open(newline="") as arquivo:
        pontos = {int(p["cidade"]): (float(p["x"]), float(p["y"]))
                  for p in csv.DictReader(arquivo)}
    with (pasta / "historico.csv").open(newline="") as arquivo:
        historico = list(csv.DictReader(arquivo))
    assert historico, f"Historico vazio: {pasta}"
    anterior = math.inf
    for geracao, r in enumerate(historico):
        rota = list(map(int, r["rota"].split()))
        assert int(r["geracao"]) == geracao
        assert len(rota) == len(pontos) and set(rota) == set(pontos)
        distancia = sum(math.dist(pontos[a], pontos[b])
                        for a, b in zip(rota, rota[1:] + rota[:1]))
        melhor = float(r["melhor_distancia"])
        assert math.isclose(distancia, melhor, rel_tol=1e-10)
        assert melhor <= anterior + 1e-9
        assert float(r["distancia_media"]) + 1e-9 >= melhor
        anterior = melhor
    return historico


def main():
    linhas = []
    for populacao, mutacao in CONFIGURACOES:
        for semente in (42, 123, 456):
            pasta = DESTINO / f"pop_{populacao}_mut_{mutacao:.2f}_semente_{semente}"
            pasta.mkdir(parents=True, exist_ok=True)
            executavel = (RAIZ / "build" / "ag") if (RAIZ / "build" / "ag").exists() else (RAIZ / "build" / "ag.exe")
            comando = [str(executavel), "aleatorio", "12", str(semente),
                       str(populacao), "1000", str(mutacao), "200"]
            resultado = subprocess.run(comando, cwd=pasta, capture_output=True, text=True, check=True)
            (pasta / "execucao.txt").write_text("Comando: " + " ".join(comando[1:]) + "\n" + resultado.stdout, encoding="utf-8")
            h = validar(pasta)
            motivo = next(l.split(": ", 1)[1] for l in resultado.stdout.splitlines() if l.startswith("Parada:"))
            linhas.append(dict(populacao=populacao, mutacao=mutacao, semente=semente,
                               inicial=h[0]["melhor_distancia"], final=h[-1]["melhor_distancia"],
                               geracoes=h[-1]["geracao"], parada=motivo))
    with (DESTINO / "resumo.csv").open("w", newline="", encoding="utf-8") as arquivo:
        escritor = csv.DictWriter(arquivo, fieldnames=list(linhas[0]))
        escritor.writeheader()
        escritor.writerows(linhas)
    for semente in (42, 123, 456):
        h = validar(DESTINO.parent / f"semente_{semente}")
        base = next(r for r in linhas if r["populacao"] == 100 and r["mutacao"] == .10 and r["semente"] == semente)
        assert float(base["final"]) == float(h[-1]["melhor_distancia"]), "Resultado original nao reproduzido"
    for pop, mut in CONFIGURACOES:
        grupo = [r for r in linhas if r["populacao"] == pop and r["mutacao"] == mut]
        print(pop, mut, "distancia media", round(statistics.mean(float(r["final"]) for r in grupo), 4),
              "geracoes medias", round(statistics.mean(int(r["geracoes"]) for r in grupo), 1))
    print("PASSOU: 15 execucoes, historicos completos e reproducao das 3 sementes originais.")


if __name__ == "__main__":
    main()
