#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO1_NAME "l11_my_fifo1"
#define FIFO2_NAME "l11_my_fifo2"

int main(void)
{
    int fd = -1;
	int fd2 = -1;
    int x = 0, y = 0;
	char semn[2];
	int result = 0;
	scanf("Dati primul nr: %d\n", &x);
	scanf("Dati semnul: %s\n", semn);
	scanf("Dati al doilea nr: %d\n", &y);

    //create fifo1
    if(mkfifo(FIFO1_NAME, 0600) != 0) {
        perror("Could not create pipe1");
        return 1;
    }

	//create fifo2
    if(mkfifo(FIFO2_NAME, 0600) != 0) {
        perror("Could not create pipe2");
        return 2;
    }

    //open, write and close fifo1
    fd = open(FIFO1_NAME, O_WRONLY);
    if(fd == -1) {
        perror("Could not open FIFO1 for writing");
        return 1;
    }

	//open, write and close fifo2
    fd2 = open(FIFO2_NAME, O_WRONLY);
    if(fd2 == -1) {
        perror("Could not open FIFO2 for writing");
        return 2;
    }
    printf("Writing value %d to FIFO\n", x);
    write(fd, &x, sizeof(x));
	write(fd, &semn, sizeof(semn));
	write(fd, &y, sizeof(y));
	read(fd2, &result, sizeof(int));
	printf("Result: %d\n", result);
    close(fd);
	close(fd2);

    //delete fifo1
    unlink(FIFO1_NAME);
	//delete fifo2
    unlink(FIFO2_NAME);

    return 0;
}
