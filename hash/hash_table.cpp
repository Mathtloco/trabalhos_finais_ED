#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
using namespace std;

// Estrutura de um no encadeado
class Node {
public:
    string key;       // campo chave (por exemplo: cpf ou id)
    string value;     // campo valor (por exemplo: nome)
    Node* next;

    Node(string k, string v) : key(k), value(v), next(nullptr) {}
};

// Classe da tabela hash com encadeamento externo
class HashTable {
private:
    vector<Node*> table; // vetor de ponteiros para listas
    size_t size;         // tamanho da tabela
    function<size_t(const string&)> dispersionFunction; // funcao de hash

public:
    // Construtor recebe o tamanho e a funcao de hash
    HashTable(size_t sz, function<size_t(const string&)> func)
        : size(sz), dispersionFunction(func) {
        table.resize(size, nullptr); // inicializa a tabela com nulos
    }

    // Insere um novo par (chave, valor), se nao for duplicata
    void insert(const string& key, const string& value) {
        if (!search(key).empty()) return; // nao insere duplicata

        size_t index = dispersionFunction(key) % size;
        Node* newNode = new Node(key, value);

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* current = table[index];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Retorna o valor associado a uma chave (ou string vazia se nao existir)
    string search(const string& key) const {
        size_t index = dispersionFunction(key) % size;
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key)
                return current->value;
            current = current->next;
        }
        return "";
    }

    // Remove um par (chave, valor) se a chave existir
    bool remove(const string& key) {
        size_t index = dispersionFunction(key) % size;
        Node* current = table[index];
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (previous == nullptr) {
                    table[index] = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }

    // Imprime todos os pares (chave, valor) armazenados
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                cout << current->key << " " << current->value << endl;
                current = current->next;
            }
        }
    }

    // Destrutor libera a memoria dos nos
    ~HashTable() {
        for (size_t i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current) {
                Node* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
    }
};

// Funcao de dispersao (hash) baseada em divisao
size_t hashDivisao(const string& key) {
    return stoull(key); // assume que a chave e numerica
}

// Funao que elimina duplicatas de um arquivo CSV
HashTable eliminateDuplicate(const string& dataset_path,
                              function<size_t(const string&)> dispersionFunc) {
    HashTable table_hash(120, dispersionFunc);
    ifstream file(dataset_path);
    string line;

    getline(file, line); // pula o cabecalho

    while (getline(file, line)) {
        stringstream ss(line);
        string key, value;
        getline(ss, key, ',');
        getline(ss, value, ',');

        table_hash.insert(key, value); // so insere se nao for duplicado
    }

    return table_hash;
}

// Funcao principal
int main() {
    string path = "dados.csv"; // nome do arquivo CSV

    // Cria a tabela hash usando a funcao de divisao
    HashTable tabela = eliminateDuplicate(path, hashDivisao);

    cout << "Chaves unicas (sem duplicatas):\n";
    tabela.print(); // imprime os dados sem repeticoes

    return 0;
}
