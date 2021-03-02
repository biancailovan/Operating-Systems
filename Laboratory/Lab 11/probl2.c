#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int fd[2];
    char c = 'c';
    int i;

    fprintf(stdout, "Creation\n");
    fprintf(stdout, "Max size: %d\n", PIPE_BUF);
    
    if( pipe(fd) != 0)
    {
        perror("pipe");
        exit(1);
    }

    fprintf(stdout, "Writing in pipe\n");
    for(i=0;; i++)
    {
        fprintf(stdout, "%d bytes written\n", i+1);
        if( write(fd[1], &c, 1) != 1)
        {
            perror("Write");
            exit(1);
        }
    }
    return 0;
}