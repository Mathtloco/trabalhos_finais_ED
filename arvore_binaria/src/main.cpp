#include "Registro.h"
#include "abb/EntradaIndice.h"
#include "abb/NoABB.h"
#include "abb/ABB.h"
#include "EDL.h"
#include <limits>     // Para std::numeric_limits
#include <functional> // Para std::function (lambdas recursivas)
#include <iostream>   // Para std::cout, std::cin

// Função auxiliar para limpar o buffer de entrada (se necessário para interação com o usuário)
void limparBufferEntrada() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Função que implementa o Requisito 6:
// Cria uma nova Estrutura de Dados Linear (EDL) contendo os Registros
// ordenados pela chave (CPF), utilizando um percurso em ordem da ABB.
void gerarEDLOrdenada(const ABB& abbIndices, const EDL& edlOriginal) {
    std::cout << "\n*** Simulação do requisito 6: Gerando EDL Ordenada por CPF ***" << std::endl;
    
    // Cria uma nova instância da EDL para armazenar os registros ordenados.
    EDL novaEDLOrdenada; 

    // Lambda recursiva que simula o percurso em ordem da ABB.
    // Para cada nó visitado na ABB, ela busca o Registro correspondente na EDL original
    // e o insere na 'novaEDLOrdenada' se não estiver marcado como deletado.
    std::function<void(NoABB*)> popularEDLOrdenada =
        [&](NoABB* no) {
        if (no) {
            // 1. Percorre a subárvore esquerda
            popularEDLOrdenada(no->esq);

            // 2. Visita o nó atual: recupera o Registro da EDL original e o adiciona à nova EDL.
            // Verifica se a posição é válida e se o registro não está logicamente deletado.
            if (no->dado.getPosicaoEDL() != -1 && 
                static_cast<size_t>(no->dado.getPosicaoEDL()) < edlOriginal.getTamanho()) 
            {
                // Usa 'buscarPorPosicao' const para acesso, pois não modificamos o original aqui.
                const Registro& regNaEDL = edlOriginal.buscarPorPosicao(no->dado.getPosicaoEDL());
                if (!regNaEDL.isDeletado()) {
                    novaEDLOrdenada.inserirRegistro(regNaEDL);
                }
            }

            // 3. Percorre a subárvore direita
            popularEDLOrdenada(no->dir);
        }
    };
    
    // Inicia o percurso da ABB a partir da raiz.
    // Acesso direto a 'abbIndices.raiz' é permitido devido à declaração 'friend' em ABB.h.
    popularEDLOrdenada(abbIndices.raiz); 

    // Imprime o conteúdo da nova EDL ordenada.
    novaEDLOrdenada.imprimirTodosRegistros();
    std::cout << "*** Fim da geração da EDL Ordenada ***" << std::endl;
}


int main() {
    // Cria instâncias da EDL (Arquivo de Registros) e da ABB (Arquivo de Índices).
    EDL arquivoRegistros; // Armazena os objetos Registro completos.
    ABB arquivoIndices;   // Armazena chaves (CPF) e posições na EDL para busca rápida.

    std::cout << "--- Inserindo Registros na EDL e na ABB de Indices ---" << std::endl;
    
    // Demonstração de inserção de múltiplos registros:
    // Para cada registro, ele é inserido na EDL, e então sua chave e posição são inseridas na ABB.
    
    Registro r1("111.111.111-11", "Alice Silva", "01/01/1990", 'F');
    int pos1 = arquivoRegistros.inserirRegistro(r1); // Insere na EDL e obtém a posição
    arquivoIndices.inserir(EntradaIndice(r1.getCpf(), pos1)); // Insere chave e posição na ABB

    Registro r2("333.333.333-33", "Carlos Mendes", "03/03/1992", 'M');
    int pos2 = arquivoRegistros.inserirRegistro(r2);
    arquivoIndices.inserir(EntradaIndice(r2.getCpf(), pos2));

    Registro r3("222.222.222-22", "Bob Oliveira", "02/02/1991", 'M');
    int pos3 = arquivoRegistros.inserirRegistro(r3);
    arquivoIndices.inserir(EntradaIndice(r3.getCpf(), pos3));

    Registro r4("555.555.555-55", "Eva Souza", "05/05/1994", 'F');
    int pos4 = arquivoRegistros.inserirRegistro(r4);
    arquivoIndices.inserir(EntradaIndice(r4.getCpf(), pos4));

    Registro r5("444.444.444-44", "Daniel Costa", "04/04/1993", 'M');
    int pos5 = arquivoRegistros.inserirRegistro(r5);
    arquivoIndices.inserir(EntradaIndice(r5.getCpf(), pos5));

    Registro r6("000.000.000-00", "Zeroberto Santos", "00/00/0000", 'O');
    int pos6 = arquivoRegistros.inserirRegistro(r6);
    arquivoIndices.inserir(EntradaIndice(r6.getCpf(), pos6));

    // Tentativa de inserir CPF duplicado na ABB (deve gerar erro, pois CPF é chave única).
    Registro r_dup("333.333.333-33", "Carlos Duplicado", "03/03/1992", 'M');
    int pos_dup = arquivoRegistros.inserirRegistro(r_dup); // EDL aceita, mas ABB recusará no índice
    arquivoIndices.inserir(EntradaIndice(r_dup.getCpf(), pos_dup));
    std::cout << "----------------------------------------------------" << std::endl;

    // Exibe o conteúdo atual da EDL e a estrutura da ABB (em ordem).
    arquivoRegistros.imprimirTodosRegistros();
    arquivoIndices.emOrdem(); // Mostra as chaves e suas posições na EDL, ordenadas por CPF.

    // 5. Implementação do Método de Acesso por Chave (CPF)
    std::cout << "\n--- Testando Acesso por Chave (CPF) ---" << std::endl;
    std::string cpfBusca = "222.222.222-22"; // CPF que existe
    int posicaoEncontrada = arquivoIndices.buscarPosicao(cpfBusca); // Busca a posição na ABB

    if (posicaoEncontrada != -1) { // Se a chave foi encontrada na ABB
        std::cout << "Registro com CPF " << cpfBusca << " encontrado na posicao EDL: " << posicaoEncontrada << std::endl;
        // Recupera o Registro completo da EDL usando a posição.
        const Registro& regEncontrado = arquivoRegistros.buscarPorPosicao(posicaoEncontrada);
        std::cout << "Detalhes do Registro: ";
        regEncontrado.imprimir(); // Imprime o Registro.
    } else {
        std::cout << "Registro com CPF " << cpfBusca << " NÃO encontrado na base de dados (não está no índice)." << std::endl;
    }

    cpfBusca = "999.999.999-99"; // CPF que não existe
    posicaoEncontrada = arquivoIndices.buscarPosicao(cpfBusca);
    if (posicaoEncontrada != -1) { // Se a chave foi encontrada na ABB
        std::cout << "Registro com CPF " << cpfBusca << " encontrado na posicao EDL: " << posicaoEncontrada << std::endl;
        const Registro& regEncontrado = arquivoRegistros.buscarPorPosicao(posicaoEncontrada);
        std::cout << "Detalhes do Registro: ";
        regEncontrado.imprimir();
    } else {
        std::cout << "Registro com CPF " << cpfBusca << " NÃO encontrado na base de dados (não está no índice)." << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    // Testando Remoção de Registros
    std::cout << "\n--- Testando Remoção de Registros ---" << std::endl;
    std::string cpfRemover;
    int posRemover; // Declarada fora dos blocos if para ter o escopo correto.

    // Remover Carlos (333.333.333-33) - Nó com 0 ou 1 filho (dependendo da árvore)
    cpfRemover = "333.333.333-33";
    posRemover = arquivoIndices.buscarPosicao(cpfRemover); // Busca na ABB para obter a posição
    if (posRemover != -1) {
        // Marca o Registro na EDL como deletado.
        arquivoRegistros.buscarPorPosicaoParaModificacao(posRemover).marcarComoDeletado();
        arquivoIndices.remover(cpfRemover); // Remove a EntradaIndice da ABB.
        std::cout << "Registro com CPF " << cpfRemover << " marcado como deletado na EDL e removido do indice da ABB." << std::endl;
    } else {
        std::cout << "CPF " << cpfRemover << " não encontrado para remoção na ABB." << std::endl;
    }
    
    // Remover Alice (111.111.111-11) - Nó com dois filhos (dependendo da árvore)
    cpfRemover = "111.111.111-11";
    posRemover = arquivoIndices.buscarPosicao(cpfRemover);
    if (posRemover != -1) {
        arquivoRegistros.buscarPorPosicaoParaModificacao(posRemover).marcarComoDeletado();
        arquivoIndices.remover(cpfRemover);
        std::cout << "Registro com CPF " << cpfRemover << " marcado como deletado na EDL e removido do indice da ABB." << std::endl;
    } else {
        std::cout << "CPF " << cpfRemover << " não encontrado para remoção na ABB." << std::endl;
    }

    // Tentar remover um CPF que não existe na ABB (já foi removido ou nunca existiu).
    cpfRemover = "999.999.999-99";
    posRemover = arquivoIndices.buscarPosicao(cpfRemover);
    if (posRemover != -1) {
        arquivoRegistros.buscarPorPosicaoParaModificacao(posRemover).marcarComoDeletado();
        arquivoIndices.remover(cpfRemover);
        std::cout << "Registro com CPF " << cpfRemover << " marcado como deletado na EDL e removido do indice da ABB." << std::endl;
    } else {
        std::cout << "CPF " << cpfRemover << " não encontrado para remoção na ABB (provavelmente já removido ou nunca inserido)." << std::endl;
    }

    std::cout << "-------------------------------------" << std::endl;
    arquivoRegistros.imprimirTodosRegistros(); // Mostrar EDL após marcações de deleção
    arquivoIndices.emOrdem();                 // Mostrar ABB após remoções de índices

    // Tentar buscar um registro que foi marcado como deletado na EDL mas cujo índice ainda existe (ou foi substituído na ABB).
    // Nota: Se a EntradaIndice foi removida da ABB, buscar esse CPF resultará em "NÃO encontrado".
    // Se a lógica da ABB mantivesse o nó para chaves duplicadas (não é o nosso caso), a busca o encontraria e mostraria "DELETADO".
    std::cout << "\n--- Tentando acessar registro deletado (já removido do índice) ---" << std::endl;
    cpfBusca = "333.333.333-33"; // Carlos, que foi removido do índice
    posicaoEncontrada = arquivoIndices.buscarPosicao(cpfBusca);
    if (posicaoEncontrada != -1) {
        std::cout << "Registro com CPF " << cpfBusca << " encontrado no indice (via ABB). Detalhes: ";
        const Registro& reg = arquivoRegistros.buscarPorPosicao(posicaoEncontrada);
        reg.imprimir(); // Deverá mostrar [REGISTRO DELETADO] se o índice ainda fosse acessível
    } else {
        std::cout << "Registro com CPF " << cpfBusca << " NÃO encontrado na ABB (removido do indice)." << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;

    // Chama a função para gerar e imprimir a EDL ordenada.
    gerarEDLOrdenada(arquivoIndices, arquivoRegistros);

    std::cout << "\nFim do programa principal." << std::endl;
    return 0;
}