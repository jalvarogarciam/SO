#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>   // Para open y las banderas O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>  // Para write y close



/* Crea un programa que reciba 2 argumentos:
    <ruta directorio> <numero entero>
Abrir el directorio e iterar sobre los archivos (imprimiendolos por pantalla)
    pista: usar readdir, opendir, struct dirent
Añadir en el archivo ejercicio.txt "hola, mi número es {numero entero arg}", 
sobreescribiendo el contenido.
    pista: usar write, open, close, struct stat
IMPORTANTE: gestion de errores (perror)
*/

/* Modificacion:
leer info actual del archivo y concatenar el mensaje
*/

int main(int argc, char * argv[]){

    if (argc != 4){
        printf("Uso: %s <ruta directorio> <numero entero> <modo a(append)/t(truncate))>\n", argv[0]);
        return 1;
    }
    const char *path = argv[1];  // "./prueba"  
    int numero = atoi(argv[2]);
    const char * FILE_NAME = "ejercicio.txt";
    char mode = argv[3][0]; // 'a' para append, 't' para truncar

    // Abre el directorio
    DIR *dir = opendir(path);
    if (dir == NULL) { //control de errores
        perror("Error al abrir el directorio");
        return 2;
    }

    // crea la estructura del directorio para leer todos los archivos
    struct dirent *entry;
    struct stat statbuf;
    char filepath[1024];   //ruta completa al archivo
    do{
        entry = readdir(dir);

        // Construir la ruta completa al archivo
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);

        //obtener informacion del archivo
        if (stat(filepath, &statbuf) == -1){
            perror("Error al obtener informacion del archivo");
            return 3;
        }
    }while ((readdir(dir)) != NULL && strcmp(entry->d_name, FILE_NAME) != 0 );


    closedir(dir);

    printf("Escribiendo en el archivo %s ... ", filepath);
    // Abre el archivo "ejercicio.txt" para sobreescribir su contenido
    int fd;
    if (mode == 'a')
        fd = open(filepath, O_WRONLY | O_APPEND);
    else if (mode == 't')
        fd = open(filepath, O_WRONLY | O_TRUNC);

    if (fd == -1) {
        perror("\nError al abrir el archivo");
        return 4;
    }


    char content[1024]; // Escribe en el archivo
    snprintf(content, sizeof(content), "hola, mi número es %d \n", numero);
    if (write(fd, content, strlen(content)) == -1) {
        perror("Error al escribir en el archivo");
        close(fd);
        return 5;
    }
    printf("OK\n");

    close(fd);

    printf("Contenido del archivo:\n\n");
    // Abre el archivo para leer su contenido
    fd = open(filepath, O_RDONLY);
    while ((read(fd, content, sizeof(content)))) {
        printf("%s", content);
    }


    close(fd);


    return 0;

}