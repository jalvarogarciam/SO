/*
tarea7.c
Programa ilustrativo del uso de pipes y la redirecci�n de entrada y
salida est�ndar: "ls | sort"
*/

#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
	int pipefd[2];
	if (pipe(pipefd)<0){
		perror("Error en pipe");
		exit(-1);
	}

	int pid = fork();
	if (pid == 0){
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execlp("ls", "ls", NULL);
	}

	wait(NULL);

	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[1]);

	execlp("sort", "sort", NULL);
	return 0;
}


