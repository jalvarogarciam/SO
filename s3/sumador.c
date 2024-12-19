#include <stdlib.h>
#include <stdio.h>



/*Este programa suma los numeros pasados como argumentos y los devuelve como salida del programa
  Uso: ./sumador <numeros separados por espacios>
*/
int main(int argc, char *argv[]) {
    if (argc<2){
        printf("Uso: %s <numeros separados por espacios>\n", argv[0]);
        exit(-1);
    }
    int sumatorio = 0;

    for (int i = 0; i < argc; i++)
    {
        sumatorio += atoi(argv[i]);    
    }
    
    exit(sumatorio);    
}