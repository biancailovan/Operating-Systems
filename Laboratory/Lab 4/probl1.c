#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


/*
    Pentru a delimita liniile din fisier, de tinut cont ca se termina cu '\n'
    Buffer-ul line in care se scrie linia citita are dim maxLength
    Functia returneaza - 0 in caz ca citirea s-a efectuat cu succes
                    -val negative pt caz de eroare
    Program care primeste ca argument un nume de fisier si un numar de linie
    -apeleaza functia getLine

*/

//Functia citeste linia cu nr lineNr din fisierul deschis cu fd
int get_line(int fd, int lineNr, char *line, int maxLength){

    char buff[200];
    int linesNumber = 0;
    int lineLength = 0;
    char str;
    int k=0;

    while(read(fd, &str, 1)>0){

        if(str == '\n'){
            linesNumber++;
        }
    }
        if(linesNumber<lineNr){
            return -1;
        }

        buff[0] = lseek(fd, 0, SEEK_SET);

        while(read(fd, &str, 1)>0){

            if(str =='\n'){
                buff[k]=lseek(fd, 0, SEEK_CUR);
                k++;
            }
        }

        lineLength = buff[lineNr] - buff[lineNr-1];

        if(maxLength<lineLength){
            return -2;
        }

        for(k=0;k<lineLength;k++){
            read(fd, &line[k], 1);
        }

        line[lineLength]=0;
        printf("%s\n", line);

        return 0;
   
}

int main(int argc, char **argv)
{
    int fd=-1;
     //int lineNr=3;
     int maxLength=5;
     char *line=(char*)malloc(maxLength);
  
     if(argc != 3){
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    
    fd=open(argv[1],O_RDONLY);

    if(fd==-1){
    
        perror("Could not open file");
        return 1;
    }
    

    printf("%d\n",get_line(fd,3,line,6));
    
    close(fd);
    return 0;    

}