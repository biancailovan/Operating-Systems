#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//#define BUFSIZE 512

/*
    -se citeste un fisier; se construieste un alt fisier cu liniile inversate
    -o linie poate fi oricat de lunga
    -nume fisier sursa + nume fisier destinatie se dau ca argumente
    NU MERGE!!!!

*/

int main(int argc, char **argv)
{
    int fd1=-1;
    int fd2=-1;
    char str=0;
    char strLine=0;
    //char buf[BUFSIZE];

    fd1 = open(argv[1], O_RDONLY);
    fd2 = creat(argv[2],S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH);

    if(fd1 == -1){
        printf("Could not open file\n");
        return 1;
    }

    if(fd2 == -1){
        printf("Could not open file\n");
        return 2;
    }

    int size = lseek(fd1, 0, SEEK_END);
    //ssize_t start = read(fd1, buf, BUFSIZE);
    //ssize_t end = read(fd1, buf, BUFSIZE);
    //ssize_t tmp;

    /*while(size!=0 && end>0){
        tmp=start;
        for(int i=0;i<start;++i){
            end=start-i+1;

            if(buf[end] == '\n')
                break;
        }

        for(int i=i+1;i<tmp;++i){
            start=tmp-i-1;
            if(buf[start++]=='\n')
                tmp=0;
        }

        if(tmp==0){
            write(fd2, buf+start, end-start+1);
        }

        else{
            write(fd2, buf, end+1);
            end =0;
        }
    
    }*/

    for(int i=0;i<size;i++){
        if(read(fd1, &str,1)!=1){
            perror("Could not read!");
            return 3;
        }
        strLine++;

    }

    int buffer[250];
    lseek(fd1, 0, SEEK_SET);
    int j=0;
    lseek(fd2, size-buffer[j], SEEK_SET);

    for(int i=0;i<size;i++){

        if(read(fd1, &str, 1)!=1){
            perror("Could not read file!");
            return 4;
        }

        if(write(fd2, &str, 1)!=1){
            perror("Something went wrong!");
            return 5;
        }

        if(str=='\n'){
            j++;
            lseek(fd2, size-buffer[i], SEEK_SET);
        }

        if(str == '\0'){
            printf("%ld", size-buffer[j]);
            lseek(fd2, 0, SEEK_SET);
        }
    }

    close(fd1);
    close(fd2);
    return 0;

}


