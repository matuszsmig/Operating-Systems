#include <stdio.h>
#include <sys/msg.h>

struct msgbuf {
    long mtype;         /* typ komunikatu   */
    char mtext[20];      /* tresc komunikatu */
} msgbuf;

int main(int argc, char* argv[]) {

    key_t klucz = ftok(".", '6');
    if (klucz == -1){
        perror("can't create key");
        return -1;
    }

    int mymsg = msgget(klucz, IPC_CREAT | 0666);
    printf("%i\n", klucz);

    if (mymsg == -1){
        perror("can't create msg");
        return -1;
    }

    struct msgbuf msg1 = {1, "Hello world"};
    struct msgbuf msg2 = {2, "Cala naprzod"};

    int snd = msgsnd(mymsg, &msg1, sizeof(msg1.mtext), IPC_NOWAIT);
    int snd2 = msgsnd(mymsg, &msg2, sizeof(msg2.mtext), IPC_NOWAIT);

    if (snd == -1 || snd2 == -1){
        perror("can't send msg");
        return -1;
    }

    struct msgbuf rcv1;
    struct msgbuf rcv2;

    int rcvd1 = msgrcv(mymsg, &rcv1, sizeof(rcv1.mtext), 0, IPC_NOWAIT);
    //int rcvd2 = msgrcv(mymsg, &rcv2, sizeof(rcv2.mtext), 0,IPC_NOWAIT);

    //if (rcvd1 == -1 || rcvd2 == -1){
    //    perror("can't send recive");
    //    return -1;
    //}

    //printf("%s %s", rcv1.mtext, rcv2.mtext);

    struct msqid_ds stat1;

    int mystat1 = msgctl(mymsg, IPC_STAT, &stat1);

    if (mystat1 == -1){
        perror("can't get stats");
        return -1;
    }

    printf("%d %lu", stat1.msg_lspid, stat1.msg_qnum);

    return 0;
}