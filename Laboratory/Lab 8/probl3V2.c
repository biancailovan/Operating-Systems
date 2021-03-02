#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#define NR 25

/*nu se intampla nimic, nu stiu de ce*/

typedef struct {
    int value; //id
    sem_t *logSem;
} TH_STRUCT;

sem_t seems[3];
//int k;
void *thread_function( void* unused){
        //int i;
        TH_STRUCT *s=(TH_STRUCT*)unused;
        while(1)
        {
                //for(i=1;i<=3;i++){
                 sem_wait(&seems[s->value]);
                // }
                 printf("%d ", s->value);
                        //for(int i=1;i<=3;i++){
                        //printf("%d ", i);
                        //}
                fflush(stdout);
                sem_post(&seems[s->value+1]);    
        
        }
        return NULL;

}

int main(int argc, char *argv[])
{
    //sem_t logSem;
    //int i;
    TH_STRUCT params[NR];
    pthread_t tids[NR];
    //seems=(sem_t*)malloc(3*sizeof(sem_t));

    //initialize random number generator
    //srand(time(NULL));
    //initialize the semaphore
    for(int i=1;i<=3;i++){
    //if(sem_init(&seems[i], 0, 0) != 0) {
       // perror("Could not init the semaphore");
        //return -1;
        sem_init(&seems[i], 0, 0);
    }
    //create the threads
    for(int i=1; i<=3; i++) {
        params[i].value = i;
        //params[i].logSem = &logSem;
        pthread_create(&tids[i], NULL, thread_function, &params[i]);
    }
    sem_post(&seems[0]);
    //wait for the threads to finish
    for(int i=1; i<=3; i++) {
        pthread_join(tids[i], NULL);
    }
    //destroy the semaphore
    for(int i=1;i<=3;i++){
    sem_destroy(&seems[i]);
    }

    return 0;
}
