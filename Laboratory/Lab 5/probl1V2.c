#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

off_t dirSize(const char * dirPath){

    DIR * dir = NULL;
    struct dirent * entry = NULL;
    struct stat statbuf;
    off_t size = 0;

    dir = opendir(dirPath);

    if(dir==NULL){
        return -1;
    }

    while((entry = readdir(dir)) != NULL ){

        lstat(entry->d_name, &statbuf);

        if(S_ISREG(statbuf.st_mode)){
            size += statbuf.st_size;
        }
          
        if(strcmp( ".", entry->d_name)!=0 && strcmp("..", entry->d_name)!=0 ){
            if(lstat(dirPath, &statbuf) == 0) {
                size+=statbuf.st_size;
                if(S_ISDIR(statbuf.st_mode)) {
                   size+=dirSize(entry->d_name);
                }
            }
        }      

    }

    closedir(dir);
    return size;
}


int main(int argc, char ** argv)
{
    if(argc < 1)
        return 1;

    fprintf(stdout,  "The sum is %ld bytes\n", dirSize(argv[1]));

    return 0;
}
