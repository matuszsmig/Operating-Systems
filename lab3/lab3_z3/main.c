#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

void recuretion(char* dir, char* input){
    char newPath[PATH_MAX];
    char realPath[PATH_MAX];
    struct dirent *thisDir;
    struct stat dirStat;
    DIR *dirr;
    dirr = opendir(dir);

    if (dirr != NULL){
        while ((thisDir = readdir(dirr))) {

            strcpy(newPath, dir);
            strcat(newPath, "/");
            strcat(newPath, thisDir->d_name);

            realpath(newPath,realPath);

            stat(realPath,&dirStat);
            if(S_ISDIR(dirStat.st_mode)){
                if (strcmp(thisDir->d_name, ".") != 0 && strcmp(thisDir->d_name, "..") != 0){
                    int currentProces = fork();
                    if (currentProces == 0){
                        recuretion(realPath, input);
                        exit(EXIT_SUCCESS);
                    }
                }
            } else {
                int toOpen;
                char ch;
                int flag = 1;
                int len = strlen(input);
                toOpen = open(thisDir->d_name, O_RDONLY);
                int itr = 0;
                while(read(toOpen,&ch,1)==1 && itr != len) {
                    if (ch != input[itr]){
                        flag = 0;
                        break;
                    }
                    itr+=1;
                }
                if (flag == 1 && itr == len){
                    printf("%s %d\n", realPath, getpid());
                }
                close(toOpen);
            }
        }
        closedir(dirr);
    }
    perror("No directory");
}

int main(int agrc, char *argv[]) {
    char* dirPath = argv[1];
    char* inuptString = argv[2];

    if (strlen(inuptString) > 255){
        perror("2nd argument is too long");
    }

    recuretion(dirPath, inuptString);

    return 0;
}
