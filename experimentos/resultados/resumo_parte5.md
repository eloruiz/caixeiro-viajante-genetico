# 📊 Resumo Experimental — Parte 5 e Bônus

Tabela comparativa dos experimentos realizados com o modelo circular:

| Experimento                   |   Cidades (N) |   População |   Dist. Inicial |   Dist. Final |   Ótimo Teórico | Erro Relativo (%)   |   Gerações | Tempo (s)   |
|:------------------------------|--------------:|------------:|----------------:|--------------:|----------------:|:--------------------|-----------:|:------------|
| Circular Padrão (Semente 42)  |            12 |         100 |         127.627 |       62.1166 |         62.1166 | 0.0000%             |        235 | 0.0071 s    |
| Circular Padrão (Semente 123) |            12 |         100 |         129.489 |       62.1166 |         62.1166 | -0.0000%            |        212 | 0.0074 s    |
| Circular Padrão (Semente 456) |            12 |         100 |         122.667 |       62.1166 |         62.1166 | 0.0000%             |        214 | 0.0069 s    |
| Bônus Alta Escala (N=200)     |           200 |         300 |        2307.77  |      189.984  |         62.8293 | 202.3814%           |       3000 | 1.1528 s    |

### 🔍 Observações Técnicas:
- **Benchmark N=12:** O algoritmo atinge convergência exata (erro relativo de 0.0000%) em poucos milissegundos, encontrando a rota perimetral convexa ideal.
- **Bônus N=200:** O teste de estresse evidencia o tempo necessário para desatar os cruzamentos internos de alta dimensionalidade em um espaço combinatório de (199! / 2) rotas possíveis.
