#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int agrc, char *argv[]) {
    int numberOfProces = atoi(argv[1]);

    int runningProceses = 0;
    while (runningProceses!=numberOfProces){
        int currentProces = fork();
        switch(currentProces){
            case -1:
                perror("Nie ma takiego procesu");
                exit(EXIT_FAILURE);
            case 0:
                printf("Jestem pocesem: %d, moim rodzicem jest: %d\n", getpid(), getppid());
                exit(EXIT_SUCCESS);
            default:
                printf("Jestem procesem %d, moim dzieckiem jest %d\n", getpid(), currentProces);
                wait(NULL);
                //exit(EXIT_SUCCESS);
        }
        runningProceses+=1;
    }

    printf("\nIlość procesów: %s\n", argv[1]);

    return 0;
}
