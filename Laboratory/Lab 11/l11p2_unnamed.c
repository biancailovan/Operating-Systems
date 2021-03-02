#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include<fcntl.h>

//Problema 1 modificata pe exemplul dat


int main(void)
{
    int fd[2];
    //int x = 0;
    char string[50];
    //char readBuffer[100];
    //char character;
  
    if(pipe(fd) != 0) {
        perror("Could not create pipe");
        return 1;
    }

    if(fork() != 0) {
        //parent
        close(fd[0]); //no use for read end
        //x = 42;
        //write(fd[1], &x, sizeof(x));
        printf("Dati un string: ");
        scanf("%s", string);
        printf("Parent: wrote %s to pipe\n", string);
        close(fd[1]);
        wait(NULL);
    } else {
        //child
        close(fd[1]); //no use for write end
        read(fd[0], &string, sizeof(string));
        printf("Child: read %s from pipe\n", string);
        close(fd[0]);
    }

    
    return 0;
}
