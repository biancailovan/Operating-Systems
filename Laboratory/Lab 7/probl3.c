#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void *threadFn(void *unused)
{
    //printf("Secondary thread\n");
    return NULL;
}

int main()
{
    pthread_t tid = -1;
    int contor=0;

    while(pthread_create(&tid, NULL, threadFn, NULL) == 0){
        //perror("Error creating thread");
        //return 1;
        contor++;

    }
    printf("Numarul de thread-uri create este de: %d\n", contor);
    //pthread_join(tid, NULL);
    return 0;
}