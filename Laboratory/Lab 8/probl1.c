#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define N 4
#define M 1000

long count=0; /*cariabila globala de tipul long*/

void *thread_function(void *unused)
{
    sem_t *logSem=(sem_t *)unused;
    long aux;
    sem_wait(logSem);

    for(int i=0;i<M;i++){
        aux=count;
        aux++;
        usleep(random()%10);
        count=aux;
    }

    sem_post(logSem);

    return NULL;
}

int main()
{
    sem_t logSem;
    pthread_t threads[N];

    srand(time(NULL));

    if(sem_init(&logSem, 0, 1)!=0){
        perror("Could not init the semaphore");
        return -1;
    }

    for(int i=0;i<N;i++){
        if(pthread_create(&threads[i], NULL, thread_function, &logSem)!=0){
            perror("ERROR\n thread not craeted");
            return -2;
        }
    }

    for(int i=0;i<N;i++){
        pthread_join(threads[i], NULL);
    }

    printf("Valoarea lui count este: %ld\n", count);

    sem_destroy(&logSem);

    return 0;
}