#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void procesamiento(int fd, int tiempo, const char filepath[]){
    printf("%i procesando %s... durante %i segundos\n", getpid(), filepath, tiempo);

    char msg[128];
    snprintf(msg, sizeof(msg), "el proceso %i esuvo aqui %i segundos\n", getpid(), tiempo);
    if (write(fd, msg, strlen(msg))==-1){
        perror("error en el procesamiento");
        exit(-1);
    }
    sleep(tiempo);
    printf("...%s procesado por %i\n", filepath, getpid());
}
/*
 * DIFERENCIAS ENTRE F_SETLK Y F_SETLKW:
 *
 * 1. F_SETLK:
 *    - No bloqueante: Intenta aplicar el cerrojo y, si no es posible (porque
 *      otro proceso tiene un cerrojo en conflicto), falla inmediatamente y
 *      retorna -1, estableciendo errno con el código de error correspondiente.
 *    - Es útil cuando no quieres que el programa quede en espera si no puede
 *      bloquear el archivo.
 *
 * 2. F_SETLKW:
 *    - Bloqueante: Si no puede aplicar el cerrojo inmediatamente, el programa
 *      queda en espera (suspendido) hasta que el cerrojo sea liberado.
 *    - Es útil cuando deseas asegurarte de que el bloqueo se realice
 *      eventualmente, incluso si hay otros cerrojos presentes.
 */
int main(int argc, char* argv[]) {
    struct flock cerrojo;       //el cerrojo
    int processing_time;        //tiempo de procesamiento (ficticio)
    const char * filepath;
    int desde_donde = 0;        // pues desde donde..., por defecto hasta el final
    int cuantos_bytes = 0;      //longitud del bloqueo, por defecto hasta el final
    int fd;                     //descriptor de fichero

    if (argc<4){
        printf("Uso: %s <nombre fichero> <inicio del bloqueo (B)> <longitud del bloqueo (B)>", argv[0]);
        exit(-1);
    }
    if (argc==5){
        processing_time=atoi(argv[--argc]);
    }else{
        srand(time(NULL)); int max = 25, min = 1;
        processing_time = rand() % (max - min + 1) + min;
    }
    cuantos_bytes = atoi(argv[--argc]);
    desde_donde = atoi(argv[--argc]);
    filepath = argv[--argc];

        
    // Intentamos abrir el archivo enmodo lectura/escritura
    if ((fd = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0664)) == -1) {
        perror("open fallo"); // Si falla, mostramos un error
        exit(-1); // Pasamos al siguiente archivo
    }


    /* Intentamos un bloqueo de escritura no bloqueante (F_SETLK) */
    int pid_poseedor = -1; // Bandera para imprimir solo un mensaje por archiv
    do { // Si falla, otro proceso lo bloquea

        /* Si el bloqueo falla, identificamos al proceso que lo mantiene */
        int error = 0;          // Bandera para comprobar que la consulta no falle
            // si no da error la consulta, y el cerrojo no esta desbloqueado y el pid_poseedor ha cambiado
        if ((error = fcntl(fd, F_GETLK, &cerrojo)) != -1 && cerrojo.l_type != F_UNLCK && pid_poseedor != cerrojo.l_pid) {
            // Imprimimos información del bloqueo activo
            printf("No es posible establecer un cerrojo: %s bloqueado por %d desde %li hasta %li para %c\n", filepath,
                    cerrojo.l_pid, cerrojo.l_start, cerrojo.l_len,
                    cerrojo.l_type == F_WRLCK ? 'w' : 'r');
            pid_poseedor = cerrojo.l_pid; // Evita imprimir repetidamente para el mismo archivo
        }
        else if (error){
            perror("fallo en la consulta del cerrojo");
            exit(-1);
        }

        // Configuramos/Restablecemos el cerrojo para bloquear el archivo (por el rango adecuado)
        cerrojo.l_type = F_WRLCK; // Tipo de bloqueo: escritura
        cerrojo.l_whence = SEEK_SET; // Inicio del bloqueo desde el inicio del archivo
        cerrojo.l_start = desde_donde; // desde la zona que decidamos (por defecto el 1er byte)
        cerrojo.l_len = cuantos_bytes; // con la longitud que decidamos (por defecto 0, hasta el final)

    }while (fcntl(fd, F_SETLK, &cerrojo) == -1);
    /* Reintentamos el bloqueo hasta que sea exitoso */

    /* Ahora el bloqueo tiene éxito, procesamos el archivo */
    procesamiento(fd, processing_time,filepath);

    /* Una vez terminado el procesamiento, desbloqueamos el archivo */
    cerrojo.l_type = F_UNLCK; // Configuramos el cerrojo para desbloquear
    cerrojo.l_whence = SEEK_SET; // Desde el inicio del archivo
    cerrojo.l_start = desde_donde; // En la zona que decidimos
    cerrojo.l_len = cuantos_bytes;

    // Usamos F_SETLKW para desbloquear el archivo de forma bloqueante
    if (fcntl(fd, F_SETLKW, &cerrojo) == -1) {
        // Si ocurre un error al desbloquear, lo mostramos
        perror("Desbloqueo");
    }

    close(fd); // Cerramos el archivo una vez procesado

    return 0;
}
