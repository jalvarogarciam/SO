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
    
    char * args[argc];
    for (int i=1; i<argc; i++){
        args[i-1]=argv[i];
    }
    args[argc-1]=NULL;
    

    if (bg){
        int pid=fork();
        if (pid<0){
            perror("Error en execvp");;
            exit(-1);
        }

        else if (pid == 0){
            execvp(args[0], args);
            perror("Error en execv"); 
            exit(-1);
        }

        

    }
    else{
        execvp(args[0], args);
        perror("Error en execv"); 
        exit(-1);
    }

    // En el caso de BG, el padre espera a que el hijo termine, y va haciendo otras cosas
    // Si no es BG, el programa no llegaría a esta parte del código	

    printf("%s: mientras tanto yo voy haciendo otras cosas en el foreground...\n", argv[0]);
    char * numeros[] = {"uno", "dos", "tres", "cuatro", "cinco", "seis", "siete", "ocho", "nueve", "diez", NULL};
    for (int i=0; i<10; i++){  printf("%s: %s\n", argv[0], numeros[i]); sleep(1);}
    wait(NULL);
    return 0;
}