#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

//Identificarea si parsarea fisierelor SF
void parse(int fd)
{
    char magic[2];
    int header_size=0;
    int version=0;
    int no_of_sections=0;
    char sect_name[12];
	int sect_type = 0;
	int sect_offset = 0;
	int sect_size = 0;

    lseek(fd, -3, SEEK_END);
    read(fd, &header_size,2);
    read(fd, magic, 1);
    magic[1]='\0';

    if(strcmp(magic, "C")!=0)
    {
        printf("ERROR\n");
        printf("wrong magic\n");
        return;
    }

    lseek(fd, -header_size, SEEK_CUR);
    read(fd, &version, 2);

    if(version<125 || version>221)
    {
        printf("ERROR\n");
        printf("wrong version\n");
        return;
    }

    read(fd, &no_of_sections, 1);

    if(no_of_sections<7 || no_of_sections>10)
    {
        printf("ERROR\n");
        printf("wrong sect_nr\n");
        return;
    }

    for(int i=0;i<no_of_sections;i++)
    {
        read(fd, sect_name, 11);
        sect_name[11]='\0';
        read(fd, &sect_type, 2);

        if(sect_type!=73 && sect_type !=73 && sect_type!=63 && sect_type!=15)
        {
            printf("ERROR\n");
            printf("wrong sect_types\n");
            return;
        }

        read(fd, &sect_offset, 4);
        read(fd, &sect_size, 4);
    }

    read(fd, &header_size, 2);
    read(fd, magic, 1);
    magic[1]='\0';

    lseek(fd, -header_size, SEEK_CUR);
    lseek(fd, 3, SEEK_CUR);//version=2, magic=1

    printf("SUCCESS\n");
    printf("version=%d\n", version);
	printf("nr_sections=%d\n", no_of_sections);

    for(int i=0;i<no_of_sections;i++)
    {
        read(fd, sect_name, 11);
        sect_name[11]='\0';
        read(fd, &sect_type, 2);
		read(fd, &sect_offset, 4);
		read(fd, &sect_size, 4);
		printf("section%d: %s %d %d\n", i + 1, sect_name, sect_type, sect_size);

    }
 
}

//Filtrarea dupa sectiuni
void findall(const char *path)
{
    char magic[2];
    int header_size=0;
    int version=0;
    int no_of_sections=0;
    char sect_name[12];
	int sect_type = 0;
	int sect_offset = 0;
	int sect_size = 0;

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[1024];
    struct stat statbuf;
    dir = opendir(path);

    if (dir == NULL)
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
        {
            snprintf(fullPath, 1024, "%s/%s", path, entry->d_name);
            //printf("%s\n", fullPath);

            if (lstat(fullPath, &statbuf) == 0)
            {
                if (S_ISREG(statbuf.st_mode))
                {
                    int ok = 0;
                    int fd = -1;
                    fd = open(fullPath, O_RDONLY);

                    if (fd == -1)
                    {
                        printf("ERROR\ninvalid directory path\n");
                    }

                    lseek(fd, -3, SEEK_END);
                    read(fd, &header_size,2);
                    read(fd, magic, 1);
                    magic[1]='\0';

                    if(strcmp(magic,"C")!=0)
                    {
                        return;
                    }

                    lseek(fd, -header_size, SEEK_CUR);
                    read(fd, &version, 2);

                    if(version<125 || version>221)
                    {
                        return;
                    }

                    read(fd, &no_of_sections, 1);

                    if(no_of_sections<7 || no_of_sections>10)
                    {
                        return;
                    }

                    for(int i=0;i<no_of_sections;i++)
                    {
                        read(fd, sect_name, 11);
                        sect_name[11]='\0';
                        read(fd, &sect_type, 2);

                        if(sect_type !=73 && sect_type!=63 && sect_type!=15)
                        {
                            return;
                        }

                        read(fd, &sect_offset, 4);
                        read(fd, &sect_size, 4);

                        if(sect_size>1390)
                        {
                            ok++;
                        }
                    }

                    read(fd, &header_size, 2);
                    read(fd, magic, 1);
                    magic[1]='\0';

                    lseek(fd, -header_size, SEEK_CUR);
                    lseek(fd, 3, SEEK_CUR);/*version=2, magic=1*/

                    //if(ok==1)   

                    if(ok == 0)
                        printf("%s\n", fullPath);
                 
                    close(fd);
                }

                if(S_ISDIR(statbuf.st_mode))
                {
                    //printf("%s\n", fullPath);
                    findall(fullPath);
                }

            }

            else
            {
                printf("ERROR\n");
                printf("invalid directory path\n");
                return;
            }
        }
    }
    closedir(dir);

} 

//Functie pentru lucrul cu sectiuni
void extract(const char *path, int sect_nr, int line_nr)
{
    int fd=-1;
    int header_size=0;
    int sect_offset=0;
    int sect_size=0;
    char magic[2];
    int version=0;
    int no_of_sections=0;
    char sect_name[12];
    int sect_type=0;

    fd=open(path, O_RDONLY);
    if(fd==-1)
    {
        printf("ERROR\n");
        printf("invalid file");
        return;
    }

    lseek(fd, -3, SEEK_END);
    read(fd, &header_size,2);
    read(fd, magic, 1);
    magic[1]='\0';

    lseek(fd, -header_size, SEEK_CUR);
    read(fd, &version, 2);

    read(fd, &no_of_sections, 1);

    read(fd, sect_name, 11);
    sect_name[11]='\0';
    read(fd, &sect_type, 2);

    read(fd, &header_size, 2);
    read(fd, magic, 1);
    magic[1]='\0';

    lseek(fd, -header_size, SEEK_CUR);
    lseek(fd, 3, SEEK_CUR);

    lseek(fd, 0, SEEK_END);
    lseek(fd, -3, SEEK_CUR);
    read(fd, &header_size, 2);
    lseek(fd, -header_size, SEEK_END);
    lseek(fd, 3, SEEK_CUR);

    lseek(fd, ((sect_nr)-1)*21, SEEK_CUR);
    lseek(fd, 13, SEEK_CUR);

    read(fd,&sect_offset,4);
	read(fd,&sect_size,4);

	int number=1,i=0; 
    char character;
	for(int k=1;k<=sect_size;k++)
    {
	    while(read(fd,&character,1))
	    {
		    if(character=='\n')
		    {
			    number++;
                break;
		    }
               
	    }
    }

    //printf("SUCCESS\n");

	lseek(fd,0,SEEK_SET);
	lseek(fd,sect_offset,SEEK_CUR);

    char line[1000000];
	char goodLINE[1000000];
    //char goodL[1000000];
    memset(line, 0, sizeof(line));
    memset(goodLINE, 0, sizeof(goodLINE));
    //line[i]=0;
	while(read(fd,&character,1) && line_nr-1!=number)
	{	
		if(character=='\r'|| character=='\n')
		{
			line[i]=0;
            //line[i]=character;
			memcpy(goodLINE,line, 1000000);
			number--;
            i=0;

		}

        else if(character!='\r' && character!='\n')
		{
			line[i]=character;
			i++;
		}

	}

        printf("SUCCESS\n");
        printf("%s\n", goodLINE);

        //memset(line, 0, sizeof(line));
        //memset(goodLINE, 0, sizeof(goodLINE));

	close(fd);

}

//Functie pentru primul criteriu de filtrare, name_starts_with=string
int startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    return lenstr < lenpre ? 0: memcmp(pre, str, lenpre) == 0;
}

//Functie pentru al doilea criteriu de filtrare, has_perm_write
void listPermissions(const char *path, const char *permissions)
{
    DIR *dir;
    char fullPath[1024];
    struct dirent *entry = NULL;
    struct stat statbuf;
    int valid=0;

    dir = opendir(path);
    if(dir==NULL)
    {
        perror("Could not open the directory");
        return;
    }

    //cel mai din stanga=>2^8, daca e dif de "-"=>am ceva acolo
    /*if{
        for(int i=0;i<9;i++)
            if(permissions[i]!='-')
    }*/

    while((entry=readdir(dir))!=NULL){
        if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
        {
            snprintf(fullPath, 1024, "%s/%s", path, entry->d_name);

            if(lstat(fullPath, &statbuf)==0)
            {
                //nr permisiunii
                if((statbuf.st_mode&0777)==valid)
                {
                    printf("%s\n", fullPath);
                }
            }

        }
    }
}

//Afisarea continutului folderelor
void list(char *path, int recursive, char *name_starts_with, int size, int filter)
{
	DIR *dir = opendir(path);
	struct dirent *entry;
	struct stat statbuf;

	char newPath[1024];
    char fullPath[512];

    //Crearea unui path
	if (path[strlen(path)] != '/')
		strcat(path, "/");

	while ((entry = readdir(dir)) != 0)
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{  
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf)==0){

            if (filter==1 && startsWith(name_starts_with, entry->d_name))
            {
                printf("%s%s\n", path, entry->d_name);
                //printf("%ld\n", strlen(path));
            }

        }

		if (filter==2 || name_starts_with == NULL || strcmp(entry->d_name, name_starts_with)==0)     
		{
			strcpy(newPath, path);
			strcat(newPath, entry->d_name);
			stat(newPath, &statbuf);

			if (size == -1)
				printf("%s\n", newPath);
			//else if (S_ISREG(statbuf.st_mode))
				//printf("%s\n", newPath);

			if (S_ISDIR(statbuf.st_mode) && recursive==1)
            {
				list(newPath, recursive, name_starts_with, size, filter);
            }      

		}
           
		}
	}

	closedir(dir);
}


int main(int argc, char **argv)
{

    char* path;
    int fd;
    struct stat statbuf;
	int recursive = 0;
	int size = -1;
    int j=2;
			
	char *name_starts_with = NULL;
    int filter=0;

    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("72942\n");
        }

        else if (strcmp(argv[1], "parse") == 0){
		
				path = argv[2] + 5;
			
				if ((fd = open(path, O_RDONLY)) >= 0){
					parse(fd);
					close(fd);
				}

				else{
					printf("ERROR\ninvalid file path\n");
				}
		}

        else if (strcmp(argv[1], "findall") == 0){
               path=NULL;
            
               if(strncmp(argv[2], "path=", 5)==0){
                   path=argv[2];
                   path=path+5;
                   printf("SUCCESS\n");
                   findall(path);
               }	
			
		}

        else if(strcmp(argv[1], "extract") ==0){
			path = NULL;
            int j=2;
			int sect_nr=1;
			int line_nr = 1;

			while (argc>j) {
				
				if (strncmp(argv[j], "path=", 5) == 0){

					path = argv[j];
                    path=path+5;
				}

				if (strncmp(argv[j], "section=", 8) == 0){
                    
					sect_nr = atoi(argv[j] + 8);
				}

				if (strncmp(argv[j], "line=", 5) == 0){

					line_nr = atoi(argv[j] + 5);
				}
			
				j++;

			}	

				extract(path,sect_nr,line_nr);
	
		}	


       else if (strcmp(argv[1], "list") == 0) {    
           path = NULL;

			while (argc>j) {
				if (strncmp(argv[j], "path=", 5) == 0){

					path = argv[j];
                    path=path+5;    

				}

				else if (strcmp(argv[j], "recursive") == 0){

					recursive = 1;
				}

                else if (strncmp(argv[j], "name_starts_with=", 17) == 0){

					name_starts_with = argv[j];
                    name_starts_with=name_starts_with+17;
                    filter=1;
				}

                else if(strncmp(argv[j], "has_perm_write=", 15)==0){
                    
                    //path=argv[j]+15;
                    size=atoi(argv[j]+15);    

                }

				j++;
			}
			
			if(stat(path, &statbuf)>=0 && S_ISDIR(statbuf.st_mode)==1){
                
				printf("SUCCESS\n");
				list(path, recursive, name_starts_with, size, filter);
			}

            else {

				printf("ERROR\ninvalid directory path\n");
			}
       }	

    }

    return 0;

}