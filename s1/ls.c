#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>     // Para el tipo DIR
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>   // Para open y las banderas O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>  // Para write y close

const size_t PATH_BUFFER_SIZE = 512;
const size_t ERR_BUFFER_SIZE = 1024;
const size_t OUTPUT_BUFFER_SIZE = 1024*1024;

int list_file (const char * style, const char * name, const char * dir_path, char * output_buffer,  char * err_buffer);
int list_dir (const char *style, const char * dir_path, char * output_buffer,  char * err_buffer);

int list_file (const char * style, const char * name, const char * dir_path, char * output_buffer,  char * err_buffer){
    struct stat statbuf;
    char filepath[PATH_BUFFER_SIZE];
    snprintf(filepath, PATH_BUFFER_SIZE, "%s/%s", dir_path, name);

    if (stat(filepath, &statbuf) != 0 && !S_ISREG(statbuf.st_mode) && !S_ISDIR(statbuf.st_mode)) {
        snprintf(err_buffer, ERR_BUFFER_SIZE, "%s\ncannot access '%s': No such file or directory\n", err_buffer, name);
        return -1;
    }


    if (S_ISREG(statbuf.st_mode)){
        if (name[0]!='.') {strcat(output_buffer, name ); strcat(output_buffer, "   ");}
    }
    else{
        if (strcmp(".",name) != 0 && strcmp("..",name) != 0){
            if (style[0]=='r')  list_dir(style, filepath, output_buffer, err_buffer);
            else {
                strcat(output_buffer, name ); 
                strcat(output_buffer, "   ");
            }
        }
    }
    return 0;

}

int list_dir (const char *style, const char * dir_path, char * output_buffer,  char * err_buffer){
    DIR * dir = opendir(dir_path);
    struct dirent *entry;
    struct stat   statbuf;

    if (!dir){
        if (stat(dir_path,&statbuf)==0){
            return list_file( style, dir_path, "", output_buffer, err_buffer );   //se lista
        }
    }

    char file_path[PATH_BUFFER_SIZE];
    while ((entry = readdir(dir)) != NULL){

        snprintf(file_path, PATH_BUFFER_SIZE, "%s/%s", dir_path, entry->d_name);
        
        if (stat(file_path, &statbuf)!=0){
            snprintf(err_buffer,ERR_BUFFER_SIZE, "%s\n error al leer el archivo %s\n", err_buffer, entry->d_name);
        }
        if (list_file( style, entry->d_name, dir_path, output_buffer, err_buffer ))
            return -1;
    }

    closedir(dir);
    return 0;
}

int main (int argc, char ** argv){
    int status=0;
    char err_msg[ERR_BUFFER_SIZE];
    char output_msg[OUTPUT_BUFFER_SIZE];
    strcpy(err_msg, argv[0]);
    strcat(err_msg, ": ");


    char dir_path[PATH_BUFFER_SIZE];

    if (argc == 1) {
        strcpy(dir_path, ".");
    } else if (argc == 2) {
        strcpy(dir_path, argv[1]);
    } else {
        fprintf(stderr, "Uso: %s [ruta directorio]\n", argv[0]);
        return 1;
    }

    status = list_dir("d", dir_path, output_msg, err_msg);
    if (status){
        printf("%s",err_msg);
        exit(-1);
    }
    printf("%s\n", output_msg);


    return status;
}