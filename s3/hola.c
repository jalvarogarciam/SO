#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>

int main() {
    // Nombre del archivo
    const char* nombreArchivo = "hola_mundo.txt";
    
    sleep(5);

    // Intenta abrir el archivo en modo de escritura
    int fd = open(nombreArchivo, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // Verifica si el archivo se abrió correctamente
    if (fd<0) {
        printf("Error al abrir el archivo.\n");
        exit(-1);  // Salida con error
    }

    // Escribe "Hola Mundo" en el archivo
    write(fd, "Hola Mundo", 10);
    
    // Cierra el archivo
    close(fd);

    printf("Se ha escrito 'Hola Mundo' en el archivo '%s'.\n", nombreArchivo);

    return 0;  // Salida exitosa
}

