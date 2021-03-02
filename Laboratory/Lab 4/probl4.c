#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
    -insereaza un string intr-un fisier, pe o pozitie data
    -am ales ca pozitia pe care inserez sirul sa fie 3

*/

int main(int argc, char **argv)
{
    int fd=-1;

    fd = open(argv[1], O_RDWR);

    if(fd == -1){
        printf("Could not open the file!\n");
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END);

    char *buffer = malloc(sizeof(char)*(size+1));

    lseek(fd, 0, SEEK_SET);

    ssize_t rd = read(fd, buffer, size);
    printf("%ld", rd);

    lseek(fd, 0,SEEK_SET);

    int poz = 3;
    char *sir="XYZ";

    for(int i=0;i<poz;i++){
        write(fd, buffer+i, 1);
    }

    for(int i=0;i<strlen(sir);i++){
        write(fd, sir+i, 1);
    }

    for(int i=poz+strlen(sir);i<size+strlen(sir);i++){
        write(fd, buffer+i-strlen(sir),1);
    }

    close(fd);
    return 0;
}