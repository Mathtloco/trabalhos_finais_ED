#ifndef NO_ABB_H
#define NO_ABB_H

#include "abb/EntradaIndice.h" // Inclui a definição da classe EntradaIndice

// Classe que representa um nó individual em uma Árvore Binária de Busca (ABB).
// Cada nó armazena uma EntradaIndice e ponteiros para seus filhos.
class NoABB {
public:
    EntradaIndice dado; // O dado (EntradaIndice) armazenado neste nó
    NoABB* esq;         // Ponteiro para o filho esquerdo
    NoABB* dir;         // Ponteiro para o filho direito

    // Construtor: inicializa o nó com uma EntradaIndice e filhos nulos.
    NoABB(const EntradaIndice& entrada) : dado(entrada), esq(nullptr), dir(nullptr) {}

    // Destrutor padrão: a liberação de memória dos nós filhos é gerenciada pela classe ABB.
    ~NoABB() = default;
};

#endif // NO_ABB_H