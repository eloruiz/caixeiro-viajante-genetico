"""Apresentação offline dos CSVs. Não executa nem altera o algoritmo genético."""
import html
import json
from pathlib import Path


def gerar_visualizacao(coordenadas, historico, otimo, titulo, destino,
                       tem_versao_completa=False):
    dados = {'pontos': coordenadas, 'historico': historico, 'otimo': otimo}
    modelo = Path(__file__).with_name('visualizador.html').read_text(encoding='utf-8')
    pagina = modelo.replace('__TITULO__', html.escape(titulo))
    pagina = pagina.replace('__DADOS__', json.dumps(dados).replace('<', '\\u003c'))
    pagina = pagina.replace('__COMPLETO__', '<a href="evolucao_rotas_completo.html">Abrir histórico completo ↗</a>' if tem_versao_completa else '')
    Path(destino).write_text(pagina, encoding='utf-8')
