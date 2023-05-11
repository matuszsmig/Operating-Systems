#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>

typedef struct sembuf sembuf;

void do_haircut(int current_customer){
    int haircut_time = rand()%10;
    sleep(haircut_time);
    printf("Klient %d zostal obsluzony\n", current_customer);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("I need 3 args!\n");
        return -1;
    }

    key_t sem_key = ftok(getenv("HOME"), 6);
    int semafors = semget(sem_key, 3, IPC_CREAT | 0666);

    semctl(semafors, 0, SETVAL, atoi(argv[1]));
    semctl(semafors, 1, SETVAL, atoi(argv[2]));
    semctl(semafors, 2, SETVAL, atoi(argv[3]));

    int num_client = 1;

    sembuf buf[1];

    while (1) {
        sleep(2);
        int hairdressers = semctl(semafors, 0, GETVAL, 0);
        int seats = semctl(semafors, 1, GETVAL, 0);
        int waitingroom = semctl(semafors, 2, GETVAL, 0);

        printf("Aktualnie jest wolnych: %d fryzjerow, %d foteli, %d miejsc w poczekalni\n", hairdressers, seats,
               waitingroom);
        int pid = fork();
        if (pid == 0) {
            printf("Klient nr %d\n", num_client);
            buf[0].sem_op = -1;
            buf[0].sem_num = 0;
            buf[0].sem_flg = IPC_NOWAIT;

            if (semop(semafors, buf, 1) != -1) {
                printf("Klient %d spotkal fryzjera\n", num_client);
                buf[0].sem_op = -1;
                buf[0].sem_num = 1;
                if (semop(semafors, buf, 1) != -1) {
                    printf("Klient %d dostal fotel\n", num_client);
                    do_haircut(num_client);
                    buf[0].sem_op = 1;
                    buf[0].sem_num = 1;
                    semop(semafors, buf, 1);
                    buf[0].sem_op = 1;
                    buf[0].sem_num = 0;
                    semop(semafors, buf, 1);
                } else {
                    buf[0].sem_op = -1;
                    buf[0].sem_num = 2;
                    if (semop(semafors, buf, 1) != -1) {
                        printf("Klient %d trafia do poczekalni i czeka na fotel\n", num_client);
                        buf[0].sem_op = -1;
                        buf[0].sem_num = 1;
                        buf[0].sem_flg = 0;
                        if (semop(semafors, buf, 1) != -1){
                            printf("Klient %d dostal fotel\n", num_client);
                            buf[0].sem_op = 1;
                            buf[0].sem_num = 2;
                            semop(semafors, buf, 1);
                            buf[0].sem_op = 1;
                            buf[0].sem_num = 1;
                            semop(semafors, buf, 1);
                            do_haircut(num_client);
                            buf[0].sem_op = 1;
                            buf[0].sem_num = 0;
                            semop(semafors, buf, 1);
                        }

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