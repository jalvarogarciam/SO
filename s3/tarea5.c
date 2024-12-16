//tarea5.c
//Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10

#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>

/*
Ejercicio 4. Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se
identificará en la salida estándar, mostrando un mensaje del tipo Soy el hijo PID. El proceso
padre simplemente tendrá que esperar la finalización de todos sus hijos y cada vez que detecte
la finalización de uno de sus hijos escribirá en la salida estándar un mensaje del tipo:
Acaba de finalizar mi hijo con <PID>
Sólo me quedan <NUM_HIJOS> hijos vivos
Implementa una modificación sobre el anterior programa en la que el proceso
padre espera primero a los hijos creados en orden impar (1º,3º,5º) y después a los hijos pares (2º
y 4º).
*/
int main(int argc, char *argv[])
{
	const int num_procesos = 5;

	int pids[num_procesos];
	int pid_index=-1;
	for (int i=0; i<num_procesos; i++){
		pids[i] = fork();
		if (pids[i]<0){
			perror("Error en la creacion de procesos");
			exit(-1);
		}
		if (pids[i]==0){pid_index=i; break;}
	}

	if (pid_index != -1){
		
		printf("Soy el hijo %i -> %i\n", pid_index, getpid());
		sleep(1);
		exit(pid_index);
	}

	sleep(1);
	printf("\nEsperando finalizacion de procesos impares...\n");
	for (int i=1; i<num_procesos; i+=2){
		int status, pid_termina = waitpid(pids[i], &status, 0);
		printf("Acaba de finalizar mi hijo %i con pid %i \n", WEXITSTATUS(status), pid_termina);
		printf("Sólo me quedan %i hijos vivos\n", num_procesos-1-i/2);
	}

	printf("\nEsperando finalizacion de procesos pares...\n");
	for (int i=0; i<num_procesos; i+=2){
		int status, pid_termina = waitpid(pids[i], &status, 0);
		printf("Acaba de finalizar mi hijo %i con pid %i \n", WEXITSTATUS(status), pid_termina);
		printf("Sólo me quedan %i hijos vivos\n", (num_procesos-1-i)/2);
	}

	return 0;
}



