#include <stdio.h>
#include <signal.h>

// Lista de señales a manejar (excluyendo SIGKILL y SIGSTOP)
const char *signals[] = {
    "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE",
    "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT",
    "SIGCHLD", "SIGCONT", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
    "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR", "SIGSYS"
};

int contador[64] = {0};

int terminar = 0;

static void handler (int signum){
  printf("\nSeñal %d emitida %i veces\n", signum, ++contador[signum-1]);
  if (signum == SIGTSTP) terminar = 1;
}

int main()
{
  struct sigaction sa;
  sa.sa_handler = handler; // ignora la señal
  sigemptyset(&sa.sa_mask); 

  //Reiniciar las funciones que hayan sido interrumpidas por un manejador 
  sa.sa_flags = SA_RESTART; 

  for (int i = 1; i<=32; i++)
    if (i!=9 && i!=19 && i!=32 && sigaction(i, &sa, NULL) == -1 ){ 
      printf("\nerror en el manejador de la señal %d\n", i);
    }

  while(terminar==0);
}