#include "ABB.h"
#include <iostream>
#include <algorithm> // Para uso futuro de std::min/max, se necessário em algoritmos mais complexos
#include <functional> // Para std::function, se usado em lambdas ou callbacks

// 1. Construtores

// Construtor padrão: Inicializa a raiz como nullptr, criando uma árvore vazia.
ABB::ABB() : raiz(nullptr) {
    // Apenas para depuração: std::cout << "ABB vazia criada." << std::endl;
}

// Construtor a partir de initializer_list: Popula a árvore com os elementos da lista.
ABB::ABB(std::initializer_list<EntradaIndice> lista) : raiz(nullptr) {
    // Apenas para depuração: std::cout << "ABB sendo criada a partir de initializer_list..." << std::endl;
    for (const auto& entrada : lista) {
        inserir(entrada); // Usa o método público de inserção
    }
    // Apenas para depuração: std::cout << "ABB criada a partir de initializer_list." << std::endl;
}

// Método auxiliar recursivo para o destrutor: Percorre a árvore em pós-ordem e deleta cada nó.
void ABB::destruirRecursivo(NoABB* no) {
    if (no) {
        destruirRecursivo(no->esq); // Primeiro destrói a subárvore esquerda
        destruirRecursivo(no->dir); // Depois destrói a subárvore direita
        delete no;                   // Finalmente, deleta o próprio nó
    }
}

// Destrutor: Chama o método auxiliar para desalocar toda a memória.
ABB::~ABB() {
    // Apenas para depuração: std::cout << "Destrutor da ABB chamado. Liberando memória..." << std::endl;
    destruirRecursivo(raiz); // Inicia a destruição a partir da raiz
    raiz = nullptr;          // Garante que a raiz seja nula após a destruição
    // Apenas para depuração: std::cout << "Memória da ABB liberada." << std::endl;
}

// Método auxiliar recursivo para o construtor de cópia e operador de atribuição:
// Cria uma cópia profunda da subárvore original.
NoABB* ABB::copiarRecursivo(const NoABB* noOriginal) {
    if (noOriginal == nullptr) {
        return nullptr; // Caso base: subárvore vazia
    }
    // Cria um novo nó, copia os dados e recursivamente copia as subárvores.
    NoABB* novoNo = new NoABB(noOriginal->dado);
    novoNo->esq = copiarRecursivo(noOriginal->esq);
    novoNo->dir = copiarRecursivo(noOriginal->dir);
    return novoNo;
}

// Construtor de cópia: Constrói uma nova ABB como uma cópia profunda de outra ABB.
ABB::ABB(const ABB& outraABB) : raiz(nullptr) {
    // Apenas para depuração: std::cout << "Construtor de cópia da ABB chamado." << std::endl;
    raiz = copiarRecursivo(outraABB.raiz); // Inicia a cópia profunda
}

// Operador de atribuição de cópia: Permite que uma ABB existente seja atribuída a outra.
// Garante auto-atribuição e segurança na liberação/cópia.
ABB& ABB::operator=(const ABB& outraABB) {
    // Apenas para depuração: std::cout << "Operador de atribuição de cópia da ABB chamado." << std::endl;
    if (this != &outraABB) { // Verifica auto-atribuição
        // 1. Libera os recursos existentes no objeto atual
        destruirRecursivo(raiz);
        raiz = nullptr;

        // 2. Copia os recursos da outra ABB
        raiz = copiarRecursivo(outraABB.raiz);
    }
    return *this; // Retorna uma referência para o próprio objeto
}

// 3. Inserção de um Registro (EntradaIndice) na ABB

// Método público para iniciar o processo de inserção.
void ABB::inserir(const EntradaIndice& novaEntrada) {
    raiz = inserirRecursivo(raiz, novaEntrada);
}

// Método auxiliar recursivo para inserção: Lógica principal da inserção na ABB.
NoABB* ABB::inserirRecursivo(NoABB* noAtual, const EntradaIndice& novaEntrada) {
    // Caso base: Se o nó atual é nulo, encontramos o local para inserir.
    // Cria um novo nó e o retorna, tornando-o a nova raiz da subárvore (ou um novo filho).
    if (noAtual == nullptr) {
        // Apenas para depuração: std::cout << "Inserindo chave no indice: " << novaEntrada.getChave() << std::endl;
        return new NoABB(novaEntrada);
    }

    // Compara a nova entrada com o dado do nó atual para decidir se vai para a esquerda ou direita.
    // Usa o operador < sobrecarregado na EntradaIndice.
    if (novaEntrada < noAtual->dado) {
        noAtual->esq = inserirRecursivo(noAtual->esq, novaEntrada);
    } else if (novaEntrada > noAtual->dado) {
        noAtual->dir = inserirRecursivo(noAtual->dir, novaEntrada);
    } else {
        // Caso de chave duplicada: A ABB não permite chaves iguais para EntradaIndice.
        std::cout << "ERRO: Entrada de indice com chave " << novaEntrada.getChave() << " já existe na ABB." << std::endl;
    }

    // Retorna o nó atual (que pode ter sido a nova raiz da subárvore após uma inserção,
    // ou o nó original se a inserção ocorreu em uma de suas subárvores).
    return noAtual;
}

// 4. Remoção de um Registro (EntradaIndice) identificado por uma chave

// Método público para iniciar o processo de remoção.
void ABB::remover(const std::string& chaveRemover) {
    // Apenas para depuração: std::cout << "Tentando remover chave de indice: " << chaveRemover << std::endl;
    raiz = removerRecursivo(raiz, chaveRemover);
}

// Método auxiliar recursivo para remoção: Lógica principal da remoção na ABB.
NoABB* ABB::removerRecursivo(NoABB* noAtual, const std::string& chaveRemover) {
    // Caso base 1: Se o nó atual é nulo, a chave não foi encontrada na árvore.
    if (noAtual == nullptr) {
        std::cout << "ERRO: Chave de indice " << chaveRemover << " não encontrada na ABB para remoção." << std::endl;
        return nullptr;
    }

    // Compara a chave a ser removida com a chave do nó atual para navegar na árvore.
    if (chaveRemover < noAtual->dado.getChave()) {
        noAtual->esq = removerRecursivo(noAtual->esq, chaveRemover); // Continua buscando na subárvore esquerda
    } else if (chaveRemover > noAtual->dado.getChave()) {
        noAtual->dir = removerRecursivo(noAtual->dir, chaveRemover); // Continua buscando na subárvore direita
    } else { // A chave a ser removida foi encontrada no noAtual
        
        // Caso 1: Nó a ser removido não tem filho esquerdo (ou é uma folha)
        if (noAtual->esq == nullptr) {
            NoABB* temp = noAtual->dir; // O filho direito (ou nullptr) substitui o nó atual
            delete noAtual;             // Deleta o nó atual
            // Apenas para depuração: std::cout << "Chave de indice " << chaveRemover << " removida (caso 0 ou 1 filho)." << std::endl;
            return temp;                // Retorna o substituto
        } 
        // Caso 2: Nó a ser removido não tem filho direito
        else if (noAtual->dir == nullptr) {
            NoABB* temp = noAtual->esq; // O filho esquerdo substitui o nó atual
            delete noAtual;             // Deleta o nó atual
            // Apenas para depuração: std::cout << "Chave de indice " << chaveRemover << " removida (caso 1 filho)." << std::endl;
            return temp;                // Retorna o substituto
        }
        
        // Caso 3: Nó a ser removido tem DOIS filhos
        // Encontra o sucessor in-order: o menor nó na subárvore direita do nó atual.
        // O sucessor in-order sempre terá 0 ou 1 filho.
        NoABB* temp = encontrarMin(noAtual->dir); 
        
        // Copia o dado (chave e posição) do sucessor para o nó atual.
        // O nó atual logicamente "assume" os dados do sucessor.
        noAtual->dado = temp->dado;
        
        // Remove recursivamente o sucessor da subárvore direita.
        // Agora, o problema de remover 'temp' cairá no Caso 1 ou 2.
        noAtual->dir = removerRecursivo(noAtual->dir, temp->dado.getChave());
        // Apenas para depuração: std::cout << "Chave de indice " << chaveRemover << " removida (substituída por sucessor)." << std::endl;
    }
    return noAtual; // Retorna o nó atual (que foi atualizado ou mantido)
}

// Método auxiliar para encontrar o nó com a menor chave em uma subárvore.
// É usado na remoção para encontrar o sucessor in-order.
NoABB* ABB::encontrarMin(NoABB* no) {
    // O nó com a menor chave em uma ABB está sempre no final do caminho mais à esquerda.
    while (no->esq != nullptr) {
        no = no->esq;
    }
    return no;
}

// 5. Deleção completa de uma ABB

// Libera toda a memória alocada pelos nós da ABB, tornando-a vazia.
void ABB::deletarABB() {
    std::cout << "Deletando toda a ABB de indices..." << std::endl;
    destruirRecursivo(raiz); // Reutiliza o método auxiliar de destruição
    raiz = nullptr;          // Garante que a raiz seja nula após a deleção
    std::cout << "ABB de indices deletada." << std::endl;
}

// Percursos na ABB

// Percurso em Pré-Ordem: (Raiz, Esquerda, Direita)
void ABB::preOrdem() const {
    std::cout << "\nPercurso em Pré-Ordem (Chave, Pos): ";
    preOrdemRecursivo(raiz);
    std::cout << std::endl;
}

void ABB::preOrdemRecursivo(NoABB* no) const {
    if (no) {
        no->dado.imprimir();      // Visita a raiz (imprime o dado da EntradaIndice)
        preOrdemRecursivo(no->esq); // Percorre a subárvore esquerda
        preOrdemRecursivo(no->dir); // Percorre a subárvore direita
    }
}

// Percurso em Ordem (Simétrica): (Esquerda, Raiz, Direita)
// Imprime os elementos em ordem crescente de suas chaves.
void ABB::emOrdem() const {
    std::cout << "\nPercurso em Ordem (Chave, Pos) - Ordenado por Chave: ";
    emOrdemRecursivo(raiz);
    std::cout << std::endl;
}

void ABB::emOrdemRecursivo(NoABB* no) const {
    if (no) {
        emOrdemRecursivo(no->esq);  // Percorre a subárvore esquerda
        no->dado.imprimir();       // Visita a raiz (imprime o dado da EntradaIndice)
        emOrdemRecursivo(no->dir);  // Percorre a subárvore direita
    }
}

// Percurso em Pós-Ordem: (Esquerda, Direita, Raiz)
// Útil para liberar a memória de forma segura (filhos antes dos pais).
void ABB::posOrdem() const {
    std::cout << "\nPercurso em Pós-Ordem (Chave, Pos): ";
    posOrdemRecursivo(raiz);
    std::cout << std::endl;
}

void ABB::posOrdemRecursivo(NoABB* no) const {
    if (no) {
        posOrdemRecursivo(no->esq);  // Percorre a subárvore esquerda
        posOrdemRecursivo(no->dir);  // Percorre a subárvore direita
        no->dado.imprimir();       // Visita a raiz (imprime o dado da EntradaIndice)
    }
}

// Percurso em Largura: Visita os nós nível por nível.
void ABB::emLargura() const {
    std::cout << "\nPercurso em Largura (Chave, Pos): " << std::endl;
    if (raiz == nullptr) {
        std::cout << "Árvore de índices vazia." << std::endl;
        return;
    }

    std::queue<NoABB*> fila; // Usa uma fila para gerenciar a ordem de visita
    fila.push(raiz);         // Começa com a raiz

    while (!fila.empty()) {
        NoABB* noAtual = fila.front(); // Pega o nó da frente da fila
        fila.pop();                    // Remove-o da fila

        noAtual->dado.imprimir();      // Visita o nó (imprime o dado da EntradaIndice)
        // std::cout << std::endl; // Opcional: quebra de linha após cada nó para visualização vertical

        // Adiciona os filhos à fila para serem visitados em seguida
        if (noAtual->esq != nullptr) {
            fila.push(noAtual->esq);
        }
        if (noAtual->dir != nullptr) {
            fila.push(noAtual->dir);
        }
    }
    std::cout << std::endl;
}

// Método de busca para encontrar a posição de um registro na EDL dado seu CPF.
// Retorna a posição (int) se encontrada, ou -1 se a chave não existir.
int ABB::buscarPosicao(const std::string& chaveBusca) const {
    NoABB* noAtual = raiz;
    while (noAtual != nullptr) {
        // Compara a chave buscada com a chave do nó atual.
        if (chaveBusca < noAtual->dado.getChave()) {
            noAtual = noAtual->esq; // Se menor, vai para a esquerda
        } else if (chaveBusca > noAtual->dado.getChave()) {
            noAtual = noAtual->dir; // Se maior, vai para a direita
        } else {
            // Chave encontrada! Retorna a posição na EDL armazenada no nó.
            return noAtual->dado.getPosicaoEDL();
        }
    }
    // Se o loop terminar e o nó atual for nullptr, a chave não foi encontrada.
    return -1;
}