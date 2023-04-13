#include <stdio.h>
#include <signal.h>

void handler_siginfo(int signum, siginfo_t *siginfo, void *context) {
    printf("Otrzymano sygnał o numerze: %d\n", siginfo->si_signo);
    printf("Otrzymano sygnał o PID: %d\n", siginfo->si_pid);
    printf("Otrzymano sygnał o UID: %d\n", siginfo->si_uid);
    printf("Otrzymano sygnał o kodzie błędu: %d\n", siginfo->si_errno);
    printf("Otrzymano sygnał o kodzie sygnału: %d\n", siginfo->si_code);
}

void handler_resethand(int signum, siginfo_t *siginfo, void *context) {
    printf("Otrzymano sygnał o numerze: %d\n", siginfo->si_signo);
    printf("Otrzymano sygnał o PID: %d\n", siginfo->si_pid);
    printf("Otrzymano sygnał o UID: %d\n", siginfo->si_uid);
    printf("Otrzymano sygnał o kodzie błędu: %d\n", siginfo->si_errno);
    printf("Otrzymano sygnał o kodzie sygnału: %d\n", siginfo->si_code);
}

int recurention_level = 0;

void handler_nodefer(int signum, siginfo_t *siginfo, void *context) {
    recurention_level++;
    printf("Current call depth: %d\n", recurention_level);
    if (recurention_level<10){
        raise(SIGUSR1);
    }
}

int main( )
{
    printf("Obsługa flagi siginfo: \n");
    struct sigaction siginfo;
    sigemptyset(&siginfo.sa_mask);
    siginfo.sa_flags = SA_SIGINFO;
    siginfo.sa_sigaction = handler_siginfo;
    sigaction(SIGUSR1, &siginfo, NULL);
    raise(SIGUSR1);

    printf("\nObsługa flagi nodefer: \n");
    struct sigaction nodefer;
    sigemptyset(&nodefer.sa_mask);
    nodefer.sa_flags = SA_NODEFER;
    nodefer.sa_sigaction = handler_nodefer;
    sigaction(SIGUSR1, &nodefer, NULL);
    raise(SIGUSR1);

    printf("\nObsługa flagi resethand: \n");
    struct sigaction resethand;
    sigemptyset(&siginfo.sa_mask);
    resethand.sa_flags = SA_RESETHAND;
    resethand.sa_sigaction = handler_resethand;
    sigaction(SIGUSR1, &resethand, NULL);
    printf("Oczekiwanie na pierwszy sygnał: \n");
    raise(SIGUSR1);
    printf("Oczekiwanie na drugi sygnał: \n");
    raise(SIGUSR1);

    return 0;
}
