//consumidorFIFO.c
//Consumidor que usa mecanismo de comunicacion FIFO.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define BUFFER "factory"



static void handler(int signum){
    printf("Borrando el fifo...\n");
    if (unlink(BUFFER)<0 && errno != ENOENT){
        //...si el error no es que ya está borrado
        perror("No se pudo borrar el BUFFER");
        exit(-1);
    }
    exit(0); 
}

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = handler; // ignora la señal
    sigemptyset(&sa.sa_mask); 
    
    //Reiniciar las funciones que hayan sido interrumpidas por un manejador 
    sa.sa_flags = SA_RESTART; 


    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);

    umask(0);
    int fifod = mkfifo(BUFFER, 0666);
    if (fifod<0 && errno != EEXIST){ //Si no se ha podido crear y no existe...
        perror("Error al crear el BUFFER");
        exit(-1);
    }

    while (1){    

        int fd = open(BUFFER, O_RDONLY);
        if (fd<0){
            perror("Error al abrir BUFFER");
            exit(-1);
        }


        char buffer[1024];
        int bytes_read;
        while((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) == sizeof(buffer)-1 ){
            buffer[bytes_read] = 0;
            printf("%s",buffer);
        }
        buffer[bytes_read] = 0;
        printf("%s",buffer);
        if (bytes_read<0){
            perror("Error al leer del BUFFER");
            exit(-1);
        }


        if (close(fd)<0){
            perror("Error al cerrar el BUFFER");
            exit(-1);
        }
        
                 

 
    }        
    
    
}





