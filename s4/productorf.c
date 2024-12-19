#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <signal.h>

#define BUFFER "factory"




static void handler(int signum){
    printf("\nBorrando el fifo...\n");
    if (unlink(BUFFER)<0 && errno != ENOENT){
        //...si el error no es que ya está borrado
        perror("No se pudo borrar el BUFFER");
        exit(-1);
    }
    exit(0); 

}

int main(int argc, char **argv){  
    struct sigaction sa;
    sa.sa_handler = handler; // ignora la señal
    sigemptyset(&sa.sa_mask); 
    
    //Reiniciar las funciones que hayan sido interrumpidas por un manejador 
    sa.sa_flags = SA_RESTART; 

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);
    
    umask(0);
    int fifod = mkfifo(BUFFER, 0666 ); // Intenta crearlo, por si no existe aun
    if (fifod<0){
        if (errno!=EEXIST){ // Si el error es diferente de que BUFFER existe
            perror("Error al crear el BUFFER");
            exit(-1);
        }
    }

    while (1){

        int fd = open(BUFFER, O_WRONLY);
        if (fd<0){
            perror("Error al abrir el BUFFER");
            exit(-1);
        }

        printf(">");
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL){
            write(fd, buffer, strlen(buffer));
        } else{
            perror("Error al leer el mensaje de la terminal");
            exit(-1);
        }
        if (close(fd)<0){
            perror("Error al cerrar el BUFFER");
            exit(-1);
        }



    }
 
    return 0;

}
