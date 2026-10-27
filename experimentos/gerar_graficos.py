
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

    # Todos os registros, inclusive geracoes sem melhora, ficam navegaveis offline.
    dados = {"pontos": coordenadas, "historico": registros}
    pagina = '''<!doctype html><html lang="pt-BR"><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Evolução das rotas</title><style>
body{font:18px system-ui;max-width:850px;margin:30px auto;padding:16px}
svg{width:100%;max-height:65vh;background:#f5f7fa}input{width:100%}
</style><h1>Evolução da melhor rota — semente SEMENTE</h1>
<p>Selecione qualquer geração ou reproduza a evolução. O circuito retorna à primeira cidade.</p>
<button id="play">Reproduzir</button><label for="geracao">Geração</label>
<input id="geracao" type="range" min="0" value="0" step="1">
<p id="estado" aria-live="polite"></p><svg viewBox="0 0 620 620" role="img" aria-label="Rota fechada entre as cidades"></svg>
<script>const dados=DADOS;const slider=document.querySelector('input');
const svg=document.querySelector('svg');let timer=null;
slider.max=dados.historico.length-1;
const pontos=Object.values(dados.pontos), xs=pontos.map(p=>p[0]),ys=pontos.map(p=>p[1]);
const xmin=Math.min(...xs),ymin=Math.min(...ys);
const escala=540/Math.max(Math.max(...xs)-xmin,Math.max(...ys)-ymin,1);
function xy(id){const p=dados.pontos[id];return [40+(p[0]-xmin)*escala,580-(p[1]-ymin)*escala]}
function desenhar(){const r=dados.historico[Number(slider.value)];
const rota=r.rota.trim().split(/\\s+/);const fechada=[...rota,rota[0]];
svg.innerHTML='<polyline fill="none" stroke="#2563eb" stroke-width="2" points="'+fechada.map(id=>xy(id).join(',')).join(' ')+'"/>'+
rota.map(id=>{const [x,y]=xy(id);return `<circle cx="${x}" cy="${y}" r="5" fill="#16233a"/><text x="${x+7}" y="${y-7}" font-size="14">${id}</text>`}).join('');
document.querySelector('#estado').textContent=`Geração ${r.geracao} | Melhor distância: ${Number(r.melhor_distancia).toFixed(4)} | Média: ${Number(r.distancia_media).toFixed(4)}`;}
slider.oninput=desenhar;document.querySelector('#play').onclick=()=>{
if(timer){clearInterval(timer);timer=null;document.querySelector('#play').textContent='Reproduzir';return}
if(Number(slider.value)===Number(slider.max))slider.value=0;
document.querySelector('#play').textContent='Pausar';timer=setInterval(()=>{
slider.value=Number(slider.value)+1;desenhar();if(Number(slider.value)>=Number(slider.max)){clearInterval(timer);timer=null;document.querySelector('#play').textContent='Reproduzir'}},120)};desenhar();
</script></html>'''
    (pasta / "evolucao_rotas.html").write_text(
        pagina.replace("SEMENTE", str(semente)).replace("DADOS", json.dumps(dados)), encoding="utf-8")

    print(f"\nSemente: {semente}")
    print(f"Distância final: {registro_final['melhor_distancia']:.4f}")
    print(f"Rota final: {' -> '.join(map(str, rota_fechada))}")
    print(f"Arquivos gerados em: {pasta}")

print("\nGráficos e rotas gerados com sucesso.")
