#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



/* Implementa un programa que tenga como argumento un número entero. 
Este programa debe crear un proceso hijo que se encargará de comprobar si 
dicho número es un número par o impar e informará al usuario con un mensaje 
que se enviará por la salida estándar.
A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o
no usando igualmente la salida estándar.
*/

int main(int argc, char * argv[]){
    if (argc != 2){
        printf("Uso: %s <numero entero>\n", argv[0]);
        return 1;
    }

    int numero = atoi(argv[1]);

    int hijo = fork();
    if (hijo<0){
        perror("fork failed");
        exit(-1);
    }
    else if (hijo==0){
        if (numero%2)printf("El número %i es impar\n", numero);
        else printf("El número %i es par\n", numero);
        exit(0);
    }
    else{
        wait(NULL);
        if(numero%4) printf("El número %i no es múltiplo de 4\n", numero);
        else printf("El número %i es múltiplo de 4\n", numero);
    }
    return 0;
}