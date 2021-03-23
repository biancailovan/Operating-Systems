#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define PIPE_1 "REQ_PIPE_35355" /*pipe-ul din care citeste, pipe request*/ 
#define PIPE_2 "RESP_PIPE_35355" /*pipe-ul in care scrie, pipe response*/

void valid_SF(int fd){
    char magic[5];
    int header_size = 0;
    int version = 0;
    int no_of_sections = 0;
    char sect_name[16];
	int sect_type = 0;
	int sect_offset = 0;
	int sect_size = 0;

    lseek(fd, -5, SEEK_END);
    read(fd, &header_size, 2);
    read(fd, magic, 4);
    magic[4] = '\0';

    if(strcmp(magic, "5YIE")!=0)
    {
        printf("wrong magic\n");
        return;
    }

    lseek(fd, -header_size, SEEK_CUR);
    read(fd, &version, 1);

    if(version < 61 || version > 139)
    {
        printf("wrong version\n");
        return;
    }

    read(fd, &no_of_sections, 1);

    if(no_of_sections < 6 || no_of_sections > 13)
    {
        printf("wrong sect_nr\n");
        return;
    }

    for(int i = 0;i < no_of_sections; i++)
    {
        read(fd, sect_name, 15);
        sect_name[15]='\0';
        read(fd, &sect_type, 2);

        if(sect_type != 23 && sect_type != 70 && sect_type != 84 && sect_type != 20 && sect_type != 55 && sect_type != 80)
        {
            printf("wrong sect_types\n");
            return;
        }

        read(fd, &sect_offset, 4);
        read(fd, &sect_size, 4);
    }

    read(fd, &header_size, 2);
    read(fd, magic, 4);
    magic[4] = '\0';

    lseek(fd, -header_size, SEEK_CUR);
    lseek(fd, 5, SEEK_CUR);//version=1, magic=4

    printf("SUCCESS\n");
    printf("version=%d\n", version);
	printf("nr_sections=%d\n", no_of_sections);

    for(int i = 0;i < no_of_sections; i++)
    {
        read(fd, sect_name, 15);
        sect_name[15] = '\0';
        read(fd, &sect_type, 2);
		read(fd, &sect_offset, 4);
		read(fd, &sect_size, 4);
		printf("section%d: %s %d %d\n", i + 1, sect_name, sect_type, sect_size);

    }
}

int main()
{

    int fd1 = -1, fd2 = -1, fd3_MAP = -1, fd_READ = -1;
    unsigned int nr = 35355;
    char success[8] = "SUCCESS";
    char error[6] = "ERROR";
    char connect_message[7] = "CONNECT";
    char request_message[25];
    char response_message[5] = "PONG";
    const char* name = "/Fe7iJN0";
    unsigned int size = 0;
    unsigned int shmId = -1;
    void* ptr;
    char *shDATA = NULL;
    unsigned int offset = 0;
    unsigned int value = 0;/*valoarea care trebuie scrisa in reg de memorie partajata*/
    unsigned int file_size1 = 0;
    unsigned int file_size2 = 0;
    //char file_name[25];
    char* file_name;
    char *myDATA = NULL;
    unsigned int no_of_bytes = 0;

    /*2.2 Conexiunea prin pipe*/

    /*PAS 1, creez RESP_PIPE_35355*/
    if (mkfifo(PIPE_2, 0600) != 0){ 
        perror("ERROR\n");
        perror("cannot create the response pipe");
        return 1;
    }

    /*PAS 2, deschid in citire REQ_PIPE_35355*/
    fd1 = open(PIPE_1, O_RDONLY);
    if (fd1 == -1){
        perror("ERROR\n");
        perror("cannot open the request pipe");
        return 3;
    }

    /*PAS 3, deschid in scriere RESP_PIPE_35355*/
    fd2 = open(PIPE_2, O_WRONLY);
    if (fd2 == -1){
        perror("ERROR\n");
        perror("cannot open the request pipe");
        return 3;
    }

    /*PAS 4, scriu mesajul CONNECT in RESP_PIPE_35355*/
    size = 7;
    write(fd2, &size, 1);
    write(fd2, &connect_message, size);

    /*Daca toti pasii s-au executat cu succes, programul afiseaza: */
    printf("SUCCESS\n");

    for (;;)
    {
        size = 4;
        read(fd1, &size, 1);
        read(fd1, &request_message, size);
        request_message[(int)size] = '\0';
        printf("%s", request_message);

        /*2.3 Request ping*/
        if (strncmp("PING", request_message, 4) == 0){
            write(fd2, &size, 1);
            write(fd2, &request_message, size);

            write(fd2, &size, 1);
            write(fd2, &response_message, size); //PONG

            size = sizeof(unsigned int);
            write(fd2, &nr, size);
        }

        /*2.4 Request pentru crearea unei regiuni de memorie partajata*/
        if (strncmp("CREATE_SHM", request_message, 10) == 0){
            read(fd1, &shmId, sizeof(unsigned int));

            shmId = shm_open(name, O_CREAT | O_RDWR, 0664);
            ftruncate(shmId, 4700637);
            ptr = mmap(0, 4700637, PROT_WRITE, MAP_SHARED, shmId, 0); 

            if (shmId < 0){
                size = 10;
                write(fd2, &size, 1);
                write(fd2, &request_message, size);
                sprintf(ptr, "%s", request_message);
                ptr+=strlen(request_message);

                size = 5;
                write(fd2, &size, 1);
                write(fd2, &error, size);
                ptr+=strlen(error);
            }

            else{
                shDATA = mmap(0, 4700637, PROT_WRITE, MAP_SHARED, shmId, 0);
                printf("%s", shDATA);
                
                if (shDATA == (void *)-1){
                    size = 10;
                    write(fd2, &size, 1);
                    write(fd2, &request_message, size);
                    ptr+=strlen(request_message);

                    size = 5;
                    write(fd2, &size, 1);
                    write(fd2, &error, size);
                }

                size = 10;
                write(fd2, &size, 1);
                write(fd2, &request_message, size);

                size = 7;
                write(fd2, &size, 1);
                write(fd2, &success, size);
            }
        }
        
        /*2.5 Request pentru scrierea in memoria paratajata*/
        if (strncmp("WRITE_TO_SHM", request_message, 12) == 0){
            read(fd1, &offset, sizeof(unsigned int));
            read(fd1, &value, sizeof(unsigned int));

            if ((offset + 3) < 4700637 && offset >= 0 && offset<= 4700637){
                memcpy(shDATA + offset, &value, 4);
                printf("%d", value);
                size = 12;
                write(fd2, &size, 1);
                write(fd2, &request_message, size);

                size = 7;
                write(fd2, &size, 1);
                write(fd2, &success, size);
            }

            else if ((offset + 3) > 4700637){ 
                size = 12;
                write(fd2, &size, 1);
                write(fd2, &request_message, size);

                size = 5;
                write(fd2, &size, 1);
                write(fd2, &error, size);
            }
        }

        /*2.6 Request pentru map-area unui fisier in memorie*/
        if (strncmp("MAP_FILE", request_message, 8) == 0){
            //read(fd2, &file_size2, 1);
            read(fd1, &file_size1, 1); //citesc 
            file_name = (char*)malloc((file_size1 + 1)*sizeof(char));
            read(fd1, file_name, file_size1); //citesc 
            file_name[(int)file_size1] = '\0';
            printf("\n%d\n", file_size1);
            printf("%d\n", file_size2);
            printf("DISPERRR\n");

            fd3_MAP = open(file_name, O_RDONLY); 
		
            if (fd3_MAP < 0){
                size = 8;
                write(fd2, &size, 1);
                write(fd2, &request_message, size); //map
               
                size = 5;
                write(fd2, &size, 1);
                write(fd2, &error, size); //error
                perror("Could not map file\n");
                continue;
            }

            file_size2 = lseek(fd3_MAP, 0, SEEK_END);
            lseek(fd3_MAP, 0, SEEK_SET);
            printf("\n%d\n", file_size2);
            printf("%d\n", file_size1);
            myDATA = (char *)mmap(NULL, file_size2, PROT_READ, MAP_SHARED, fd3_MAP, 0);
            printf("%s\n", myDATA);
            if (myDATA == (void *)-1){
                size = 8;
                write(fd2, &size, 1);
                write(fd2, &request_message, size); //map

                size = 5;
                write(fd2, &size, 1);
                write(fd2, &error, size); //error
                close(fd3_MAP);
                perror("Could not map\n");
                continue;
            }

            printf("%d\n", file_size2);
            size = 8;
            write(fd2, &size, 1);
            write(fd2, &request_message, size); //map

            size = 7;
            write(fd2, &size, 1);
            write(fd2, &success, size); //success
        }

        /*2.7 Request pentru citirea de la un offset din fisier*/
        if (strncmp("READ_FROM_FILE_OFFSET", request_message, 21) == 0) {
            read(fd1, &no_of_bytes, sizeof(unsigned int));
            read(fd1, &offset, no_of_bytes);
            printf("%d\n", no_of_bytes);
            printf("%d\n", offset);
            lseek(fd1, offset, SEEK_END);
            lseek(fd1, 0, SEEK_CUR);
            if(offset + no_of_bytes < file_size2){
                size = 21;
                write(fd2, &size, 1);
                write(fd2, &request_message, size); 

                size = 5;
                write(fd2, &size, 1);
                write(fd2, &error, size);
                return 0;
            }

            size = 7;
            write(fd2, &size, 1);
            write(fd2, &success, size);
        }


        /*2.8 Request pentru citirea dintr-o sectiune SF*/
        if (strncmp("READ_FROM_FILE_SECTION", request_message, 22) == 0){   
            size = 22;
            write(fd2, &size, 1);
            write(fd2, &request_message, size);

            size = 5;
            write(fd2, &size, 1);
            write(fd2, &error, size);
            return 0;

            size = 7;
            write(fd2, &size, 1);
            write(fd2, &success, size);

        }

        /*2.9 Request pentru citirea de la un offest din spatiul de memorie logic*/
        if (strncmp("READ_FROM_LOGICAL_SPACE_OFFSET", request_message, 30) == 0){
            if (fd_READ == -1){
                perror("ERROR\n");
                return 5;
            }
            valid_SF(fd_READ);
            size = 30;
            write(fd2, &size, 1);
            write(fd2, &request_message, size);
		
            size = 5;
            write(fd2, &size, 1);
            write(fd2, &error, size);
            close(fd_READ);
            return 0;

            size = 7;
            write(fd1, &size, 1);
            write(fd1, &success, size);
        }

        /*2.10 Request exit*/
        if (strncmp("EXIT", request_message, 4) == 0)
            return 0;

    }

    close(fd1);
    close(fd2);
    munmap(myDATA, file_size2);
    shmdt(shDATA);
    shDATA = NULL;
    myDATA = NULL;
    shmdt(myDATA);
    unlink("RESP_PIPE_35355");
    unlink("REQ_PIPE_35355");
    shm_unlink(name);
    unlink(file_name);
    free(file_name);

    return 0;
}
