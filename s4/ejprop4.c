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
int main(int argc, char *argv[]) {


    if (argc<2){
        printf("Uso: %s <numeros separados por espacios>\n", argv[0]);
        exit(-1);
    }

    char path_sumador[] = "./bin/sumador";

    int pipefd1[2], pipefd2[2];
    if (pipe(pipefd1)<0 || pipe(pipefd2)<0){
        perror("Error en pipe");
        exit(-1);
    }


    // cantidad de numeros que tendra el primer esclavo
    int mitad= (argc-1)/2.0 + 0.5;

    char *args1[mitad+2], *args2[(argc-1)-mitad+2];
    args1[0]=args2[0]=path_sumador; //primer elemento es el path del programa
    args1[mitad+2-1]=NULL;
    args2[(argc-1)-mitad+2-1]=NULL; //ultimo elemento es NULL

    for (int i=1; i<=mitad; i++) args1[i]=argv[i];
    for (int i=1; i<=(argc-1)-mitad; i++) args2[i]=argv[i+mitad];


    //Crea el proceso de los primeros numeros...
    int pid1 = fork();
    if (pid1<0){
        perror("Fallo en fork 1");
        exit(-1);
    }else if (pid1==0){
        close(pipefd1[0]); //el proceso esclavo no quiere leer.
        
        //Duplica la pipefd1[1] en el descriptor de la salida estandar
        dup2(pipefd1[1], STDOUT_FILENO);
        close(pipefd1[1]); 
        //...cerramos pipefd1 porque ya no lo vamos a usar.
        
        if (execv(path_sumador, args1)<0){
            perror("Error en execv 1");
            exit(-1);
        }
    }


    //Crea el proceso del resto de numeros...
    int pid2 = fork();
    if (pid2<0){
        perror("Fallo en fork 2");
        exit(-1);
    }else if (pid2==0){
        printf("%i\n",pipefd2[0]);
        close(pipefd2[0]); //el proceso esclavo no quiere leer.
        printf("%i\n",pipefd2[0]);

        //Duplica la pipefd2[1] en el descriptor de la salida estandar
        dup2(pipefd2[1], STDOUT_FILENO);
        close(pipefd2[1]); 
        //...cerramos pipefd2 porque ya no lo vamos a usar.

        if (execv(path_sumador, args2)<0){
            perror("Error en execv 2");
            exit(-1);
        }
    }


    //Cierra el descriptor de escritura padre->hijos
    if (close(pipefd1[1])<0 || close(pipefd2[1])<0){
        perror("Error en close del pipefd padre");
        exit(-1);
    }

    //espera a que terminen...
    wait(NULL); wait(NULL);

    int val1, val2, bytes_read;
    //lee 4 bytes de ambos pipes y los guarda en val1 y val2
    if ((bytes_read=read(pipefd1[0], &val1, sizeof(int))) < 0  || 
        (bytes_read=read(pipefd2[0], &val2, sizeof(int))) < 0 ){
        perror("Error en read");
        exit(-1);
    }

    //Cierra los pipes...
    if (close(pipefd1[0])<0 || close(pipefd2[0])<0){
        perror("Error en close del pipefd padre");
        exit(-1);
    }

    printf("resultado: %i + %i = %i \n", val1, val2, (val1)+(val2));
}
