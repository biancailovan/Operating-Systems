#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int check_vowel(char ch)
{
    if (ch == 'a' || ch == 'A' || ch == 'e' || ch == 'E' || ch == 'i' || ch == 'I' || ch =='o' || ch=='O' || ch == 'u' || ch == 'U')
      return 1;
    else
      return 0;
}

int main(int argc, char **argv)
{
    int fd;
    off_t size, i;
    char *data = NULL;
    char *t = NULL;
    int c = 0;
    int d = 0;
    
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

    for(i=0;i<size;i++) {
        if(check_vowel(data[c]) == 0) {      
            t[d] = data[c];
            d++;
            printf("%c", data[i]);
         }
    }

    t[d] = '\0';
    printf("\n");

    munmap(data, size);
    close(fd);

    return 0;
}
