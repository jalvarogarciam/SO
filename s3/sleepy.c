#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char * argv[]){
    if (argc!=2){
        printf("Use: %s <seconds>\n", argv[0]);
        exit(-1);
    }

    printf("%s: I'll be sleeping for a while (%s s)...\n", argv[0], argv[1]);
    sleep(atoi(argv[1]));
    printf("%s: Good morning!\n", argv[0]);

    return 0;
}