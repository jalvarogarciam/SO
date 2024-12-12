#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


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
    struct flock cerrojo;
    int processing_time = atoi(argv[argc-1]);
    int fd;

    // Iteramos sobre cada archivo pasado como argumento
    for (int i=1; i<argc-1; i++) {
        
        // Intentamos abrir el archivo enmodo lectura/escritura
        if ((fd = open(argv[i], O_RDWR | O_APPEND | O_CREAT, 0777)) == -1) {
            perror("open fallo"); // Si falla, mostramos un error
            continue; // Pasamos al siguiente archivo
        }

        // Configuramos el cerrojo para bloquear el archivo completo
        cerrojo.l_type = F_WRLCK; // Tipo de bloqueo: escritura
        cerrojo.l_whence = SEEK_SET; // Inicio del bloqueo desde el inicio del archivo
        cerrojo.l_start = 0; // Bloqueo desde el byte 0
        cerrojo.l_len = 0; // Bloqueo hasta el final del archivo

        /* Intentamos un bloqueo de escritura no bloqueante (F_SETLK) */
        int mensaje_impreso = 0; // Bandera para imprimir solo un mensaje por archiv
        while (fcntl(fd, F_SETLK, &cerrojo) == -1) { // Si falla, otro proceso lo bloquea
            /* Si el bloqueo falla, identificamos al proceso que lo mantiene */
            int error = 0;          // Bandera para comprobar que la consulta no falle
            while ((error = fcntl(fd, F_GETLK, &cerrojo)) != -1 && cerrojo.l_type != F_UNLCK) {

                if (!mensaje_impreso) {
                    // Imprimimos información del bloqueo activo
                    printf("%s bloqueado por %d desde %li hasta %li para %c\n", argv[i],
                           cerrojo.l_pid, cerrojo.l_start, cerrojo.l_len,
                           cerrojo.l_type == F_WRLCK ? 'w' : 'r');
                    mensaje_impreso = 1; // Evita imprimir repetidamente para el mismo archivo
                }

                // Si l_len es 0, el cerrojo afecta todo el archivo restante
                if (!cerrojo.l_len) break; // Salimos del bucle para evitar un bucle infinito

                // Si hay más cerrojos parciales, avanzamos al siguiente rango
                cerrojo.l_start += cerrojo.l_len; // Movemos el inicio al final del cerrojo actual
                cerrojo.l_len = 0; // Configuramos para buscar en el resto del archivo
            } /* Continuamos mientras haya bloqueos en otras regiones */
            if (error){
                perror("fallo en la consulta del cerrojo");
                exit(-1);
            }

        } /* Reintentamos el bloqueo hasta que sea exitoso */

        /* Ahora el bloqueo tiene éxito, procesamos el archivo */
        procesamiento(fd, processing_time,argv[i]);

        /* Una vez terminado el procesamiento, desbloqueamos el archivo */
        cerrojo.l_type = F_UNLCK; // Configuramos el cerrojo para desbloquear
        cerrojo.l_whence = SEEK_SET; // Desde el inicio del archivo
        cerrojo.l_start = 0; // Desde el byte 0
        cerrojo.l_len = 0; // Hasta el final del archivo

        // Usamos F_SETLKW para desbloquear el archivo de forma bloqueante
        if (fcntl(fd, F_SETLKW, &cerrojo) == -1) {
            // Si ocurre un error al desbloquear, lo mostramos
            perror("Desbloqueo");
        }

        close(fd); // Cerramos el archivo una vez procesado
    }

    return 0;
}
