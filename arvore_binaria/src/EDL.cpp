#include "EDL.h"
#include <iostream>
#include <stdexcept> // Para std::out_of_range

// Construtor padrão.
EDL::EDL() {
    // Apenas para depuração: std::cout << "EDL criada." << std::endl;
}

// Implementação da busca por posição para acesso constante.
const Registro& EDL::buscarPorPosicao(int indice) const {
    // Validação do índice para evitar acesso fora dos limites.
    if (indice < 0 || static_cast<size_t>(indice) >= dados.size()) {
        throw std::out_of_range("EDL::buscarPorPosicao (const): Indice fora dos limites.");
    }
    return dados[indice]; // Retorna uma referência constante ao Registro.
}

// Implementação da busca por posição para modificação (não-constante).
// Necessária para métodos como 'marcarComoDeletado'.
Registro& EDL::buscarPorPosicaoParaModificacao(int indice) {
    // Validação do índice para evitar acesso fora dos limites.
    if (indice < 0 || static_cast<size_t>(indice) >= dados.size()) {
        throw std::out_of_range("EDL::buscarPorPosicaoParaModificacao: Indice fora dos limites.");
    }
    return dados[indice]; // Retorna uma referência não-constante ao Registro.
}

// Implementação da inserção de um novo registro.
int EDL::inserirRegistro(const Registro& novoRegistro) {
    dados.push_back(novoRegistro); // Adiciona o registro ao final do vetor.
    return dados.size() - 1;       // Retorna o índice onde o registro foi inserido.
}

// Retorna o tamanho atual da EDL.
size_t EDL::getTamanho() const {
    return dados.size();
}

// Imprime todos os registros na EDL.
void EDL::imprimirTodosRegistros() const {
    std::cout << "\n--- Conteúdo da EDL (Arquivo de Registros) ---" << std::endl;
    if (dados.empty()) {
        std::cout << "EDL vazia." << std::endl;
        return;
    }
    for (size_t i = 0; i < dados.size(); ++i) {
        std::cout << "Pos [" << i << "]: ";
        dados[i].imprimir(); // Chama o método imprimir do Registro.
    }
    std::cout << "------------------------------------------" << std::endl;
}