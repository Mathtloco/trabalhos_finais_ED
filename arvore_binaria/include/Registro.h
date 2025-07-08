#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include <iostream>
#include <iomanip> // Para std::setw e std::left

// Classe que representa um registro de dados completo de uma pessoa.
// Contém informações como CPF, nome, data de nascimento, sexo e um flag de deleção.
class Registro {
private:
    std::string cpf;
    std::string nome;
    std::string dataNascimento;
    char sexo;
    bool flagDeletado; // Indica se o registro está logicamente deletado

public:
    // Construtor padrão: inicializa um registro como deletado.
    Registro() : cpf(""), nome(""), dataNascimento(""), sexo(' '), flagDeletado(true) {}

    // Construtor completo: para criar um novo registro não deletado.
    Registro(const std::string& cpf, const std::string& nome,
             const std::string& dataNascimento, char sexo)
        : cpf(cpf), nome(nome), dataNascimento(dataNascimento), sexo(sexo), flagDeletado(false) {}

    // Getters para acessar os campos do registro.
    std::string getCpf() const { return cpf; }
    std::string getNome() const { return nome; }
    std::string getDataNascimento() const { return dataNascimento; }
    char getSexo() const { return sexo; }
    bool isDeletado() const { return flagDeletado; }

    // Marca o registro como logicamente deletado.
    void marcarComoDeletado() {
        flagDeletado = true;
        // Opcional: Limpar outros campos para economizar memória ou por segurança
        nome = "";
        dataNascimento = "";
        sexo = ' ';
    }
    
    // Restaura/reutiliza um registro, preenchendo-o com novos dados e marcando-o como não deletado.
    void restaurarRegistro(const std::string& newCpf, const std::string& newNome,
                           const std::string& newDataNascimento, char newSexo) {
        cpf = newCpf;
        nome = newNome;
        dataNascimento = newDataNascimento;
        sexo = newSexo;
        flagDeletado = false;
    }

    // Imprime os detalhes do registro no console.
    void imprimir() const {
        if (flagDeletado) {
            std::cout << "[REGISTRO DELETADO]" << std::endl;
        } else {
            std::cout << "CPF: " << std::setw(15) << std::left << cpf
                      << "Nome: " << std::setw(30) << std::left << nome
                      << "Data Nasc.: " << std::setw(15) << std::left << dataNascimento
                      << "Sexo: " << sexo << std::endl;
        }
    }
};

#endif // REGISTRO_H