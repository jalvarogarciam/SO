#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Crea un programa que reciba como parámetros un vector de enteros separados por espacios: 
    ./ejprop3 1 2 3 2 5 1...
Y calcule la suma de todos estos.

-El programa creará 2 procesos hijos (usando fork). Un hijo se encargará de sumar los números pares 
y otro los impares (NÚMEROS, no posiciones).

-Cada hijo mostrará esa suma, su PID (getpid) y el PID de su padre (getppid). En el ámbito de proceso hijo.

El proceso padre deberá esperar a que ambos hijos terminen (waitpid), 
recuperar el valor de cada suma en el ámbito de proceso padre (consultar relación exit y status del waitpid), 
sumar los dos valores y mostrarlos por pantalla. 
También deberá mostrar su PID para confirmar que es el padre de los dos procesos hijos creados.
*/


int sumarray(int * array, size_t size){
    int suma = 0;
    for (int i=0; i<size; i++) suma += array[i];
    return suma;
}

int main(int argc, char *argv[]) {


    if (argc<2){
        printf("Uso: %s <numeros separados por espacios>\n", argv[0]);
        exit(-1);
    }

    int fd1[2];
    int fd2[2];
    int bytesLeidos, bytesLeidos2, val1, val2;
    pid_t esclavo1, esclavo2;

    int n = argc-1;
    int mitad = n/2;

    char *numeros1[mitad+2];
    char *numeros2[n-mitad+2];
    numeros1[0] = numeros1[2] = "./esclavo";

    for (int i=1; i<=mitad; i++){
        numeros1[i]=argv[i];
    }
    numeros1[mitad+1]=NULL;

    for (int i=1; i<=n; i++){
        numeros2[i-mitad]=argv[i];
    }

    pipe(fd1);pipe(fd2);

    if ((esclavo1=fork()) < 0){
        gestion error
    }
    else if (esclavo1==0){
        close(fd1[0]);
        dup2(fd1[1], STDOUT_FILENO);
        if (execv("./esclavo", numeros1)<1){
            gestion error
        }
    }else{
        close(fd1[1]);
        while((bytesLeidos=read(fd1[0], &val1, sizeof(int)) > 0 )){
            printf("Suma hijo 1: %d",val1);
        }
        close(fd1[0]);
        printf("\n");
    }

    if ((esclavo2=fork()) < 0){
        gestion error
    }
    else if (esclavo2==0){
        close(fd1[0]);
        dup2(fd1[1], STDOUT_FILENO);
        if (execv("./esclavo", numeros2)<1){
            gestion error
        }
    }else{
        close(fd2[1]);
        while((bytesLeidos2=read(fd2[0], &val2, sizeof(int)) > 0 )){
            printf("Suma hijo 1: %d",val2);
        }
        close(fd2[0]);
        printf("\n");
    }
    printf("resultado %i\n", val1+val2);
    return 0;



}
