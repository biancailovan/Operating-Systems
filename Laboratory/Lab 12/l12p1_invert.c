#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int fd;
    off_t size, i;
    char *data = NULL;
    char *data_inv = NULL;
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        perror("Could not open input file");
        return 1;
    }
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    data = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(data == (void*)-1) {
        perror("Could not map file");
        close(fd);
        return 1;
    }

    //data_inv = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    data_inv = (char*) malloc(size*sizeof(char));
    if(data_inv == (void*)-1) {
        perror("Could not map file");
        close(fd);
        return 1;
    }


    for(i=1; i<=size-1; i++) {
        data_inv[i] = data[size-i-1];
    }
    data_inv[size] = data[0];
    for(off_t j=1; j<=size-1;j++)
        printf("%c", data_inv[j]);
    printf("\n");

    munmap(data, size);
    //munmap(data_inv, size);
    close(fd);

    return 0;
}
