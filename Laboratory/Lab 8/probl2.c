#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define NR_THREADS 16

typedef struct {
    int value;
    sem_t *logSem;
} TH_STRUCT;

long count=0;
int nrThreads;/*var globala de tip int*/

void *limited_area(void *unused){
    //long aux;
    //int nrThreads;
    //TH_STRUCT s = (TH_STRUCT*)unused;
    TH_STRUCT *s=(TH_STRUCT*)unused;
    sem_wait(s->logSem);
    //aux=nrThreads;
    /**/
    nrThreads++;
    usleep(100);
    printf("The number of threads in the limited area is: %d\n", nrThreads);
    count=nrThreads;
    nrThreads--;
    sem_post(s->logSem);
    return NULL;
    /**/

}

int main(int argc, char *argv[])
{
    //int nrThreads;
    sem_t logSem;
    int i;
    TH_STRUCT params[NR_THREADS];
    pthread_t tids[NR_THREADS];

    //initialize random number generator
    srand(time(NULL));
    int N=atoi(argv[1]);
    //initialize the semaphore
    if(sem_init(&logSem, 0, N) != 0) {
        perror("Could not init the semaphore");
        return -1;
    }
    //create the threads
    for(i=0; i<NR_THREADS; i++) {
        params[i].value = 10 * (i + 1);
        params[i].logSem = &logSem;
        pthread_create(&tids[i], NULL, limited_area, &params[i]);
    }
    //wait for the threads to finish
    for(i=0; i<NR_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }
    //destroy the semaphore
    sem_destroy(&logSem);

    return 0;
}
    
    
