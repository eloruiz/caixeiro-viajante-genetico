# 📊 Resumo Experimental — Parte 5 e Bônus

Tabela comparativa dos experimentos realizados com o modelo circular:

| Experimento | N | Pop | Inicial | Final | Ótimo | Erro (%) | Gerações | Tempo (s) |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Circular (Semente 42) | 12 | 100 | 127.6268 | 62.1166 | 62.1166 | 0.0000% | 235 | 0.0140 |
| Circular (Semente 123) | 12 | 100 | 129.4887 | 62.1166 | 62.1166 | 0.0000% | 212 | 0.0128 |
| Circular (Semente 456) | 12 | 100 | 122.6670 | 62.1166 | 62.1166 | 0.0000% | 214 | 0.0132 |
| Bônus Alta Escala (N=200) | 200 | 300 | 2307.7720 | 189.9840 | 62.8293 | 202.3814% | 3000 | 1.0926 |

### 🔍 Principais Observações Técnicas:
1. **Convergência Perfeita no Benchmark ($N=12$):** Todas as 3 sementes atingiram **0.0000% de erro relativo**, encontrando exatamente o polígono regular convexo analítico em poucas gerações.
2. **Desatamento de Cruzamentos no Bônus ($N=200$):** O teste de alta escala demonstra o AG reduzindo drasticamente a distância caótica inicial e desatando progressivamente os nós internos.
3. **Visualizadores HTML Offline:** Cada experimento possui `evolucao_rotas.html` interativo nos mesmos padrões da Parte 4, com suporte a visualização suave e métricas em tempo real.
