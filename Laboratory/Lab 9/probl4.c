#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define N 6
#define NR_THREADS 6

//nu prea cred ca e bine, am incercat ish

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int i=0;
int j=0;

void enter_bridge(int dir)
{
    pthread_mutex_lock(&lock);
    printf("Car enter bridge\n");
    printf("direction: %d\n", dir);

    if(dir==0){
        while(dir==0&&i!=0){
            pthread_cond_wait(&cond, &lock2);
        }

        pthread_mutex_lock(&lock2);
        i++;
        if(i==N){
            pthread_mutex_lock(&lock2);
        }
        else{
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&lock2);
        }

    }

    else if(dir==1){
        while(dir==1&&j!=0){
            pthread_cond_wait(&cond, &lock1);
        }

        pthread_mutex_lock(&lock1);
        j++;
        if(j==N){/*mi-am definit un numer maxim de masini pe pod*/
            pthread_mutex_lock(&lock1);
        }
        else{
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&lock1);
        }

    }

    pthread_mutex_unlock(&lock);
}

void cross_bridge(int dir)
{
    /*vreau sa verific daca sunt masini pe pod din cealalta directie*/
    /*asteapta atata timp cat sunt*/
    if(dir==0){
        while(dir==0&&i!=0){
            pthread_cond_wait(&cond, &lock);
            pthread_mutex_lock(&lock);
        }

        if(dir==0&&i==0){
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&lock);
        }

    }

    else if(dir==1){
        while(dir==1&&j!=0){
            pthread_cond_wait(&cond, &lock);
            pthread_mutex_lock(&lock);
        }

        if(dir==1&&j==0){
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&lock);
        }

    }

    printf("Car is crossing the bridge");

    //pthread_mutex_unlock(&lock);
}

void exit_bridge(int dir)
{
    pthread_mutex_lock(&lock);
    if(dir==0){
        pthread_mutex_unlock(&lock2);
        pthread_cond_broadcast(&cond);
        i--;
    }

    else if(dir==1){
        pthread_mutex_unlock(&lock1);
        pthread_cond_broadcast(&cond);
        j--;
    }

    printf("Car exited bridge!");
    //printf(", id:%ld", pthread_self());

}

void *car_thread(void *direction)
{
    int dir = (int)direction;
    enter_bridge(dir);
    cross_bridge(dir);
    exit_bridge(dir);

    return NULL;
}

int main()
{
    pthread_t tids[NR_THREADS];
    int k;
    for(k=0;k<NR_THREADS;k++){
        printf("Creating thread: %d\n", k);
        pthread_create(&tids[k], NULL, car_thread, (void*)1);
        pthread_create(&tids[k], NULL, car_thread, (void*)0);
    }

    for(k=0; k<NR_THREADS; k++) {
        pthread_join(tids[k], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_cond_destroy(&cond);

    return 0;
    
}