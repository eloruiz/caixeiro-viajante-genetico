"""Atualiza somente os HTMLs existentes a partir dos CSVs, sem executar o C++."""
import csv
import math
import re
from pathlib import Path
from visualizacao import gerar_visualizacao


def main():
    raiz=Path(__file__).resolve().parent/'resultados'
    for arquivo in sorted(raiz.rglob('evolucao_rotas*.html')):
        pasta=arquivo.parent
        if not (pasta/'historico.csv').exists() or not (pasta/'pontos.csv').exists():
            continue
        with (pasta/'historico.csv').open(newline='',encoding='utf-8') as f:
            historico=list(csv.DictReader(f))
        with (pasta/'pontos.csv').open(newline='',encoding='utf-8') as f:
            pontos={int(p['cidade']):(float(p['x']),float(p['y'])) for p in csv.DictReader(f)}
        titulo=re.search(r'<title>(.*?)</title>',arquivo.read_text(encoding='utf-8-sig'))
        titulo=titulo.group(1) if titulo else 'Evolução das rotas'
        # Pontos circulares desta aplicação têm raio 10 e centro na origem.
        circular=all(math.isclose(math.hypot(x,y),10,rel_tol=1e-9) for x,y in pontos.values())
        if not circular and pasta.name.startswith('semente_'):
            titulo=f'Cenário aleatório — Semente {pasta.name.split("_")[-1]}'
        otimo=len(pontos)*20*math.sin(math.pi/len(pontos)) if circular else None
        gerar_visualizacao(pontos,historico,otimo,titulo,arquivo)
        print(arquivo.relative_to(raiz))


if __name__=='__main__':
    main()
