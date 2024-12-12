#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>   // Para open y las banderas O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>  // Para write y close



/*Programa una nueva orden que recorra la jerarquía de subdirectorios existentes a
partir de uno dado como argumento. Para los archivos regulares que tengan permiso 
de ejecución para el grupo y para otros, deberá:
    -   Listarlos junto a su número de inodo.
    -   El recuento de todos ellos y el espacio que ocupan en bytes.
    -   Además del nombre de los archivos encontrados, deberá devolver sus números de inodo y la suma total de espacio ocupado por
ejemplo:
$> ./buscar <pathname>
Los i-nodos son:
./a.out 55
./bin/ej 123
./bin/ej2 87
...
Existen 24 archivos regulares con permiso x para grupo y otros
El tamaño total ocupado por dichos archivos es 2345674 bytes
*/
int recuento = 0;
int tamanio = 0;

void imprime_archivo(const char filepath[], struct stat statbuf){
    if (S_IXGRP & statbuf.st_mode && S_IXOTH & statbuf.st_mode){
        printf("    %s %li\n",filepath, statbuf.st_ino);
        recuento++;
        tamanio+=statbuf.st_size;
    }
}

void imprime_directorio(const char dirpath[]){
    DIR * dir = opendir(dirpath);
    if (dir == NULL){
        perror("Error al abrir el directorio");
        exit(-1);
    }

    struct dirent * entry;
    struct stat statbuf;
    char filepath[1024];
    while ((entry=readdir(dir))!=NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);

        if (stat(filepath, &statbuf)==-1){
            printf("%s\n",dirpath);
            perror("error al acceder a los metadatos del archivo");
            exit(-1);
        }

        if (S_ISREG(statbuf.st_mode)){
            imprime_archivo(filepath, statbuf);
        }else if (S_ISDIR(statbuf.st_mode)){
            imprime_directorio(filepath);
        }
    }
    closedir(dir);
}

int main(int argc, char * argv[]){
    if (argc != 2){
        printf("Uso: %s <ruta directorio>\n", argv[0]);
        return 1;
    }

    printf("Los i-nodos son:\n");

    imprime_directorio(argv[1]);

    printf("Existen %i archivos regulares con permiso x para grupo y otros\n", recuento);
    printf("El tamaño total ocupado por dichos archivos es %i bytes\n", tamanio);

    return 0;
}



/*
FALLOS:


-   S_IXGRP y S_IXOTH son flags, no funciones.
-   stat falla con las entradas . y .., hay que comprobarlo!
-   hay que poner la ruta completa al archivo, entry solo da su nombre (snpritf)
*/