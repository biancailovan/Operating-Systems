#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void remove_directory(const char* path){

    DIR *dir=NULL;
    char filePath[512];
    struct stat statBuff;
    struct dirent *entry=NULL;

    dir=opendir(path);

    if(dir==NULL){
        perror("Could not open the directory\n");
        return;
    }

    while((entry=readdir(dir))!=NULL){

        if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
            snprintf(filePath,512, "%s/%s", path, entry->d_name);

            if(lstat(filePath, &statBuff)==0){

                if(S_ISDIR(statBuff.st_mode)){
                    remove_directory(filePath);
                    rmdir(filePath);
                }

                else{
                     if(S_ISREG(statBuff.st_mode))
                           unlink(filePath);
                     else
                       
                     if(S_ISLNK(statBuff.st_mode))
                            unlink(filePath);
                }         

            }
        }
    }

    rmdir(path);

   closedir(dir);
}

int main(int argc, char** argv){

   char answer[10];
    int yes, no;

    if (argc != 2) {
        fprintf(stderr, "Usage:\t%s <directory>\n", *argv);
        return 1;
    }

    do {
        printf("\nAre you sure you want to delete the entire directory %s?\n", argv[1]);
        fgets(answer, 5, stdin);

        yes = !strncmp(answer, "yes\n", 4);
        no = !strncmp(answer, "no\n", 3);

        if (no) {
            printf("\nexit...\n");
            return 0;
        } 

        else if (yes) {  
            remove_directory(argv[1]);
        } 

        else {
            printf("\nPlease answer exactly yes or no.");
        }
    } while (!yes && !no);

    return 0;
}
