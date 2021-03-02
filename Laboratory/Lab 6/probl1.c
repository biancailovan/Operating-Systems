#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid2=-1, pid3=-1, pid4=-1, pid5=-1;
  
    pid2 = fork();
    if(pid2 == 0) {

        pid4=fork();

        if(pid4==-1){
        perror("Could not create child process");
        exit(-1);
        }else if(pid4==0){
            /*PROCESUL 4*/
            printf("[GRANDCHILD] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
            sleep(60); /*pentru a vizualiza ierarhia de procese*/
            exit(1);

        }
        else{
            /*this code is executed only by child*/
            printf("[CHILD] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
            waitpid(pid4, NULL, 0);
            exit(2);
        }
    }

    pid3 = fork();
    if(pid3 == 0) {
        
        pid5=fork();

        if(pid5==-1){
        perror("Could not create child process");
        exit(-1);
        }else if(pid5==0){
            /*PROCESUL 5*/
            printf("[GRANDCHILD] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
            sleep(60); /*pentru a vizualiza ierarhia de procese*/
            exit(3);
        }

        else{
            /*this code is executed only by child*/
            printf("[CHILD] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
            waitpid(pid5, NULL, 0);
            exit(4);
        }

    }

    /*this code is executed only by a parent*/
    printf("[PARENT] My PID is %d. I created a child with PID %d.\n", getpid(), pid2);
    waitpid(pid2, NULL, 0);
    /*this code is executed only by a parent*/
    printf("[PARENT] My PID is %d. I created a child with PID %d.\n", getpid(), pid3);
    waitpid(pid3, NULL, 0);
    

    return 0;
}