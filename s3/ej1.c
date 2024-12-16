#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



/*Ejercicio 1. Implementa un programa en C que tenga como argumento un número entero. Este
programa debe crear un proceso hijo que se encargará de comprobar si dicho número es un
número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar.
A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o
no usando igualmente la salida estándar.*/
int main(int argc, char * argv[]){
    if (argc!=2){
        printf("Numero de argumentos erroneo\n");
        exit(-1);
    }

    int pid = fork(); //crea el proceso hijo
    if (pid<0){ //comprobacion de errores
        perror("Error en la creacion del proceso hijo");
        exit(-2);
    }
    else if (pid==0){ //codigo del proceso hijo
        printf("El numero %s %ses par.\n", argv[1], atoi(argv[1])%2==0? "":"no ");
        exit(0); //termina
    }

    if (wait(NULL)==-1){//espera a que el proceso hijo termine
        perror("Error al esperar la finalizacion del hijo");
    }//codigo del proceso padre
    printf("El numero %s %ses divisible por 4.\n", argv[1], atoi(argv[1])%4==0? "":"no ");
    
    return 0;
}