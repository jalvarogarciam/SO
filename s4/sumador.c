#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int suma = 0;
    // Sumar todos los números pasados como argumentos
    for (int i = 1; i < argc; i++) {
        suma += atoi(argv[i]);
    }

    write(STDOUT_FILENO, &suma, sizeof(int));

    return 0;
}

