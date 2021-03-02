#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


/*
    citesc un fisier; construiesc alt fisier cu continutul inversat
    nume fisier sursa + nume fisier destinatie se primesc ca argumente

*/

int main(int argc, char **argv)
{
    int fd1 = -1;
    int fd2 = -1;

    char str=0;

    fd1 = open(argv[1], O_RDONLY);
    fd2 = creat(argv[2],S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH);
    
    if(fd1 == -1){
        printf("Could not open file\n");
        return 1;
    }

    if(fd2 == -1){
        printf("Could not open file\n");
        return 2;
    }

    off_t size = lseek(fd1, -1, SEEK_END);
  
    while(size!=0){

        if(read(fd1, &str, 1)!=1){
            printf("Could not read file\n");
        }

        if(write(fd2, &str, 1)!=1){
            printf("Could not read file\n");
        }

        printf("%c\n", str);
        lseek(fd1, -2, SEEK_CUR);
        size--;
    }

    close(fd1);
    close(fd2);

    return 0;

}