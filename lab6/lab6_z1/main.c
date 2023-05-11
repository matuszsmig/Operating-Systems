#include <stdio.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>

#include "exheader.h"

key_t client_list[MAX_CLIENTS];

struct msgbuf {
    long mtype;
    int client_queue;
    int client_id;
    int receiver_id;
    char message[MAXSIZE];
    struct tm time;
} msgbuf;

int server;

void init(struct msgbuf buffer) {
    int flag = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i] == -1) {
            client_list[i] = buffer.client_queue;
            printf("Initiating client with id %d\n", i);
            buffer.client_id = i;
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        fprintf(stderr, "Client limit reached.");
        buffer.client_id = -1;
    }
    msgsnd(buffer.client_queue, &buffer, MAXSIZE, 0);
}

void stop(struct msgbuf buffer){
    printf("Stopping client with id %d\n", buffer.client_id);
    client_list[buffer.client_id]=-1;
}

void toOne(struct msgbuf buffer){
    if(client_list[buffer.receiver_id]!=-1){
        printf("Message sent to client with id %d\n", buffer.receiver_id);
        msgsnd(client_list[buffer.receiver_id],&buffer,MAXSIZE,0);
    } else {
        printf("Client with this id doesn't exists\n");
    }
}

void toAll(struct msgbuf buffer){
    for(int i=0; i<MAX_CLIENTS;i++){
        if(client_list[i]!=-1){
            printf("Message sent to client with id %d\n", i);
            msgsnd(client_list[i],&buffer,MAXSIZE,0);
        }
    }
}

void list(){
    printf("Active clients: ");
    for(int i=0; i<MAX_CLIENTS;i++){
        if(client_list[i]!=-1){
            printf("%d, ", i);
        }
    }
    printf("\n");
}

void log_message(struct msgbuf buffer){
    FILE* log = fopen("log.txt","a");
    fprintf(log,"Message from id: %d\nTime: %d:%d;%d\n%sType: ",
            buffer.client_id, buffer.time.tm_hour, buffer.time.tm_min, buffer.time.tm_sec, buffer.message);

    switch (buffer.mtype)
    {
        case INIT:
            fprintf(log,"INIT\n");
            break;
        case LIST:
            fprintf(log,"LIST\n");
            break;
        case TOONE:
            fprintf(log,"2ONE\n");
            break;
        case TOALL:
            fprintf(log,"2ALL\n");
            break;
        case STOP:
            fprintf(log,"STOP\n");
            break;
    }
    fclose(log);
}

void server_stop(){
    struct msgbuf buffer;
    buffer.mtype=STOP;
    for(int i=0; i<MAX_CLIENTS;i++){
        if(client_list[i]!=-1){
            printf("Stopping client with id %d\n", i);
            msgsnd(client_list[i],&buffer,MAXSIZE,0);
        }
    }
    msgctl(server,IPC_RMID,NULL);

    exit(0);
}

int main() {
    for(int i=0; i<MAX_CLIENTS;i++){
        client_list[i]=-1;
    }

    struct msgbuf buffer;
    key_t server_key = ftok(getenv("HOME"),'6');
    server = msgget(server_key, IPC_CREAT | 0666);
    signal(SIGINT, server_stop);

    while (1){
        msgrcv(server, &buffer, MAXSIZE, -6, 0);
        switch(buffer.mtype){
            case INIT:
                init(buffer);
                log_message(buffer);
                break;
            case LIST:
                list();
                log_message(buffer);
                break;
            case TOALL:
                toAll(buffer);
                log_message(buffer);
                break;
            case TOONE:
                toOne(buffer);
                log_message(buffer);
                break;
            case STOP:
                stop(buffer);
                log_message(buffer);
                break;
        }
    }

    return 0;
}