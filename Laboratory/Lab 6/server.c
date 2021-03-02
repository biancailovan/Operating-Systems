#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char ** argv)
{
    printf("[SERVER] PID=%d ParentPID=%d\n", getpid(), getppid());

    int numar1 = atoi(argv[0]);
    printf("%d\n",numar1 );
	int numar2 = atoi(argv[1]);
	printf("%d\n",numar2 );

	int ok = 0;

	if(strcmp(argv[2],"+")==0)
		ok=1;
	else if(strcmp(argv[2],"-")==0)
		ok=2;

	int rezultat=0;
	if(ok == 1)
		rezultat = numar1 + numar2;
	if(ok == 2)
		rezultat = numar1 - numar2;

    printf("Rezultatul este %d\n",rezultat);

	exit(rezultat);

	return 0;

}