#include <stdio.h>
#include <bits/types/sigset_t.h>
#include <signal.h>
#include <string.h>

void s_pending(){
    sigset_t rec_signal;
    sigpending(&rec_signal);
    if (sigismember(&rec_signal, SIGUSR1)){
        printf("e_Signal is pending\n");
    } else {
        printf("e_Signal isnt pending\n");
    }
}

void s_mask(){
    sigset_t rec_signal;
    sigemptyset(&rec_signal);
    sigaddset(&rec_signal, SIGUSR1);
    sigprocmask(SIG_BLOCK, &rec_signal, NULL);
}

int main(int argc, char* argv[]) {

    if (strcmp(argv[1], "ignore") == 0){
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(argv[1], "mask") == 0){
        s_mask();
    }
    else if (strcmp(argv[1], "pending") == 0){
        s_pending();
    }

    raise(SIGUSR1);
    return 0;
}