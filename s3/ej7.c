#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>



/* Ejercicio 7. Escribe un programa que acepte como argumentos el nombre de un programa, sus
argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto programa deberá ejecutar el
programa pasado como primer argumento en foreground si no se especifica la cadena “bg” y en
background en caso contrario. Si el programa tiene argumentos hay que ejecutarlo con éstos.
*/

int main(int argc, char * argv[]){
    if (argc < 2){
        printf("Uso: %s <nombre del programa> <argumentos>...<bg>\n \
        (bg se pondra en caso de que se quiera ejecutar el programa en background)\n", argv[0]);
        exit(-1);
    }

    int bg = strcmp(argv[argc-1], "bg") == 0? 1:0;
    if (bg) argc--;
    
    const char * args[argc];
    for (int i=1; i<argc; i++){
        args[i-1]=argv[i];
    }
    args[argc-1]=NULL;
    

    if (bg){
        int pid=fork();
        if (pid<0){
            perror("No se pudo crear proceso");
            exit(-1);
        }
        const char * args[argc];
        for (int i=1; i<argc; i++){
            args[i-1]=argv[i];
        }
        args[argc-1]=NULL;
        if (execvp(args[0], args) == 0){
            perror("Error en execv"); 
            exit(-1);
        }
        exit(0);

    }
    else{
        if (execvp(args[0], args) == 0){
            perror("Error en execv"); 
            exit(-1);
        }
    }
    
    return 0;
}