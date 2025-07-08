#ifndef EDL_H
#define EDL_H

#include <vector>
#include "Registro.h" // Inclui a definição da classe Registro

// Classe que simula uma Estrutura de Dados Linear (EDL), ou "arquivo de registros".
// Armazena objetos Registro completos em um vetor.
class EDL {
private:
    std::vector<Registro> dados; // O "array de registros" em memória principal

public:
    // Construtor padrão.
    EDL();

    // Busca um Registro pela sua posição (índice) no vetor.
    // Retorna uma referência CONSTANTE ao Registro.
    // Lança std::out_of_range se o índice for inválido.
    const Registro& buscarPorPosicao(int indice) const;

    // Insere um novo Registro no final da EDL.
    // Retorna a posição (índice) onde o Registro foi gravado.
    // Tem complexidade O(1) amortizada para std::vector.
    int inserirRegistro(const Registro& novoRegistro);

    // Retorna o número atual de registros na EDL.
    size_t getTamanho() const;

    // Imprime todos os registros armazenados na EDL, incluindo os marcados como deletados.
    void imprimirTodosRegistros() const;

    // Métodos para permitir a modificação de um Registro específico,
    // o que é necessário para 'marcarComoDeletado'.
    // Esta é uma sobrecarga para permitir acesso não-const para modificação.
    Registro& buscarPorPosicaoParaModificacao(int indice);
};

#endif // EDL_H