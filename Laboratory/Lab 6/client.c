#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

 int main(int argc,char ** argv)
{

	int childPid; 
	int status;

	printf("[CLIENT] PID=%d ParentPID=%d\n", getpid(), getppid());
	
	while(1)/*putem introduce comenzi cat timp nu se introduce exit*/
	{

        /*afisare prompt, nu stiu daca e ok ca nu am inteles prea bine*/
		printf("\n>");
		
		char numar1[3], numar2[3], semn[3];

        /*citesc cele 2 numere, dupa care citesc semnul*/
		scanf("%s", numar1);
		printf("%s\n", numar1);
		scanf("%s", numar2);
		printf("%s\n", numar2);
		scanf("%s", semn);
		printf("%s\n", semn);
	
		childPid = fork();
		if (childPid < 0) {/*verific daca am eroare, caz in care va da exit*/
			perror("Error creating new process");
			exit(1);
		}
		
        /*daca fork returneaza o valoare>0 atunci suntem in parinte*/
		if (childPid > 0) /*parinte*/
        { 
			printf("[Parent] ChildPID=%d is executing \n", childPid);
				
			waitpid(childPid, &status, 0);
			signed char rezultat;
			rezultat = WEXITSTATUS(status);
			
			if(WIFEXITED(status))
				printf("Server exited with %d\n", WEXITSTATUS(status));

			printf("REZULTAT %d\n", rezultat);
			printf("[Parent] Terminate the execution\n");

		} 

        else 
        {/*daca nu suntem in parinte, suntem in copil si executam codul*/	
		
			execl("./server", numar1, numar2, semn, NULL);
		
            /*pentru cazurile cand nu se executa cu succes execlp*/
			perror("Error executing execl");
			printf("[Child] Terminate the execution\n");
			exit(1);
		}

	}
	
	return 0;
}