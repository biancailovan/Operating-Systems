#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int shmFd;
    //char *sharedChar = NULL;
    int *sharedInt = NULL;
    int nr = 0;

    shmFd = shm_open("/probl2_myshm", O_RDWR, 0);
    if(shmFd < 0) {
        perror("Could not aquire shm");
        return 1;
    }
    //sharedChar = (char*)mmap(0, sizeof(char), PROT_READ | PROT_WRITE, 
        //MAP_SHARED, shmFd, 0); ;
    sharedInt = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, 
        MAP_SHARED, shmFd, 0); ;
    if(sharedInt == (void*)-1){
        perror("Could not map the shared memory");
        return 1;
    }

    printf("found sharedInt: %d\n", *sharedInt);
    //*sharedChar = 'X';
    while(*sharedInt != nr) {
        //if(*sharedInt != nr){
        printf("sharedInt: %d\n", *sharedInt);
        nr = *sharedInt;
        nr++;
        *sharedInt = nr;
        sleep(1);
       // }
    }

    munmap(sharedInt, sizeof(int));
    sharedInt = NULL;

    return 0;
}
