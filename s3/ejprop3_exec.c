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
ESTA VERSION UTILIZA EXEC PARA CADA PARTE, USANDO EL PROGRAMA SUMADOR.C
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

    char path_sumador[] = "./bin/sumador";

    int suma = 0;


    char * args_pares [argc+1], tam_pares=1;
    char * args_impares [argc+1], tam_impares=1;

    for (int i = 1; i<argc; i++){
        int numero = atoi(argv[i]);
        if (numero%2) args_pares[tam_pares++]=argv[i];
        else           args_impares[tam_impares++]=argv[i]; 
    }
    // Indicamos el final de los argumentos con null...
    args_pares[tam_pares++] = args_impares[tam_impares++] = NULL;
    // Indicamos el nombre del programa en el primer argumento
    args_pares[0] = args_impares[0] = "sumador";

    int pid_par = fork();
    if (pid_par<0){
        perror("Error en fork");
        exit(-1);
    
    }else if (pid_par==0){
        execv(path_sumador, args_pares);
        perror("Error en execv pares");// Si execv falla, se ejecutará el siguiente código
        exit(EXIT_FAILURE);
    }

    int pid_impar = fork();
    if (pid_impar<0){
        perror("Error en fork");
        exit(-1);
    
    }else if (pid_impar==0){
        execv(path_sumador, args_impares);
        perror("Error en execv impares");// Si execv falla, se ejecutará el siguiente código
        exit(EXIT_FAILURE);
    }


    int status_par, status_impar;
    waitpid(pid_impar, &status_impar,0);
    waitpid(pid_par, &status_par,0);

    suma = WEXITSTATUS(status_impar) +  WEXITSTATUS(status_par);
    printf("Proceso padre (PID: %d): Suma total = %d\n", getpid(), suma);

    return 0;
}
