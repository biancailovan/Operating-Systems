#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FIFO1_NAME "l11_my_fifo1"
#define FIFO2_NAME "l11_my_fifo2"

int main(void)
{
    int fd = -1;
    int fd2 = -1;
    int x = 0, y = 0;
    int result = 0;
    char semn[2];

    //open, read and close fifo
    fd = open(FIFO1_NAME, O_RDONLY);
    if(fd == -1) {
        perror("Could not open FIFO1 for reading");
        return 1;
    }

    fd2 = open(FIFO2_NAME, O_RDONLY);
    if(fd2 == -1) {
        perror("Could not open FIFO2 for reading");
        return 2;
    }
    read(fd, &x, sizeof(x));
    read(fd, semn, sizeof(semn));
    read(fd, &y, sizeof(y));
    printf("The first number is: %d\n", x);
    printf("The sign is: %s\n", semn);
    printf("The second number is: %d\n", y);

    if(strcmp(semn, "+") == 0){
      result = x + y;
    }
    else if(strcmp(semn, "-") == 0){
      result = x -y;
    }

    //close(fd);

    printf("Result: %d\n", result);
    write(fd2, &result, sizeof(char));
    close(fd);
    close(fd2);

    return 0;
}
