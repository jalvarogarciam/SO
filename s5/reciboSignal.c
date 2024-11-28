/*
 reciboSignal.c
 Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
 Utilización de la llamada sigaction para cambiar el comportamiento del proceso       
 frente a la recepción de una señal.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

static void handler(int sigNum)
  {

    if(sigNum == SIGUSR1)
      printf("\nRecibida la senal SIGUSR1\n\n");
    else if(sigNum == SIGUSR2)
      printf("\nRecibida la senal SIGUSR2\n\n"); 
   }

int main(int argc, char *argv[]){

  struct sigaction sa;

  if(setvbuf(stdout,NULL,_IONBF,0)) { perror("\nError en setvbuf"); }

  //Inicializar la estructura sig_USR_na para especificar la nueva acción para la señal.
  sa.sa_handler= handler;

  //'sigemptyset' inicia el conjunto de señales dado al conjunto vacio. 
  sigemptyset (&sa.sa_mask);
  sa.sa_flags = 0;


//Establecer mi manejador particular de señal para SIGUSR1
if( sigaction(SIGUSR1,&sa,NULL) <0) 
    {
	perror("\nError al intentar establecer el manejador de senal para SIGUSR1");
	exit(EXIT_FAILURE);
    }
//Establecer mi manejador particular de señal para SIGUSR2
if( sigaction(SIGUSR2,&sa,NULL) <0) 
    {
	perror("\nError al intentar establecer el manejador de senal para SIGUSR2");
	exit(EXIT_FAILURE);
    }
for(;;)
   {
   }
}
