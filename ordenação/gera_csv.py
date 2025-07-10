
import csv
import random
import string
import os


def gerar_nome_aleatorio(tamanho=8):
    """Gera uma string aleatória para nomes."""
    letras = string.ascii_lowercase
    return ''.join(random.choice(letras) for i in range(tamanho)).capitalize()


def gerar_csv_grande(nome_arquivo, num_linhas, tamanho_mb_desejado=None):
    """
    Gera um arquivo CSV grande para testes de ordenação externa.

    Args:
        nome_arquivo (str): Nome do arquivo a ser criado.
        num_linhas (int): Número de linhas/registros a serem gerados.
        tamanho_mb_desejado (int, optional): Se fornecido, gera linhas até 
                                             atingir aproximadamente este tamanho.
    """
    print(f"Gerando arquivo de teste: {nome_arquivo}...")

    # Criamos uma lista de IDs e a embaralhamos para garantir que o arquivo não esteja ordenado
    ids = list(range(1, num_linhas + 1))
    random.shuffle(ids)

    cabecalho = ['id_aluno', 'nome', 'email']

    try:
        with open(nome_arquivo, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(cabecalho)

            for i in range(num_linhas):
                id_aluno = ids[i]
                nome = gerar_nome_aleatorio()
                email = f"{nome.lower()}{id_aluno}@exemplo.com"
                writer.writerow([id_aluno, nome, email])

                # Relatório de progresso e verificação de tamanho
                if (i + 1) % 100000 == 0:
                    print(f"  {i+1}/{num_linhas} linhas escritas...")
                    if tamanho_mb_desejado:
                        tamanho_atual_mb = os.path.getsize(
                            nome_arquivo) / (1024 * 1024)
                        if tamanho_atual_mb >= tamanho_mb_desejado:
                            print(
                                f"Tamanho de {tamanho_mb_desejado:.2f} MB atingido. Interrompendo.")
                            break

        tamanho_final_mb = os.path.getsize(nome_arquivo) / (1024 * 1024)
        print(f"Arquivo '{nome_arquivo}' gerado com sucesso!")
        print(f"Tamanho final: {tamanho_final_mb:.2f} MB")
        print(f"Total de linhas: {i+1}")

    except IOError as e:
        print(f"Erro ao escrever no arquivo {nome_arquivo}: {e}")


if __name__ == '__main__':
    # Exemplo de uso: gerar um arquivo com 5 milhões de linhas
    # (Isso deve gerar um arquivo de aproximadamente 150-200 MB)
    # Para um teste maior, aumente o número de linhas.
    # Para um arquivo > 4GB, use num_linhas = 100_000_000
    gerar_csv_grande('dataset_grande.csv', num_linhas=5_000_000)

    # Exemplo para gerar um arquivo menor para testes rápidos de lógica
    #gerar_csv_grande('dataset_pequeno.csv', num_linhas=1000)
