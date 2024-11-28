#include <stdio.h>
#include <unistd.h>

int main() {
    // Nombre del archivo
    const char* nombreArchivo = "hola_mundo.txt";
    
    sleep(5);

    // Intenta abrir el archivo en modo de escritura
    FILE* archivo = fopen(nombreArchivo, "w");

    // Verifica si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;  // Salida con error
    }

    // Escribe "Hola Mundo" en el archivo
    fprintf(archivo, "Hola Mundo\n");

    // Cierra el archivo
    fclose(archivo);

    printf("Se ha escrito 'Hola Mundo' en el archivo '%s'.\n", nombreArchivo);

    return 0;  // Salida exitosa
}

