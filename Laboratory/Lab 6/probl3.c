#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

/*nr total=>2^7-1*/
/*nu stiu daca am inteles bine problema*/
/*nu merge pentru n mai mare decat 8*/

int main()
{
    pid_t beforePid, afterPid, forkedPid;
    int count=0;
    int i;
    int status;

    beforePid = getpid();
    for (i = 0; i < 7; i++)
    {
        forkedPid = fork();

        if (forkedPid > 0) 
        {
            waitpid(forkedPid, &status, 0);
            /* proces parinte, numara copiii si descendentii */
            count = count + WEXITSTATUS(status); 
        } 
        else 
        {
            /* proces copil */
            count = 1;
        }
    }

    afterPid = getpid();
    if (afterPid == beforePid) 
    {
        printf("%d procese au fost creeate\n", count);
    }

    return count;

}