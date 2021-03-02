#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
/*3 ingrediente: tutun, hartie si foaie*/

#define NR_OF_SMOKERS 3
#define TUTUN 1
#define HARTIE 2
#define BRICHETA 3

int ingredient;
pthread_mutex_t lock;
pthread_cond_t cond[NR_OF_SMOKERS+1];

void *thread_fumator(void *arg){
    int id=*(int*)arg;/*id: ingredientul pe care fumatorul in are*/
    for(;;) {
		pthread_mutex_lock(&lock);
		while (ingredient != id) {
			pthread_cond_wait(&cond[id], &lock);
		}
		ingredient = -1;
		pthread_cond_signal(&cond[0]);
		pthread_mutex_unlock(&lock);
    	printf("Fumatorul %d: ia ingredientele ..\n\n", id);
    }

    return NULL;
}

void printIngredients(int x)
{
	printf("Dealer-ul ofera");
	if (x == TUTUN) {
		printf("hartie si bricheta");
	} else if (x == HARTIE) {
		printf("tutun si bricheta");
	} else {
		printf("tutun si hartie");
	}
}

void *thread_dealer(void *arg){
    int random;
    srand((unsigned)time(NULL));
    random=rand()%3;
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond[0], &lock);
    sleep(1);
    ingredient=random;
    printIngredients(random);
    pthread_cond_signal(&cond[1]);
	pthread_cond_signal(&cond[2]);
	pthread_cond_signal(&cond[3]);
	pthread_mutex_unlock(&lock);

    return NULL;

}

int main()
{
    int id[NR_OF_SMOKERS];
    pthread_t tids[NR_OF_SMOKERS];/*thread fumator*/
    pthread_t tdealer;/*thread dealer*/
    if (pthread_create(&tdealer, NULL, thread_dealer, NULL) != 0) {
		printf("FAIL!!!\n");
	}
    for(int i=0;i<NR_OF_SMOKERS;i++){
        id[i]=i+1;
        pthread_create(&tids[i], NULL, thread_fumator, &id[i]);
    }

    for(int i=0;i<NR_OF_SMOKERS;i++){
        id[i]=i+1;
        pthread_create(&tids[i], NULL, thread_fumator, &id[i]);
        pthread_join(tids[i], NULL);
    }
    pthread_join(tdealer, NULL);

    //pthread_join(tdealer, NULL); 
  


    return 0;

}