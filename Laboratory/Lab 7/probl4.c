#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include<time.h>

/*nu cred ca merge chiar ok dar nu-mi dau seama de ce*/

#define NR_THREADS 9

void *myThreadFun(void *unused) 
{ 
    //sleep(1); 
    srand(time(NULL));
    int status=0;
    while(!status){
        int sleepingFor=rand()%5+1;
        long int unused2= (long)unused;
        printf("\nThread: %ld is sleeping for %d seconds", unused2, sleepingFor);
        sleep(sleepingFor);
    //printf("sleeping for %d seconds", sleepingFor);
    }
  
    return NULL; 
} 
   
int main() 
{ 
    int idThread;
    int status=0;
    pthread_t idt[NR_THREADS];

    for(int i=0;i<NR_THREADS;i++){
        long int nr=i;
        pthread_create(&idt[i], NULL, myThreadFun,(void*)nr); 
    }

    while(status<NR_THREADS){
        sleep(6);
        printf("\nThe thread you want to stop is: \n");
        sleep(3);
        scanf("%d", &idThread);
        sleep(3);
        pthread_cancel(idt[idThread]);
        status++;
    }
    
    //pthread_join(thread_id, NULL); 
    //printf("After Thread\n"); 
    //exit(0); 
    return 0;

}