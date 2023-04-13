#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc != 3){
        perror("There aren't 3 arguments!");
        exit(EXIT_FAILURE);
    }

    long double width;
    int num_of_proc;
    sscanf(argv[1], "%Lf", &width);
    sscanf(argv[2], "%i", &num_of_proc);
    long double num_of_rectangle = 1.0 / width / num_of_proc;

    if (num_of_rectangle <= num_of_proc || width > 1.0){
        perror("Can't solve for this arguments");
        exit(EXIT_FAILURE);
    }

    char *fifo = "fifo_file";
    mode_t fifo_mode = 0666;

    if (mkfifo(fifo, fifo_mode) == -1) {
        perror("Error creating named pipe");
        exit(EXIT_FAILURE);
    }

    FILE* fifo_open = fopen(fifo, "a+");

    if(fifo_open == NULL){
        perror("can't open file");
        return -1;
    }

    struct timespec timespec_start, timespec_end;
    struct tms tms_start, tms_end;

    clock_gettime(CLOCK_REALTIME, &timespec_start);
    times(&tms_start);

    long double result = 0.0;

    char arg1[20], arg2[20], arg3[20], arg4[20];
    sprintf(arg1, "%Lf", width);
    sprintf(arg2, "%Lf", num_of_rectangle);
    sprintf(arg4, "%s", fifo);

    for (int i = 0; i!= num_of_proc; i++){
        sprintf(arg3, "%d", i);
        pid_t pid = fork();
        if (pid == 0) {
            execl("./exec_f", "exec_f", arg1, arg2, arg3, arg4, NULL);
        } else if (pid == -1){
            perror("CANNOT CREATE CHILD PROCESS!\n");
            exit(EXIT_FAILURE);
        } else { // rodzic
            long double child_result;
            fread(&child_result, sizeof(child_result), 1, fifo_open);
            result += child_result;
        }
    }

    fclose(fifo_open);
    unlink(fifo);

    clock_gettime(CLOCK_REALTIME, &timespec_end);
    times(&tms_end);
    printf("Wynik: %Lf Długość: %Lf, Ilość procesów: %i, Czas: %ld s\n",
           result, width, num_of_proc, timespec_end.tv_sec - timespec_start.tv_sec);
    return 0;
}