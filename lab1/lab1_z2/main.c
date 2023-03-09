#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>
#include <stdlib.h>

#ifdef DYNAMIC
    #include <dlfcn.h>

    struct PointerArray{
        int currentPostion;
        struct Block *pointerArray;
    };

    struct Block{
        int isActive;
        char* file;
    };
#else
    #include "mylib.h"
#endif

int main(int argc, char **argv) {

    #ifdef DYNAMIC
        void *handle = dlopen("./libmylib.so", RTLD_LAZY);

        struct PointerArray (*initPointerArray)(int) = dlsym(handle, "initPointerArray");
        void (*count)(struct PointerArray*, char*) = dlsym(handle, "count");
        char* (*valueOnIndex)(struct PointerArray*, int) = dlsym(handle, "valueOnIndex");
        void* (*freeOnIndex)(struct PointerArray*, int) = dlsym(handle, "freeOnIndex");
        void* (*freeArray)(struct PointerArray*) = dlsym(handle, "freeArray");
    #endif

    int maxSize = 1024;
    char *REPL = calloc(maxSize, sizeof(char));
    char *init_show = calloc(5, sizeof(char));
    char *count_cm = calloc(6, sizeof(char));
    char *delete_index = calloc(13, sizeof(char));
    char *destroy = calloc(8, sizeof(char));
    int len;
    int index;
    struct PointerArray ptrArr;

    struct timespec timespec_start, timespec_end;
    struct tms tms_start, tms_end;

    do {
        fgets(REPL, maxSize, stdin);

        clock_gettime(CLOCK_REALTIME, &timespec_start);
        times(&tms_start);

        strncpy(count_cm, REPL, 6);
        strncpy(init_show, REPL, 5);
        strncpy(delete_index, REPL, 13);
        strncpy(destroy, REPL, 7);

        len = strlen(REPL);
        if(len > 0 && REPL[len-1] == '\n'){
            REPL[len-1] = '\0';
        }

        char *command = calloc(len, sizeof(char));

        if (strcmp(init_show, "init ") == 0){
            strncpy(command, REPL + 5, len - 5);
            index = atoi(command);
            ptrArr = initPointerArray(index);
        }
        if (strcmp(count_cm, "count ") == 0){
            strncpy(command, REPL + 6, len - 6);
            count(&ptrArr, command);
        }
        if (strcmp(init_show, "show ") == 0){
            strncpy(command, REPL + 5, len - 5);
            index = atoi(command);
            printf("%s\n", valueOnIndex(&ptrArr, index));
        }
        if (strcmp(delete_index, "delete index ") == 0){
            strncpy(command, REPL + 13, len - 13);
            index = atoi(command);
            freeOnIndex(&ptrArr, index);
        }
        if (strcmp(destroy, "destroy") == 0){
            freeArray(&ptrArr);
        }

        free(command);

        clock_gettime(CLOCK_REALTIME, &timespec_end);
        times(&tms_end);
        printf("Czasy wykonania:\n");
        printf("Rzeczywisty: %ld nsec\n", timespec_end.tv_nsec - timespec_start.tv_nsec);
        printf("Użytkownik: %ld nsec\n", tms_end.tms_utime - tms_start.tms_utime);
        printf("Systemowy: %ld nsec\n", tms_end.tms_stime - tms_start.tms_stime);

    } while (strcmp(REPL, "exit") != 0);

    free(REPL);
    free(init_show);
    free(count_cm);
    free(delete_index);
    free(destroy);

    #ifdef DYNAMIC
        dlclose(handle);
    #endif
    return 0;
}