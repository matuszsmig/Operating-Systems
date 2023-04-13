#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

int main(int argc, char* argv[]) {
    if (argc != 3){
        perror("There aren't 3 arguments!");
        exit(EXIT_FAILURE);
    }

    long double width;
    int num_of_proc;
    sscanf(argv[1], "%Lf", &width);
    sscanf(argv[2], "%i", &num_of_proc);
    double num_of_rectangle = 1.0 / width / num_of_proc;

    if (num_of_rectangle <= num_of_proc || width > 1.0){
        perror("Can't solve for this arguments");
        exit(EXIT_FAILURE);
    }

    struct timespec timespec_start, timespec_end;
    struct tms tms_start, tms_end;

    clock_gettime(CLOCK_REALTIME, &timespec_start);
    times(&tms_start);

    long double rectangle_area = 0.0;
    long double result = 0.0;

    for (int i = 0; i!= num_of_proc; i++){
        rectangle_area = 0.0;

        int fd[2];
        pipe(fd);
        pid_t pid = fork();

        if (pid == 0) { // dziecko
            close(fd[0]);
            long double current_x = i * num_of_rectangle * width;
            rectangle_area = 0.0;
            for (int j = 0; j != num_of_rectangle; j++){
                current_x += width;
                rectangle_area += 4.0/(current_x * current_x + 1) * width;
            }
            write(fd[1], &rectangle_area, sizeof(long double));
            exit(EXIT_SUCCESS);
        } else if (pid == -1){
            perror("CANNOT CREATE CHILD PROCESS!\n");
            exit(EXIT_FAILURE);
        } else { // rodzic
            close(fd[1]);
            long double child_result;
            read(fd[0], &child_result, sizeof(long double));
            result += child_result;
        }
    }

    clock_gettime(CLOCK_REALTIME, &timespec_end);
    times(&tms_end);
    printf("Wynik: %Lf Długość: %Lf, Ilość procesów: %i, Czas: %ld s\n",
           result, width, num_of_proc, timespec_end.tv_sec - timespec_start.tv_sec);
    return 0;
}
