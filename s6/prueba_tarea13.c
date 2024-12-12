#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char * args1[] = {"./tarea13","archivo.txt", "file.txt",  "6", NULL};
    char * args2[] = {"./tarea13","file.txt", "archivo.txt",  "2", NULL};


    int p1 = fork();
    if (p1<0){
        perror("fork p1 failed");
        exit(-1);
    }
    else if (p1==0){
        if (execv(args1[0], args1)== -1) {
            perror("Error al ejecutar el programa");
            exit(EXIT_FAILURE);
        }
        exit (0);
    }

    int p2 = fork();
    if (p2<0){
        perror("fork p2 failed");
        exit(-1);
    }
    else if (p2==0){
        
        if (execv("./tarea13", args2)== -1) {
            perror("Error al ejecutar el programa");
            exit(EXIT_FAILURE);
        }
        exit (0);
    }

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);


    return 0;
}