# Makefile do projeto: compila e roda todos os testes com um comando só.
#
# Como usar (no terminal, dentro da pasta do projeto):
#   make teste    compila e roda todos os testes (para no primeiro que falhar)
#   make limpar   apaga os executáveis gerados na compilação

# Compilador (g++) e opções: padrão C++17 e avisos ligados para pegar erros cedo
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# LISTA DE TESTES (ADICIONAR AQUI): escreva o nome do executável de cada teste novo
TESTES = teste_problema teste_cruzamento

# Regra principal: compila todos os testes da lista e roda um por um.
# O "|| exit 1" faz o make parar assim que um teste falhar.
teste: $(TESTES)
	@for t in $(TESTES); do echo "== $$t =="; ./$$t || exit 1; done

# REGRAS DE COMPILAÇÃO (ADICIONAR AQUI): uma regra por teste novo.
# Formato: nome do executável: arquivos dos quais ele depende
#          (tab) g++ teste.cpp arquivos_do_src.cpp -o nome do executável

# Parte 1 (Elo): modelagem e aptidão
teste_problema: testes/teste_problema.cpp src/problema.cpp include/problema.h
	$(CXX) $(CXXFLAGS) testes/teste_problema.cpp src/problema.cpp -o teste_problema

# Parte 2 (Lucas): representação e cruzamento
teste_cruzamento: testes/teste_cruzamento.cpp src/cruzamento.cpp include/cruzamento.h
	$(CXX) $(CXXFLAGS) testes/teste_cruzamento.cpp src/cruzamento.cpp -o teste_cruzamento

# Apaga os executáveis (eles são gerados de novo a qualquer momento com make teste)
limpar:
	rm -f $(TESTES)

# Diz ao make que "teste" e "limpar" são comandos, e não arquivos
.PHONY: teste limpar
