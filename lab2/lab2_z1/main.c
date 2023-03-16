#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>

int main(int argc, char **argv) {
    char charToFind = *argv[1];
    char charToReplace = *argv[2];
    char* fileToOpen = argv[3];
    char* fileToWrite = argv[4];

    struct timespec timespec_start, timespec_end;
    struct tms tms_start, tms_end;

    clock_gettime(CLOCK_REALTIME, &timespec_start);
    times(&tms_start);

    int toOpen, toWrite;
    char ch;
    toOpen = open(fileToOpen, O_RDONLY);
    toWrite = open(fileToWrite, O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    while(read(toOpen,&ch,1)==1) {
        if (charToFind == ch) {
            write(toWrite, &charToReplace, 1);
        } else {
            write(toWrite, &ch, 1);
        }
    }
    close(toOpen);
    close(toWrite);

    clock_gettime(CLOCK_REALTIME, &timespec_end);
    times(&tms_end);
    printf("Czasy wykonania:\n");
    printf("Rzeczywisty: %ld nsec\n", timespec_end.tv_nsec - timespec_start.tv_nsec);
    printf("Użytkownik: %ld nsec\n", tms_end.tms_utime - tms_start.tms_utime);
    printf("Systemowy: %ld nsec\n", tms_end.tms_stime - tms_start.tms_stime);


    clock_gettime(CLOCK_REALTIME, &timespec_start);
    times(&tms_start);

    FILE *fileO=fopen(fileToOpen, "r");
    FILE *fileW = fopen(fileToWrite, "w");
    if(fileO && fileW)
    {
        fseek(fileO, 0, SEEK_END);
        int sizeOfFile = ftell(fileO);
        fseek(fileO, 0, SEEK_SET);
        char stringTowrite[sizeOfFile];
        fread(stringTowrite,1, sizeOfFile,fileO);
        for (int i = 0; i != sizeOfFile; i++){
            if (stringTowrite[i] == charToFind)
                stringTowrite[i] = charToReplace;
        }
        fwrite(stringTowrite, 1, sizeOfFile, fileW);
    }
    fclose(fileO);
    fclose(fileW);

    clock_gettime(CLOCK_REALTIME, &timespec_end);
    times(&tms_end);
    printf("Czasy wykonania:\n");
    printf("Rzeczywisty: %ld nsec\n", timespec_end.tv_nsec - timespec_start.tv_nsec);
    printf("Użytkownik: %ld nsec\n", tms_end.tms_utime - tms_start.tms_utime);
    printf("Systemowy: %ld nsec\n", tms_end.tms_stime - tms_start.tms_stime);

    return 0;
}