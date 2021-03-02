#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define NR_OF_PLACES 5
#define TOTAL_PERS 20

int nrOfPersons=0;
int haveGroup=0;

pthread_mutex_t TABLElock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t FREEtable=PTHREAD_COND_INITIALIZER;

void *thread_person(void *arg){
    int idPers=*(int*)arg;
    pthread_mutex_lock(&TABLElock);

    if(haveGroup){
        printf("%d grup format, ",idPers);
        printf("nr persoane la masa: %d\n", nrOfPersons);
        while(haveGroup){/*cat timp am un grup, astept pana masa e libera*/
            /*var conditilonala FREE table*/
            pthread_cond_wait(&FREEtable, &TABLElock);

        }
    }
        printf("%d grupul a plecat, ", idPers);
        printf("nr pers la masa: %d\n", nrOfPersons);

        nrOfPersons++;/*incr nr total de persoane de la masa*/
        if(nrOfPersons==NR_OF_PLACES){/*in acest caz se formeaza grup*/
            haveGroup=1;
        }
        /*manca*/
        pthread_mutex_unlock(&TABLElock);

        usleep(rand()%1000000);
        pthread_mutex_lock(&TABLElock);
        /*dupa ce terminam de mancat, actualiam nr curent de pers de la masa*/
        nrOfPersons--;
        if(haveGroup && nrOfPersons==0){/*asta inseamna ca celelalte pers care asteapta sa manance au acces la masa*/
            haveGroup=0;
            pthread_cond_broadcast(&FREEtable);
        }
        pthread_mutex_unlock(&TABLElock);

    return NULL;
}

int main()
{
    int idPers[TOTAL_PERS];
    pthread_t tids[TOTAL_PERS];
    for(int i=0;i<TOTAL_PERS;i++){
        idPers[i]=i+1;
        pthread_create(&tids[i], NULL, thread_person, &idPers[i]);
    }

    for(int i=0;i<TOTAL_PERS;i++){
        idPers[i]=i+1;
        pthread_create(&tids[i], NULL, thread_person, &idPers[i]);
        pthread_join(tids[i], NULL);
    }
    pthread_mutex_destroy(&TABLElock);
    pthread_cond_destroy(&FREEtable);


    return 0;
}