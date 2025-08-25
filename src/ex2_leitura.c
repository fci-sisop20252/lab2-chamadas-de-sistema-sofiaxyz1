/*
 * Exercício 2 - Leitura Básica de Arquivo
 *
 * OBJETIVO: Implementar leitura de arquivo usando syscalls
 * 1. Compile: gcc src/ex2_leitura.c -o ex2_leitura
 * 2. Execute: ./ex2_leitura dados/teste1.txt
 * 3. Observe: strace -e openat,read,close ./ex2_leitura dados/teste1.txt
 */

#include <fcntl.h>    // open()
#include <unistd.h>   // read(), close()
#include <stdio.h>    // printf(), fprintf()
#include <errno.h>    // errno
#include <string.h>   // strerror()

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 128
#endif

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "dados/teste1.txt";
    char buffer[BUFFER_SIZE];
    ssize_t bytes_lidos;
    int fd;

    printf("=== Exercício 2: Leitura de Arquivo ===\n\n");

    // Abrir o arquivo para leitura
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Erro ao abrir '%s': %s\n", path, strerror(errno));
        return 1;
    }
    printf("Arquivo aberto! File descriptor: %d\n", fd);

    // Ler até BUFFER_SIZE-1 bytes para poder terminar com '\0'
    bytes_lidos = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytes_lidos < 0) {
        fprintf(stderr, "Erro na leitura de '%s': %s\n", path, strerror(errno));
        close(fd);
        return 1;
    }
    buffer[bytes_lidos] = '\0';  // terminador nulo para imprimir como string

    // Exibir resultados
    printf("Bytes lidos: %ld\n", (long)bytes_lidos);
    printf("Conteúdo:\n%s\n", buffer);

    // Fechar o arquivo
    if (close(fd) < 0) {
        fprintf(stderr, "Erro ao fechar '%s': %s\n", path, strerror(errno));
        return 1;
    }
    printf("Arquivo fechado!\n\n");

    printf("Execute também com strace:\n");
    printf("  strace -e openat,read,close ./ex2_leitura %s\n", path);
    return 0;
}
