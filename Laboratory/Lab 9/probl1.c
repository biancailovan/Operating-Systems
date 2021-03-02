#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

/*variabile gloabele: 
-> vector de numere intregi
-> n, dimensiunea acestuia
-> nrDiv(nr de divizori pt fiecare element din vector)*/

//#define NR_THREADS 6
#define INTERVAL_LENG 50
#define NR 15

int v[NR]={4,25,44,16,32,59,2, 8, 11, 43, 19, 36, 23,80,40}; /*vector de numere intregi*/
int n=NR;/*dimensiune vector*/
int nrDiv[NR]={0};/*nr de divizori pt fiecare element din vector*/
int nextPos=0;
int nrThreads;

typedef struct{
    int *v;
    int *nrDiv;
    int nREZ; /*nr de elemente din vect resultat*/
    int currentValue;
    pthread_mutex_t *lock1;

}TH_PARAM;

void isDivizor(int x)
{
    int i;
    int count=0;
    for(i=2;i<v[x];i++){
        if(v[x]%i==0){
            count++;
        }
        
    }
    nrDiv[i]=count;
    //printf("Numarul de divizori: %d\n", count);
    printf("Vectorul v[%d] = %d, iar nrDiv[%d] = %d\n",x,v[x],x,count);
}

void *computeDivizors(void *arg)
{
    TH_PARAM *param=(TH_PARAM*)arg;
    //int i;
    int  pos;
    pthread_mutex_lock(param->lock1);
    param->currentValue+=INTERVAL_LENG;
    pos=nextPos;
    nextPos++;
    pthread_mutex_unlock(param->lock1);

    //printf("pos: %d\n", pos);
    while(pos<n){
       
            /*pthread_mutex_lock(param->lock2);
            pos=param->nREZ;
            ++param->nREZ;
            pthread_mutex_unlock(param->lock2);*/
            isDivizor(pos);
            pthread_mutex_lock(param->lock1);
            pos=nextPos;
            nextPos++;
            pthread_mutex_unlock(param->lock1);
         
    }

    return NULL;
}


int main(int argc, char **argv)
{
    //int res[n];
    //n=param->nREZ;
    //TH_PARAM param;
    int i;
    //sscanf(argv[1], "%d", NR_THREADS);
    pthread_t tids[nrThreads];
    pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
    
    //pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

    sscanf(argv[1], "%d", &nrThreads);
    TH_PARAM params[nrThreads];
    //param.currentValue=1;
   // param.lock1=&lock1;

    for(i=0;i<nrThreads;i++){
        params[i].currentValue=i;
        params[i].lock1=&lock1;
        printf("Creating Thread: %d\n", i);
        pthread_create(&tids[i], NULL, computeDivizors, &params[i]);
    }

    for(i=0;i<nrThreads;i++){
        pthread_join(tids[i], NULL);
    }


    pthread_mutex_destroy(&lock1);


    return 0;
}