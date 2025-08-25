# Relatório — Laboratório 2: Chamadas de Sistema

---

## Exercício 1 — printf() vs write()

**Traces usados**: `traces/ex1a_trace.txt`, `traces/ex1b_trace.txt`

**Número de syscalls `write`:**
- `ex1a_printf`: 9 chamadas
- `ex1b_write`: 7 chamadas

**Análise**  
O `printf` utiliza buffering em espaço de usuário, podendo acumular várias impressões antes de chamar `write()`. Por isso, gera menos chamadas de sistema do que o esperado.  
Já `write` faz a chamada de sistema diretamente a cada invocação, sendo mais **previsível** e 1:1 com o código.  
Em termos de desempenho, o buffering do `printf` pode reduzir o número de chamadas ao kernel, mas o `write` é mais determinístico.

---

## Exercício 2 — Leitura de Arquivo

**Resultados obtidos com `./ex2_leitura dados/teste1.txt`:**
- **File descriptor:** 3
- **Bytes lidos:** 127
- **Conteúdo:**  
Esta e a primeira linha do arquivo de teste.
Segunda linha pro exercicio 1.
Terceira linha do arquivo.
Ultima linha do arquivo.

**Análise**  
- O EOF é detectado quando `read()` retorna **0**.  
- É essencial checar os retornos de `open`, `read` e `close` para tratar erros (como arquivo inexistente ou falha de leitura).  
- O FD foi **3**, porque 0, 1 e 2 já estão reservados para stdin, stdout e stderr.

---

## Exercício 3 — Contador de Linhas

### Resultados
| BUFFER_SIZE | Linhas | Caracteres | Chamadas read() | Tempo (s) | Média bytes/read |
|-------------|--------|-------------|-----------------|-----------|------------------|
| 16          | 25     | 1300        | 23              | 0.000175  | 56.5             |
| 64          | 25     | 1300        | 21              | 0.000100  | 61.9             |
| 256         | 25     | 1300        | 23              | 0.000140  | 56.5             |
| 1024        | 25     | 1300        | 23              | 0.000087  | 56.5             |

### Análise
- Buffers **menores** → mais chamadas `read()`, aumentando overhead.  
- Nem todas as `read()` retornam o valor do buffer: a última geralmente retorna menos, e no EOF retorna **0**.  
- Quanto maior o buffer, menos chamadas → tendência de melhorar performance.  
- Os tempos podem variar por fatores de sistema, mas o padrão mostra que menos syscalls geralmente aumentam throughput.

---

## Exercício 4 — Cópia de Arquivo

**Resultados obtidos com `./ex4_copia dados/origem.txt dados/destino.txt`:**
- **Bytes copiados:** 1364
- **Operações (read+write):** 12
- **Tempo:** 0.000285 segundos
- **Throughput:** 4673.79 KB/s
- **Diff:** arquivos idênticos (`diff` não produziu saída)

**Análise**  
- O arquivo de destino foi aberto com:  
`open("dados/destino.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)`  
- É importante verificar se `bytes_escritos == bytes_lidos` em cada iteração, pois `write()` pode escrever menos bytes que o pedido.  
- Caso o disco fique cheio, `write()` retorna -1 e `errno = ENOSPC`.  
- O fechamento dos descritores (`close()`) é essencial para liberar recursos e garantir que os dados sejam gravados no disco.

---
