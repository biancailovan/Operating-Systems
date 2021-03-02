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

#define max 1000

int n ;
int nr=0;
int value[max];

sem_t Na, Cl, Na_Cl, Cl_Na, print;

void *Cl_function(void *unused){
    sem_wait(&Cl_Na);

    sem_post(&Cl);
    sem_wait(&Na);
    sem_wait(&print);
    printf("NaCl din clor\n");
    sem_post(&print); 

    sem_post(&Cl_Na);

    return NULL;
}

void *Na_function(void *unused){
    sem_wait(&Na_Cl); 

    sem_post(&Na);
    sem_wait(&Cl);
    sem_wait(&print);
    printf("NaCl din natriu\n");
    sem_post(&print);

    sem_post(&Na_Cl); 

    return NULL; 
}

int main(int argc, char *argv[])
{
    pthread_t th;
    sem_init(&Na, 0, 0);
    sem_init(&Cl, 0, 0);
    sem_init(&Na_Cl, 0, 1);
    sem_init(&Cl_Na, 0, 1);
    sem_init(&print, 0, 1);
    srand(time(NULL)); 
 
    while(1){
        int rand;
        printf("1 pt clor, 0 pt natriu: ");
        scanf("%d", &rand);
        nr++;
        if(rand){
            pthread_create(&th, NULL, Cl_function, &value[nr]);
        }
        else
        {
            /* code */
            pthread_create(&th, NULL, Na_function, &value[nr]);
        }

        if(nr>1000){
            break; 
        }
        
    }  

    return 0;
}



