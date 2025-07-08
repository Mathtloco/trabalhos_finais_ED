#ifndef ENTRADA_INDICE_H
#define ENTRADA_INDICE_H

#include <string>
#include <iostream>

// Classe que representa uma entrada em um nó da Árvore Binária de Busca (ABB).
// Contém a chave de ordenação (CPF) e a posição correspondente na EDL.
// Esta classe é leve e usada para as comparações na ABB.
class EntradaIndice {
private:
    std::string chave; // CPF: a chave de ordenação usada pela ABB
    int posicaoEDL;    // Índice na Estrutura de Dados Linear (EDL) onde o Registro completo está

public:
    // Construtor padrão: inicializa com valores vazios/nulos.
    EntradaIndice() : chave(""), posicaoEDL(-1) {}

    // Construtor completo: para criar uma entrada com chave e posição.
    EntradaIndice(const std::string& chave, int posicaoEDL)
        : chave(chave), posicaoEDL(posicaoEDL) {}

    // Construtor para busca: permite criar uma EntradaIndice apenas com a chave para comparações.
    explicit EntradaIndice(const std::string& chave)
        : chave(chave), posicaoEDL(-1) {}

    // Getters para acessar a chave e a posição.
    std::string getChave() const { return chave; }
    int getPosicaoEDL() const { return posicaoEDL; }

    // Sobrecargas dos operadores de comparação.
    // Essenciais para a ABB determinar a ordem dos nós, baseadas na 'chave' (CPF).
    bool operator<(const EntradaIndice& outra) const {
        return chave < outra.chave;
    }

    bool operator==(const EntradaIndice& outra) const {
        return chave == outra.chave;
    }

    bool operator!=(const EntradaIndice& outra) const {
        return chave != outra.chave;
    }

    bool operator>(const EntradaIndice& outra) const {
        return chave > outra.chave;
    }

    bool operator<=(const EntradaIndice& outra) const {
        return chave <= outra.chave;
    }

    bool operator>=(const EntradaIndice& outra) const {
        return chave >= outra.chave;
    }

    // Imprime a chave e a posição da entrada do índice no console.
    void imprimir() const {
        std::cout << "(Chave: " << chave << ", Pos: " << posicaoEDL << ") ";
    }
};

#endif // ENTRADA_INDICE_H