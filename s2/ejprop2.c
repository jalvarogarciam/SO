#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>   // Para open y las banderas O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>  // Para write y close



/* Crea un programa que reciba 2 argumentos:
    <ruta directorio> <permisos (formato 0XXX)>
Cambiar los permisos de todos los archivos regulares del directorio a los 
permisos indicados e imprimir para cada uno:
    <nombre archivo> <permisos antiguos> <permisos nuevos>
pista: 
    usar chmod/fchmod para cambiar permisos, 
    struct stat (st_mode) para consultar los permisos
    dirent (closedir, opendir, readdir) para iterar sobre los archivos
    strtol para convertir de string a entero (permisos)
IMPORTANTE: gestion de errores (perror)
*/

int main(int argc, char * argv[]){
    if (argc != 3){
        printf("Uso: %s <ruta directorio> <permisos (formato 0XXX)>\n", argv[0]);
        return 1;
    }

    const char  *path = argv[1];
    long permisos = strtol(argv[2], NULL, 8);
    // convierte argv[2] en un entero largo en base 8 (si puede)

    umask(0); //importante para establecer los permisos con la mascara correcta


    // Abre el directorio
    DIR *dir = opendir(path);
    if (dir == NULL) { //control de errores
        perror("Error al abrir el directorio");
        exit(-1);
    }

    // crea la estructura del directorio para leer todos los archivos
    struct dirent *entry;
    struct stat statbuf;
    char filepath[1024];   //ruta completa al archivo
    while((entry=readdir(dir)) != NULL){
        //construir la ruta completa al archivo
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);

        //obtener informacion del archivo
        if (stat(filepath, &statbuf) != -1){
                
            if (S_ISREG(statbuf.st_mode)){
                
                if (chmod(filepath, permisos) == -1){
                    perror("Error al cambiar los permisos del archivo");
                    exit(-1);
                }
                printf("%s %o %lo\n", entry->d_name, statbuf.st_mode & 0777, permisos);
            }
        }
        else{
            perror("Error al obtener informacion del archivo");
            exit(-1);
        }
    }
    closedir(dir);
}