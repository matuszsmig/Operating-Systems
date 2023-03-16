#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char* dirPath = argv[1];
    struct dirent *thisDir;
    struct stat dirStat;
    long long int totalSize = 0;
    DIR *dirr = opendir(dirPath);

    while ((thisDir = readdir(dirr))) {
        stat(thisDir->d_name,&dirStat);
        if(!S_ISDIR(dirStat.st_mode)){
            printf ("%s %li\n", thisDir->d_name, dirStat.st_size);
            totalSize += dirStat.st_size;
        }
    }
    closedir(dirr);
    printf("Razem: %lli\n", totalSize);

    system("wc --bytes *");


    return 0;
}
