#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>

long long int totalSize = 0;

int count(const char* dirPath, const struct stat* buf, int flag){
    if(flag==FTW_F){
        printf("%s %li\n", dirPath, buf->st_size);
        totalSize += buf->st_size;
    }
    return 0;
}

int main(int argc, char **argv) {
    char* dirPath = argv[1];
    if (ftw(dirPath, count, 2) != 0){
        perror("dont work");
    }
    printf("Razem: %lli\n", totalSize);

    return 0;
}