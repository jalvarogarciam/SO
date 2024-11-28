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

    // arrays para la suma
    int * pares = malloc((argc-1)* sizeof(int)), num_pares=0;
    int * impares = malloc((argc-1)* sizeof(int)), num_impares=0;
    int suma=0;

    for (int i=1; i<argc; i++){ // rellenamos los valores de pares e impares
        int num = atoi(argv[i]);
        if (num%2==0) pares[num_pares++]=num;
        else impares[num_impares++]=num;
    }

    // SUMAR PARES ...........................................................
    int pid_par = fork();
    if (pid_par<0){
        perror("fork par failed");
        exit(-2);
    }
    if (pid_par == 0) {
        int suma_pares = sumarray(pares, num_pares);
        printf("Proceso hijo (PID: %d, PPID: %d): Suma de pares = %d\n", getpid(), getppid(), suma_pares);
        exit(suma_pares); // devuelve el resultado de la suma
    }//.........................................................................

    // SUMAR IMPARES ...........................................................
    int pid_impar = fork();
    if (pid_impar<0 ){
        perror("fork impar failed");
        exit(-2);
    }
    if (pid_impar == 0) {
        int suma_impares = sumarray(impares, num_impares);
        printf("Proceso hijo (PID: %d, PPID: %d): Suma de impares = %d\n", getpid(), getppid(), suma_impares);
        exit(suma_impares); // devuelve el resultado de la suma
    }//.........................................................................

    // Suma de ambas partes e impresion del resultado.
    if (pid_impar!=0 && pid_par!=0){
        waitpid(pid_impar, &pid_impar,0);
        waitpid(pid_par, &pid_par, 0);
        suma = WEXITSTATUS(pid_impar)+WEXITSTATUS(pid_par);
        printf("Proceso padre (PID: %d): Suma total = %d\n", getpid(), suma);
    }


    free(pares);    //liberacion de la memoria dinamica reservada para los arrays
    free(impares);
    return 0;
}
