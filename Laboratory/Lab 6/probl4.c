#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
/*nu stiu cum se face*/

/*pentru a vedea care dintre liniile unui fisier contin un anumit string*/
void matchPattern(char *argv[])
{
    int fd,r,j=0;
    char ch,line[100];
    if((fd=open(argv[2],O_RDONLY)) != -1)
    {
        while((r=read(fd,&ch,sizeof(char)))!= 0)
        {
            if(ch!='\n')
            {
                line[j]=ch;
                j++;
            }
            
            else
            {
                if(strstr(line,argv[1])!=NULL)
                    printf("%s\n",line);
                memset(line,0,sizeof(line));
                j=0;
            }

        }
    }   
}

int main(int argc, char **argv)
{
    pid_t pid;
    int fd, status;
    struct stat statt;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <result_file>\n", argv[0]);
        return 1;
    }

    if(argc==3)
    {
        if(stat(argv[2],&statt)==0)
            matchPattern(argv);
        else 
        {
            perror("stat()");
            exit(1);
        }
    }

    pid = fork();
    if(pid == -1) {
        perror("Could not create child process");
        return -1;
    } else if(pid == 0) {
        fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if(fd < 0) {
            perror("Could not open the result file");
            exit(1);
        }
        dup2(fd, 1); //use fd as standard output
        //will inherit the standard output
        execlp("grep", "grep", "abcd", argv[1], NULL);
        perror("execlp failed");
        exit(2);
    } else {
        wait(&status);
        if(WEXITSTATUS(status) == 0) {
            printf("Command completed successfuly!\n");
        } else {
            printf("Command failed!\n");
        }
    }
    return 0;
}