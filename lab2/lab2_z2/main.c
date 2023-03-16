#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>

void reverseBlock(char block[], int size){
    char block2[size];
    for (int i = 0; i != size; i++) {
        block2[size - i - 1] = block[i];
    }
    for (int i = 0; i!=size; i++){
        block[i] = block2[i];
    }
}


int main(int argc, char **argv) {
    char* fileToOpen = argv[1];
    char* fileToWrite = argv[2];

    struct timespec timespec_start, timespec_end;
    struct tms tms_start, tms_end;

    clock_gettime(CLOCK_REALTIME, &timespec_start);
    times(&tms_start);

    FILE *fileO=fopen(fileToOpen, "r");
    FILE *fileW = fopen(fileToWrite, "w");
    if(fileO && fileW)
    {
        fseek(fileO, 0, SEEK_END);
        int sizeOfFile = ftell(fileO);
        fseek(fileO, -1, SEEK_CUR);
        char stringTowrite[sizeOfFile];
        for (int i = 0; i != sizeOfFile; i++){
            fread(stringTowrite,1, 1,fileO);
            fwrite(stringTowrite, 1, 1, fileW);
            fseek(fileO, -2, SEEK_CUR);
        }
    }
    fclose(fileO);
    fclose(fileW);
    clock_gettime(CLOCK_REALTIME, &timespec_end);
    times(&tms_end);
    printf("Czasy wykonania:\n");
    printf("Rzeczywisty: %ld nsec\n", timespec_end.tv_nsec - timespec_start.tv_nsec);
    printf("Użytkownik: %ld nsec\n", tms_end.tms_utime - tms_start.tms_utime);
    printf("Systemowy: %ld nsec\n", tms_end.tms_stime - tms_start.tms_stime);


    clock_gettime(CLOCK_REALTIME, &timespec_start);
    times(&tms_start);
    FILE *fileToOpenB = fopen(fileToOpen, "r");
    FILE *fileToWriteB = fopen(fileToWrite, "w");
    int size = 1024;
    char block[size];
    if(fileToOpenB && fileToWriteB)
    {
        fseek(fileToOpenB, 0, SEEK_END);
        int sizeOfFile = ftell(fileToOpenB);
        if (sizeOfFile > size){
            fseek(fileToOpenB, -size, SEEK_CUR);
        }
        while (sizeOfFile > size){
            fread(block,1, size,fileToOpenB);
            reverseBlock(block, size);
            fwrite(block, 1, size, fileToWriteB);
            if (sizeOfFile-size > size){
                fseek(fileToOpenB, -2*size, SEEK_CUR);
            }
            sizeOfFile-=size;
        }
        fseek(fileToOpenB, -size, SEEK_CUR);
        fseek(fileToOpenB, -sizeOfFile, SEEK_CUR);
        fread(block, 1, sizeOfFile,  fileToOpenB);
        reverseBlock((char *) block, sizeOfFile);
        fwrite(block, 1, sizeOfFile, fileToWriteB);
    }
    fclose(fileToOpenB);
    fclose(fileToWriteB);
    clock_gettime(CLOCK_REALTIME, &timespec_end);
    times(&tms_end);
    printf("Czasy wykonania:\n");
    printf("Rzeczywisty: %ld nsec\n", timespec_end.tv_nsec - timespec_start.tv_nsec);
    printf("Użytkownik: %ld nsec\n", tms_end.tms_utime - tms_start.tms_utime);
    printf("Systemowy: %ld nsec\n", tms_end.tms_stime - tms_start.tms_stime);

    char cmd[1024];
    strcpy(cmd, "tac < ");
    strcat(cmd, fileToOpen);
    strcat(cmd, " | rev > plik_wyjściowy.txt");
    system(cmd);
}
