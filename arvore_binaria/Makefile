CXX = g++
CXXFLAGS = -std=c++11 -Wall -g # -g para informações de debug (para depurador)

# Define os diretórios de origem e include
SRCDIR = src
INCLUDEDIR = include
INCLUDES = -I$(INCLUDEDIR) -I$(INCLUDEDIR)/abb # Adiciona include/ e include/abb/ ao caminho de busca de cabeçalhos

# Define o diretório de build
BUILDDIR = build

# Lista dos arquivos .cpp que de fato contêm código fora dos .h
# Estes estão agora na pasta src/
SRCS = $(SRCDIR)/ABB.cpp $(SRCDIR)/EDL.cpp $(SRCDIR)/main.cpp

# Cria os nomes dos arquivos objeto na pasta build
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Nome do executável final, que também será colocado na pasta build
TARGET = $(BUILDDIR)/sistema_banco_dados

# .PHONY é uma diretiva para alvos que não correspondem a nomes de arquivos reais.
.PHONY: all clean builddir

# Regra principal 'all': cria o diretório build e depois o executável
all: builddir $(TARGET)

# Cria o diretório build se ele não existir
builddir:
	@mkdir -p $(BUILDDIR)

# Regra para linkar os arquivos objeto e criar o executável.
# $^ representa todos os pré-requisitos (os arquivos .o).
# $@ representa o alvo (o executável).
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Regra genérica para compilar arquivos .cpp em .o, colocando-os na pasta build.
# $< é o nome do arquivo-fonte (o .cpp).
# $@ é o nome do arquivo objeto (o .o).
# -c compila o .cpp em .o sem linkar.
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regra 'clean': remove todos os arquivos objeto e o executável da pasta build.
clean:
	@rm -rf $(BUILDDIR)