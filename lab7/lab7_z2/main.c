#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

void do_haircut(int current_customer){
    int haircut_time = rand()%10 + 1;
    sleep(haircut_time);
    printf("Klient %d zostal obsluzony\n", current_customer);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("I need 3 args!\n");
        return -1;
    }

    sem_t* hairdressers = sem_open("hairdressers", O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, atoi(argv[1]));
    sem_t* seats = sem_open("seats", O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, atoi(argv[2]));
    sem_t* waitingroom = sem_open("waitingroom", O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, atoi(argv[3]));

    int *num_hairdressers = malloc(sizeof(int));
    int *num_seats = malloc(sizeof(int));
    int *num_waitingroom = malloc(sizeof(int));

    int num_client = 1;

    while (1) {
        sleep(2);
        sem_getvalue(hairdressers, num_hairdressers);
        sem_getvalue(seats, num_seats);
        sem_getvalue(waitingroom, num_waitingroom);

        printf("Aktualnie jest wolnych: %d fryzjerow, %d foteli, %d miejsc w poczekalni\n"
               ,*num_hairdressers, *num_seats, *num_waitingroom);
        int pid = fork();
        if (pid == 0) {
            printf("Klient nr %d\n", num_client);

            if (sem_trywait(hairdressers) != -1) {
                printf("Klient %d spotkal fryzjera\n", num_client);

                if (sem_trywait(seats) != -1) {
                    printf("Klient %d dostal fotel\n", num_client);
                    do_haircut(num_client);
                    sem_post(seats);
                    sem_post(hairdressers);
                } else {
                    if (sem_trywait(waitingroom) != -1) {
                        printf("Klient %d trafia do poczekalni i czeka na fotel\n", num_client);
                        sem_wait(seats);

                        printf("Klient %d dostal fotel\n", num_client);
                        do_haircut(num_client);
                        sem_post(waitingroom);
                        sem_post(hairdressers);
                        sem_post(seats);
                    } else {
                        printf("Klient %d musi opuscic lokal bo nie bylo miejsca w poczekalni\n", num_client);
                    }
                }
                break;
            } else {
                printf("Klient %d musi opuscic lokal bo nie bylo fryzjera\n", num_client);
            }
        }
        num_client += 1;
    }
    return 0;
}