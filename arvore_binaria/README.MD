# Sistema de Gerenciamento de Registros com ABB e EDL

Este projeto implementa um sistema básico de gerenciamento de registros utilizando uma Estrutura de Dados Linear (EDL) para armazenamento dos registros e uma Árvore Binária de Busca (ABB) como estrutura de índice. A solução simula um sistema de banco de dados onde os dados completos são armazenados sequencialmente e as buscas são otimizadas através de um índice baseado em árvore.

## Requisitos do Projeto

O sistema foi desenvolvido para atender aos seguintes requisitos:

1.  **Arquivo de Registro (EDL)**: Implementado como um `std::vector<Registro>` em memória principal, atuando como uma Estrutura de Dados Linear (EDL). Cada posição contém um `Registro` de dados (simulando uma pessoa com CPF, Nome, Data de Nascimento, Sexo).
2.  **Busca por Posição na EDL**: A EDL possui uma única função de busca por posição (índice do array), com complexidade O(1).
3.  **Registros Não Ordenados na EDL**: Os registros na EDL não possuem qualquer ordenação.
4.  **Arquivo de Índice (ABB)**: Uma Árvore Binária de Busca (ABB) é mantida separadamente como um "arquivo de índice".
    * **Inserção**: Novas inserções de registros de dados são feitas na EDL em tempo O(1) (amortizado com `std::vector::push_back`). A chave do registro (CPF) e a `posição` (índice) onde o registro foi salvo na EDL são então inseridas na ABB.
    * **Remoção**: Quando um registro é apagado, sua chave é removida da ABB. No "arquivo de registros" (EDL), a posição do registro é apenas marcada como `deletado` usando um `flagDeletado` dentro do `Registro`, sem deslocar os demais registros.
5.  **Método de Acesso**:
    * **Busca por Chave**: Dada uma "chave de busca" (CPF), a ABB é consultada primeiro. Se a chave não for encontrada na ABB, indica que o registro não está presente na "base de dados", economizando a "abertura do arquivo" de dados.
    * **Recuperação do Registro**: Se a chave for encontrada na ABB, a "posição" (índice) do registro na EDL é recuperada do nó da ABB. Usando essa posição, o `Registro` completo é recuperado da EDL em O(1) e suas informações são apresentadas.
6.  **Versão Ordenada da EDL**: É possível criar uma nova EDL ordenada pela chave (CPF) através de um percurso em ordem da ABB. Este percurso visita as chaves na sequência ordenada e usa as posições para recuperar os registros correspondentes da EDL original, populando a nova EDL.

## Estrutura do Projeto

O projeto é organizado nas seguintes classes:

* `Registro`: Representa o dado completo de uma pessoa, incluindo um `flagDeletado`.
* `EntradaIndice`: Uma estrutura leve que armazena a `chave` (CPF) e a `posicaoEDL` (índice do `Registro` correspondente na EDL). Esta é a informação que os nós da ABB armazenam.
* `NoABB`: Representa um nó individual da Árvore Binária de Busca. Contém uma `EntradaIndice` e ponteiros para os filhos esquerdo e direito.
* `ABB`: A classe que implementa a Árvore Binária de Busca, gerenciando os `NoABB`s. Lida com inserção, remoção, percursos e busca de posições com base na `EntradaIndice`.
* `EDL`: A Estrutura de Dados Linear, implementada como um `std::vector<Registro>`, que atua como o "arquivo de registro".
* `main.cpp`: O programa principal que orquestra as operações entre a `EDL` e a `ABB`, demonstrando a funcionalidade do sistema.

## Como Compilar e Executar

Para compilar e executar o projeto, siga os passos abaixo:

1.  **Clone o repositório** (assumindo que você o enviará para o GitHub):
    ```bash
    git clone <URL_DO_SEU_REPOSITORIO>
    cd <NOME_DO_SEU_REPOSITORIO>
    ```

2.  **Compilar o Projeto**:
    Você pode usar o `g++` diretamente ou o `Makefile` fornecido.

    **Usando `g++`:**
    ```bash
    g++ -std=c++11 -Wall Registro.cpp ABB.cpp EDL.cpp main.cpp -o sistema_banco_dados
    ```
    * `-std=c++11`: Garante o uso do padrão C++11 para `std::initializer_list` e `nullptr`.
    * `-Wall`: Habilita todos os warnings.
    * `-g`: Inclui informações de depuração (útil para `gdb`).

    **Usando `Makefile` (recomendado):**
    ```bash
    make
    ```

3.  **Executar o Programa**:
    ```bash
    ./sistema_banco_dados
    ```

## Extensões (Para Habilidosos)

### Múltiplas ABBs (Índices Secundários)

Para estender o projeto e ter múltiplas ABBs para diferentes campos (por exemplo, Nome, Data de Nascimento):

1.  **Novas Classes `EntradaIndiceNome`, `EntradaIndiceDataNasc`**:
    * Cada uma armazenaria o campo relevante (`nome`, `dataNascimento`) e a `posicaoEDL`.
    * Cada uma implementaria seu próprio `operator<` baseado no campo específico.
2.  **Novas Instâncias da `ABB`**:
    * No `main.cpp`, você criaria `ABB indiceNome;`, `ABB indiceDataNasc;`.
3.  **Inserção/Remoção em Múltiplos Índices**:
    * Ao inserir um `Registro` na EDL, você também criaria `EntradaIndiceNome` e `EntradaIndiceDataNasc` e as inseriria nas respectivas ABBs.
    * Ao remover, você removeria das múltiplas ABBs.

### Tratamento de Chaves Repetidas (Ex: Data de Nascimento)

Se um campo como `dataNascimento` pode ter valores repetidos para `Registros` distintos, a ABB padrão (que não permite duplicatas na chave) não funcionaria diretamente. Alternativas:

1.  **Armazenar Lista de Posições**: Em vez de `int posicaoEDL`, a `EntradaIndice` (ou sua versão para campos repetidos) poderia armazenar um `std::vector<int> posicoesEDL`. Ao inserir, se a chave já existir, você adiciona a nova posição a este vetor. Ao remover, você removeria a posição específica do vetor. Isso complica a lógica de comparação (`operator<` ainda seria pela chave, mas a igualdade significa adicionar à lista).
2.  **Chave Composta**: Criar uma chave composta única, por exemplo, "DataNascimento_CPF". Isso garantiria a unicidade na ABB, mas a busca inicial seria apenas pela data de nascimento, e depois exigiria uma iteração na EDL (ou nos resultados do nó da ABB) para encontrar o CPF específico.
3.  **Árvores AVL/Rubro-Negras**: Para manter a árvore balanceada e com bom desempenho mesmo com muitas inserções/remoções, considerar a implementação de árvores balanceadas (AVL ou Rubro-Negras) no lugar da ABB simples. O livro "Estruturas de Dados e Seus Algoritmos (3a. ed.)" de Jayme Luiz Szwarcfiter e Lilian Markenzon, Seções 5.3 e 5.4, é uma excelente referência para isso.

Lembre-se que, para a solução atual, a chave de ordenação (CPF) é considerada única. A implementação de chaves repetidas é um desafio adicional que pode ser explorado.

---
**Citação:** O conceito de ABB e suas operações, bem como as ideias de percursos, são amplamente abordadas no livro "Estruturas de Dados e Seus Algoritmos (3a. ed.)" por Jayme Luiz Szwarcfiter e Lilian Markenzon, especialmente no Capítulo 4 sobre Árvores Binárias de Busca[cite: 26, 27, 28, 29, 30]. A estratégia de remoção de um nó com dois filhos, substituindo-o pelo sucessor in-order, também é uma técnica padrão. O uso de `std::vector::push_back` para inserção em O(1) amortizado e acesso por índice em O(1) é uma característica das Estruturas de Dados Lineares baseadas em arrays. O destrutor utilizando um percurso em pós-ordem para liberação de memória é uma prática recomendada para árvores.
