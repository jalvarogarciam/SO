#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>   // Para open y las banderas O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>  // Para write y close



/* Programa que dicta todos los archivos relevantes de un directorio pasado como 
arvumento, escribe un contenido en el archivo "ejercicio.txt" del directorio y 
por último imprime su contenido.
*/
int main(int argc, char * argv[]){

    if (argc < 3){
        printf("Uso: %s <ruta directorio> <numero entero> <modo>\n", argv[0]);
        return 1;
    }

    const char *dir_path = argv[1];  // "./prueba"  
    int numero = atoi(argv[2]);
    const char * FILE_NAME = "ejercicio.txt";
    int mode = argc == 4 && argv[3][0] == 't' ? O_TRUNC : O_APPEND; 
    // 'a' para append (predeterminado), 't' para truncar


    // Abre el directorio
    DIR *dir = opendir(dir_path);
    if (dir == NULL) { //control de errores
        perror("Error al abrir el directorio");
        exit(-1);
    }

    printf("Contenido del directorio %s:\n\n", dir_path);

    // crea la estructura del directorio para leer todos los archivos
    struct dirent *entry;
    struct stat statbuf;
    
    while ((entry = readdir(dir)) != NULL){

        // Construir la ruta completa a cada archivo
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);

        //obtener informacion del archivo
        if (stat(filepath, &statbuf) == -1){
            perror("Error al obtener informacion del archivo");
            exit(-1);
        }

        // si es un archivo regular, un enlace simbólico o un directorio, imprime su nombre
        if (S_ISREG(statbuf.st_mode) || S_ISDIR(statbuf.st_mode) )
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                printf("%s   ", entry->d_name);

    }printf("\n\n");


    closedir(dir);



    printf("Escribiendo en el archivo %s ... ", FILE_NAME);

    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, FILE_NAME);
    int fd = open(filepath, O_WRONLY | mode);
    if (fd == -1) {
        perror("\nError al abrir el archivo");
        exit(-1);
    }


    char content[1024]; // Escribe en el archivo
    snprintf(content, sizeof(content), "hola, mi número es %d \n", numero);
    if (write(fd, content, strlen(content)) == -1) {
        perror("Error al escribir en el archivo");
        close(fd);
        exit(-1);
    }
    printf("OK\n");

    close(fd);


    printf("Contenido del archivo:\n\n");
    fd = open(filepath, O_RDONLY);
    int bytes_read; // almacenará el número de bytes leidos
    while ((bytes_read = read(fd, content, sizeof(content)))) {
        content[bytes_read] = '\0'; // read no lo hace  de forma automatica
        printf("%s", content);
    }


    close(fd);


    return 0;
}