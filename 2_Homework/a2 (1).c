#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include<pthread.h>
#include <semaphore.h>

/*TEMA 2: Procese si Thread-uri*/

/*variabile globale*/
int nrThreads=0;
int myThreads=0;
int haveFound=0;
int turn=0;
/**/

pthread_mutex_t mutexONE, mutexTWO, mutexTHREE, mutexFOUR, lock;
pthread_cond_t cond;
sem_t sema;

typedef struct thread_arg_t
{
	int process;
	int thread;
} STRUCT;

/*Sincronizarea thread-urilor din acelasi proces*/
/*T3.5 trebuie sa inceapa inainte ca T3.4 sa inceapa si trebuie sa se incheie dupa terminarea acestuia*/

/*P3 creeaza 5 thread-uri*/
void funcINIT(STRUCT *thinfo)
{
	if (thinfo->process == 3 && thinfo->thread == 4)
		pthread_mutex_lock(&mutexONE);
	info(BEGIN, thinfo->process, thinfo->thread);
	if (thinfo->process == 3 && thinfo->thread == 5)
		pthread_mutex_unlock(&mutexONE);
	if (thinfo->process == 3 && thinfo->thread == 5)
		pthread_mutex_lock(&mutexTWO);
	info(END, thinfo->process, thinfo->thread);
	if (thinfo->process == 3 && thinfo->thread == 5)
		pthread_mutex_unlock(&mutexTWO);

}

/*Sincronizarea thread-urilor din procese diferite*/
/*T5.1 trebuie sa se incheie inainte ca thread-ul T3.3 sa porneasca
  T5.5 nu poate incepe decat dupa T3.3 s-a terminat*/

/*P5 creeaza 6 thread-uri*/
void functionP3(STRUCT *thinfo)
{

	/*if (thinfo->process == 3 && thinfo->thread == 4)
		pthread_mutex_lock(&mutexONE);*/

	//5.1 porneste dupa ce se termina 3.3
	if (thinfo->process == 3 && thinfo->thread == 3)
		pthread_mutex_lock(&mutexTHREE);
	if (thinfo->process == 5 && thinfo->thread == 5)
		pthread_mutex_lock(&mutexFOUR);

	if (thinfo->process == 3 && thinfo->thread == 4)
		pthread_mutex_lock(&mutexONE);


	info(BEGIN, thinfo->process, thinfo->thread);
	if (thinfo->process == 3 && thinfo->thread == 5)
		pthread_mutex_unlock(&mutexONE);
	if (thinfo->process == 3 && thinfo->thread == 5)
		pthread_mutex_lock(&mutexTWO);
	info(END, thinfo->process, thinfo->thread);
	if (thinfo->process == 3 && thinfo->thread == 4)
		pthread_mutex_unlock(&mutexTWO);

	if (thinfo->process == 5 && thinfo->thread == 1)
		pthread_mutex_unlock(&mutexTHREE);
	if (thinfo->process == 3 && thinfo->thread == 3)
		pthread_mutex_unlock(&mutexFOUR);


}

//5.1 se incheie inainte ca 3.3 sa porneasca
//5.5 incepe dupa ce 3.3 s-a terminat

//3.5 incepe inainte ca 3.4 sa porneasca
//3.5 se incheie dupa 3.4

void *th_function(void *arg){
	//int idTh=*(int*)arg;
	pthread_mutex_lock(&lock);
	if(turn){
		while(turn){
			//pthread_cond_wait(&lock, &cond);
			sem_wait(&sema);
		}
	}
	nrThreads++;
	if(nrThreads == 49){
		turn = 1;
	}
	pthread_mutex_unlock(&lock);

	pthread_mutex_lock(&lock);
	nrThreads--;
	if(turn && nrThreads == 0){
		turn=0;
		pthread_cond_broadcast(&cond);

	}
	pthread_mutex_unlock(&lock);
	sem_post(&sema);
	return NULL;
}

/*BARIERA DE THREAD-URI*/
/*cel mult 5 thread-uri din procesul P4 pot rula simultan*/
void thFunction4(STRUCT *args){
	sem_wait(&sema);
	if(args->thread != 10){
		pthread_mutex_lock(&lock);/*LOCK*/
		nrThreads++;
	if(args->thread == 10)
		haveFound = 1;
	pthread_mutex_unlock(&lock);/*UNLOCK*/
	info(BEGIN, 4, args->thread);
	}
	if(args->thread != 10){
		pthread_mutex_lock(&lock);/*LOCK*/
		if(turn == 0 && nrThreads == 4){
			info(BEGIN, 4, 10);
			info(END, 4, 10);
			turn = 1 ;
		}
	while(haveFound){
		if(haveFound == 0 && myThreads == 49){
			pthread_mutex_lock(&lock);/*LOCK*/
			nrThreads++;
			haveFound = 1;
			pthread_mutex_unlock(&lock);/*UNLOCK*/
			info(BEGIN, 4, 10);
		}
	}

		pthread_mutex_unlock(&lock);/*UNLOCK*/

		pthread_mutex_lock(&lock);/*LOCK*/
		nrThreads--;
		pthread_mutex_unlock(&lock);/*UNLOCK*/

		info(END, 4, args->thread);

	}

	sem_post(&sema);

}


/*Creeaza thread-uri pentru P3*/
void create_threads(pthread_t tids[], STRUCT thread[], int n){
	for (int i =0; i < n; i++)
		pthread_create(&tids[i], 0, (void *(*)(void *))functionP3, (void *)&thread[i]);
	for (int i = 0; i < n; i++)
		pthread_join(tids[i], 0);
}

/*Creeaza thread-uri pentru P4*/
void create_threads4(pthread_t tids[], STRUCT thread[], int n){
	sem_init(&sema, 0, 5);
	pthread_mutex_init(&lock, 0);
	for (int i=0; i < n; i++)
		pthread_create(&tids[i], 0, (void *(*)(void *))thFunction4, (void *)&thread[i]);
	for (int i = 0; i < n; i++)
		pthread_join(tids[i], 0);
	
}

/*Sincr th din acelasi proces*/
void create_threadsINIT(pthread_t tids[], STRUCT thread[], int n){
	for (int i=0; i < n; i++)
		pthread_create(&tids[i], 0, (void *(*)(void *))funcINIT, (void *)&thread[i]);
	for (int i = 0; i < n; i++)
		pthread_join(tids[i], 0);
}

int pid2, pid3, pid4, pid5, pid6, pid7, pid8;

/*Ierarhia de procese*/
int create_process(int id){

    int pid;
    if ((pid = fork()) == 0){
		info(BEGIN, id,  0);

		if (id == 2){
            pthread_mutex_init(&mutexONE, 0);
			pthread_mutex_init(&mutexTWO, 0);
			pid6 = create_process(6);
			waitpid(pid6, 0, 0);
		}
		else if (id == 6){
			pid7 = create_process(7);
			waitpid(pid7, 0, 0);
		}
		
        else if (id == 3){
			pthread_mutex_init(&mutexONE, 0);
			pthread_mutex_init(&mutexTWO, 0);
			pthread_mutex_init(&mutexTHREE, 0);
			pthread_mutex_init(&mutexFOUR, 0);
		
			pid8 = create_process(8);
			pthread_t threads[11];
			//pthread_mutex_lock(&mutexTWO);
            pthread_mutex_lock(&mutexONE);
			pthread_mutex_lock(&mutexTHREE);
			pthread_mutex_lock(&mutexTWO);
			pthread_mutex_lock(&mutexFOUR);
			//pthread_mutex_lock(&mutexTWO);
			//STRUCT args[5] = {{3,1},{3,2},{3,3},{3,4}, {3, 5}};
			STRUCT args[11] = {{3,1},{3,2},{3,3},{3,4}, {3, 5},{5,1},{5,2},{5,3},{5,4},{5, 5},{5,6}};
			//pthread_mutex_lock(&mutexONE);
            //pthread_mutex_lock(&mutexTWO);
			create_threads(threads, args, 11);
		
			waitpid(pid8, 0, 0);
		}

        else if (id == 4){
			pthread_mutex_init(&mutexONE, 0);
			pthread_mutex_init(&mutexTWO, 0);
            pthread_t threads[49];
			STRUCT args[49] = {{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8},{4,9},{4,10},
							{4,11},{4,12},{4,13},{4,14},{4,15},{4,16},{4,17},{4,18},{4,19},{4,20},
							{4,21},{4,22},{4,23},{4,24},{4,25},{4,26},{4,27},{4,28},{4,29},{4,30},
							{4,31},{4,32},{4,33},{4,34},{4,35},{4,36},{4,37}, {4,38},{4,39},{4,40},{4,41},{4,42},{4,43},{4,44},
                            {4,45},{4,46},{4,47},{4,48},{4,49}};
			//create_threads(threads, args, 49);
			create_threads4(threads, args, 49);
			//pid4 = create_process(4);
			pid5 = create_process(5);
           	//pthread_t threads2[6];
			//pthread_mutex_lock(&mutexTHREE);
			//STRUCT args2[6] = {{5,1},{5,2},{5,3},{5,4}, {5, 5},{5, 6}};
			//STRUCT args2[11] = {{3,1},{3,2},{5,1},{3,3},{5,2},{5,3},{5,4}, {5, 5},{3,4},{3,5},{5, 6}};
			//pthread_mutex_lock(&mutexFOUR);
			//create_threads(threads2, args2, 6);
			
			waitpid(pid5, 0, 0);
		}

		info(END, id, 0);
		exit('\0');
	}
	else

		return pid;

}

int main()
{
    init();

	/*procesul parinte*/
    info(BEGIN, 1, 0);

    pid2 = create_process(2);
    pid3 = create_process(3);
    pid4 = create_process(4);

	/*copii sai*/
    waitpid(pid2, 0, 0);
    waitpid(pid3, 0, 0);
    waitpid(pid4, 0, 0);
    
    info(END, 1, 0);

    return 0;
	
}
