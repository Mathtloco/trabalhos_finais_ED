#ifndef ABB_H
#define ABB_H

#include "abb/NoABB.h"
#include <initializer_list> // Para construtores com lista de inicialização
#include <vector>           // Potencialmente usado em percursos ou métodos auxiliares
#include <queue>            // Para percurso em largura
#include <string>           // Para chaves de busca e remoção

// Forward declaration para a classe EDL, necessária para a função friend.
// Permite que o compilador saiba que EDL é uma classe sem a necessidade de incluir "EDL.h" aqui,
// evitando inclusão circular em cenários mais complexos.
class EDL;

// Classe que implementa a estrutura de dados Árvore Binária de Busca (ABB).
// Gerencia a hierarquia de nós (NoABB) e fornece operações de manipulação e percurso.
class ABB {
private:
    NoABB* raiz; // Ponteiro para a raiz da árvore

    // Métodos auxiliares recursivos e privados para gerenciamento interno da ABB.

    // Desaloca recursivamente todos os nós de uma subárvore.
    void destruirRecursivo(NoABB* no);

    // Cria uma cópia profunda (recursiva) de uma subárvore.
    NoABB* copiarRecursivo(const NoABB* noOriginal);

    // Lógica recursiva para inserir uma nova EntradaIndice na subárvore.
    NoABB* inserirRecursivo(NoABB* noAtual, const EntradaIndice& novaEntrada);

    // Lógica recursiva para remover um nó com uma dada chave de uma subárvore.
    NoABB* removerRecursivo(NoABB* noAtual, const std::string& chaveRemover);
    
    // Encontra o nó com a menor chave em uma subárvore (utilizado na remoção).
    NoABB* encontrarMin(NoABB* no);

    // Métodos recursivos para os percursos da árvore.
    void preOrdemRecursivo(NoABB* no) const;
    void emOrdemRecursivo(NoABB* no) const;
    void posOrdemRecursivo(NoABB* no) const;

    // Declara a função global 'gerarEDLOrdenada' como friend para permitir
    // acesso direto à 'raiz' da ABB para fins de percurso.
    // Em um design de produção mais robusto, um iterador ou um método público
    // na ABB para coletar os dados seria preferível.
    friend void gerarEDLOrdenada(const ABB& abbIndices, const EDL& edlOriginal);

public:
    // Construtores da ABB:

    // Construtor padrão: cria uma ABB vazia (raiz nula).
    ABB();

    // Construtor a partir de uma lista de inicialização de EntradaIndice.
    // Insere cada elemento da lista na ABB.
    ABB(std::initializer_list<EntradaIndice> lista);

    // Construtor de cópia: realiza uma cópia profunda de outra ABB.
    ABB(const ABB& outraABB);

    // Operador de atribuição de cópia: permite atribuir uma ABB a outra,
    // garantindo cópia profunda e segurança.
    ABB& operator=(const ABB& outraABB);

    // Destrutor: garante a liberação correta de toda a memória alocada pelos nós.
    ~ABB();

    // Métodos de manipulação da ABB (API pública):

    // Insere uma nova EntradaIndice na ABB.
    void inserir(const EntradaIndice& novaEntrada);

    // Remove um nó da ABB identificado por sua chave (CPF).
    void remover(const std::string& chaveRemover);

    // Deleta todos os nós da ABB, liberando a memória e tornando-a vazia.
    void deletarABB();

    // Métodos para realizar os diferentes tipos de percursos na ABB.
    void preOrdem() const;
    void emOrdem() const; // Percurso em ordem simétrica (elementos em ordem crescente)
    void posOrdem() const;
    void emLargura() const; // Percurso por nível (para os mais habilidosos)

    // Busca a posição na EDL de um Registro dado sua chave (CPF).
    // Retorna a posição (índice) se encontrada, ou -1 caso contrário.
    int buscarPosicao(const std::string& chaveBusca) const;
};

#endif // ABB_H