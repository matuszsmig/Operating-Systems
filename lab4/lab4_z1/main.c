#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <bits/types/sigset_t.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

void s_handler(int sig){
    printf("Signal handler\n");
}

void s_pending(){
    sigset_t rec_signal;
    sigpending(&rec_signal);
    if (sigismember(&rec_signal, SIGUSR1)){
        printf("Signal is pending\n");
    } else {
        printf("Signal isnt pending\n");
    }
}

void s_mask(){
    sigset_t rec_signal;
    sigemptyset(&rec_signal);
    sigaddset(&rec_signal, SIGUSR1);
    sigprocmask(SIG_BLOCK, &rec_signal, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2){
        perror("Too many arguments");
        return -1;
    }

    if (strcmp(argv[1], "ignore") == 0){
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(argv[1], "handler") == 0){
        signal(SIGUSR1, s_handler);
    }
    else if (strcmp(argv[1], "mask") == 0){
        s_mask();
    }
    else if (strcmp(argv[1], "pending") == 0){
        s_mask();
    }
    else{
        perror("Incorrect command");
        exit(-1);
    }

    printf("Parent proces: ");

    raise(SIGUSR1);
    if (strcmp(argv[1], "pending") == 0){
        s_pending();
    }


    int proces = fork();
    if (proces == 0){
        printf("Child proces: ");
        if (strcmp(argv[1], "pending") == 0){
            s_pending();
        }
        else{
            raise(SIGUSR1);
        }

        execl("./exec_file","exec_file", argv[1], NULL);
        return 0;

    }

    wait(NULL);



    return 0;
}
