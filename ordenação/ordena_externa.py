import os
import csv
import tempfile
import heapq


def _tenta_converter_para_numero(valor):
    """Tenta converter um valor para int ou float. Se falhar, retorna o valor original."""
    try:
        return int(valor)
    except ValueError:
        try:
            return float(valor)
        except ValueError:
            return valor


def _divide_em_runs_ordenados(caminho_arquivo_entrada, coluna_chave, ordem, tamanho_buffer_mb, pasta_temporaria):
    """
    Fase 1: Lê o arquivo em chunks, ordena cada chunk em memória e salva em arquivos temporários.
    """
    print("--- Fase 1: Dividindo o arquivo em 'runs' ordenados ---")

    # Determina se a ordenação é reversa (descendente)
    reverso = (ordem.lower() == 'descendente')

    # Converte o tamanho do buffer de MB para bytes
    tamanho_buffer_bytes = tamanho_buffer_mb * 1024 * 1024

    arquivos_temporarios = []

    try:
        with open(caminho_arquivo_entrada, 'r', newline='', encoding='utf-8') as f_entrada:
            leitor = csv.reader(f_entrada)
            cabecalho = next(leitor)

            # Determina o índice da coluna chave
            if isinstance(coluna_chave, str):
                try:
                    indice_chave = cabecalho.index(coluna_chave)
                except ValueError:
                    raise ValueError(
                        f"Coluna '{coluna_chave}' não encontrada no cabeçalho: {cabecalho}")
            else:
                indice_chave = int(coluna_chave)
                if not (0 <= indice_chave < len(cabecalho)):
                    raise IndexError(
                        f"Índice da coluna {indice_chave} está fora do intervalo.")

            run_atual = []
            tamanho_run_atual = 0
            num_run = 0

            for linha in leitor:
                run_atual.append(linha)
                # Estima o tamanho da linha em memória (aproximação)
                tamanho_run_atual += sum(len(s) for s in linha)

                if tamanho_run_atual >= tamanho_buffer_bytes:
                    print(f"  Processando run #{num_run}...")

                    # Ordena o run atual em memória
                    run_atual.sort(key=lambda r: _tenta_converter_para_numero(
                        r[indice_chave]), reverse=reverso)

                    # Salva o run ordenado em um arquivo temporário
                    caminho_temp = os.path.join(
                        pasta_temporaria, f"run_{num_run}.csv")
                    with open(caminho_temp, 'w', newline='', encoding='utf-8') as f_temp:
                        escritor = csv.writer(f_temp)
                        escritor.writerow(cabecalho)
                        escritor.writerows(run_atual)

                    arquivos_temporarios.append(caminho_temp)

                    # Reseta para o próximo run
                    run_atual = []
                    tamanho_run_atual = 0
                    num_run += 1

            # Processa o último run que pode não ter atingido o tamanho do buffer
            if run_atual:
                print(f"  Processando último run #{num_run}...")
                run_atual.sort(key=lambda r: _tenta_converter_para_numero(
                    r[indice_chave]), reverse=reverso)
                caminho_temp = os.path.join(
                    pasta_temporaria, f"run_{num_run}.csv")
                with open(caminho_temp, 'w', newline='', encoding='utf-8') as f_temp:
                    escritor = csv.writer(f_temp)
                    escritor.writerow(cabecalho)
                    escritor.writerows(run_atual)
                arquivos_temporarios.append(caminho_temp)

    except FileNotFoundError:
        raise FileNotFoundError(
            f"Arquivo de entrada não encontrado: {caminho_arquivo_entrada}")
    except Exception as e:
        print(f"Ocorreu um erro durante a fase de divisão: {e}")
        # Limpa arquivos temporários em caso de falha
        for f in arquivos_temporarios:
            os.remove(f)
        raise

    print(f"Total de {len(arquivos_temporarios)} runs criados.")
    return arquivos_temporarios, indice_chave, cabecalho


def _mescla_arquivos(arquivos_de_entrada, caminho_arquivo_saida, indice_chave, reverso):
    """
    Mescla múltiplos arquivos ordenados em um único arquivo de saída ordenado.
    Utiliza uma min-heap (ou max-heap para descendente) para fazer a mesclagem de forma eficiente (k-way merge).
    """
    print(f"  Mesclando {len(arquivos_de_entrada)} arquivos...")

    arquivos_abertos = []
    leitores = []
    min_heap = []

    try:
        # Abre todos os arquivos de entrada e prepara os leitores
        for i, caminho_arquivo in enumerate(arquivos_de_entrada):
            f = open(caminho_arquivo, 'r', newline='', encoding='utf-8')
            arquivos_abertos.append(f)
            leitor = csv.reader(f)
            leitores.append(leitor)

            next(leitor)  # Pula o cabeçalho

            # Adiciona a primeira linha de cada arquivo na heap
            try:
                primeira_linha = next(leitor)
                chave = _tenta_converter_para_numero(
                    primeira_linha[indice_chave])
                # Para ordem descendente, negamos os números para usar uma min-heap como max-heap
                if reverso and isinstance(chave, (int, float)):
                    chave = -chave
                heapq.heappush(min_heap, (chave, i, primeira_linha))
            except StopIteration:
                # Arquivo vazio (além do cabeçalho), apenas ignora
                pass

        # Abre o arquivo de saída
        with open(caminho_arquivo_saida, 'w', newline='', encoding='utf-8') as f_saida:
            escritor = csv.writer(f_saida)

            # Determina o cabeçalho a partir do primeiro arquivo de entrada
            with open(arquivos_de_entrada[0], 'r', newline='', encoding='utf-8') as f_temp:
                cabecalho = next(csv.reader(f_temp))
                escritor.writerow(cabecalho)

            # Processa a heap até que ela esteja vazia
            while min_heap:
                # Pega o menor elemento (do topo da heap)
                chave, indice_arquivo, linha = heapq.heappop(min_heap)

                # Escreve a linha no arquivo de saída
                escritor.writerow(linha)

                # Pega a próxima linha do mesmo arquivo de onde a linha anterior veio
                try:
                    proxima_linha = next(leitores[indice_arquivo])
                    proxima_chave = _tenta_converter_para_numero(
                        proxima_linha[indice_chave])
                    if reverso and isinstance(proxima_chave, (int, float)):
                        proxima_chave = -proxima_chave
                    heapq.heappush(
                        min_heap, (proxima_chave, indice_arquivo, proxima_linha))
                except StopIteration:
                    # O arquivo terminou, não há mais linhas para adicionar
                    pass

    finally:
        # Garante que todos os arquivos abertos sejam fechados
        for f in arquivos_abertos:
            f.close()


def ordenacao_externa(caminho_arquivo_entrada, coluna_chave, ordem='ascendente', tamanho_buffer_mb=100):
    """
    Procedimento principal de Ordenação Externa usando o algoritmo Merge-Sort.

    Args:
        caminho_arquivo_entrada (str): Caminho para o arquivo CSV a ser ordenado.
        coluna_chave (str or int): Nome ou índice da coluna para ordenação.
        ordem (str): 'ascendente' ou 'descendente'.
        tamanho_buffer_mb (int): Tamanho máximo do buffer em Megabytes para ordenação em memória.
    """
    if not os.path.exists(caminho_arquivo_entrada):
        print(
            f"Erro: Arquivo de entrada '{caminho_arquivo_entrada}' não encontrado.")
        return

    # Cria um diretório temporário para os runs
    pasta_temporaria = tempfile.mkdtemp(prefix="ordenacao_externa_")
    print(f"Pasta temporária criada em: {pasta_temporaria}")

    try:
        # --- Fase 1: Divisão e Ordenação Interna ---
        runs_ordenados, indice_chave, cabecalho = _divide_em_runs_ordenados(
            caminho_arquivo_entrada, coluna_chave, ordem, tamanho_buffer_mb, pasta_temporaria
        )

        if not runs_ordenados:
            print("Arquivo de entrada está vazio ou contém apenas o cabeçalho.")
            return

        # --- Fase 2: Mesclagem Externa (Merge) ---
        print("\n--- Fase 2: Mesclando os 'runs' ---")
        reverso = (ordem.lower() == 'descendente')

        # A implementação aqui mescla todos os runs de uma só vez (k-way merge),
        # que é mais eficiente do que mesclar em pares repetidamente.

        # Define o nome do arquivo de saída
        base, ext = os.path.splitext(caminho_arquivo_entrada)
        caminho_arquivo_saida = f"{base}_ordenado{ext}"

        _mescla_arquivos(
            runs_ordenados, caminho_arquivo_saida, indice_chave, reverso)

        print(f"\n--- Ordenação Concluída ---")
        print(f"Arquivo final ordenado salvo em: {caminho_arquivo_saida}")

    except Exception as e:
        print(f"Ocorreu um erro fatal durante a ordenação: {e}")

    finally:
        # --- Limpeza ---
        print("\n--- Limpando arquivos temporários ---")
        for arquivo in os.listdir(pasta_temporaria):
            os.remove(os.path.join(pasta_temporaria, arquivo))
        os.rmdir(pasta_temporaria)
        print("Limpeza concluída.")


if __name__ == '__main__':
    # --- Parâmetros de Entrada ---
    # Use 'dataset_pequeno.csv' para um teste rápido

    datasets = {'ids': {
        0: 'dataset_grande.csv', 1: 'id_aluno'},
        'ids_test': {
        0: 'dataset_pequeno.csv', 1: 'id_aluno'}}

    DATASET = 'ids_test'
    ORDEM = 'descendente'

    ARQUIVO_DE_ENTRADA, COLUNA_CHAVE = datasets[
        DATASET][0], datasets[DATASET][1]
    TAMANHO_BUFFER_MB = 50  # Reduza este valor para simular menos RAM disponível

    # Verifica se o arquivo de entrada existe. Se não, sugere criá-lo.
    if not os.path.exists(ARQUIVO_DE_ENTRADA):
        print(f"Arquivo '{ARQUIVO_DE_ENTRADA}' não encontrado.")
        print(
            "Execute 'python gerar_csv_grande.py' primeiro para criar o arquivo de dados.")
    else:
        # Executa o procedimento de ordenação externa
        ordenacao_externa(
            caminho_arquivo_entrada=ARQUIVO_DE_ENTRADA,
            coluna_chave=COLUNA_CHAVE,
            ordem=ORDEM,
            tamanho_buffer_mb=TAMANHO_BUFFER_MB
        )
