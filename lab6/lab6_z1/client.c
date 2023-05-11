#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/msg.h>
#include <signal.h>

#include "exheader.h"

key_t client_key;
key_t server_key;
int client;
int server;
int client_id;

struct msgbuf {
    long mtype;
    int client_queue;
    int client_id;
    int receiver_id;
    char message[MAXSIZE];
    struct tm time;
} msgbuf;

int init(){
    time_t timestamp = time(NULL);

    struct msgbuf buff;
    buff.mtype = INIT;
    buff.time = *localtime(&timestamp);
    buff.client_queue = client;

    msgsnd(server, &buff, MAXSIZE, 0);
    msgrcv(client, &buff, MAXSIZE,0 ,0);

    if (buff.client_id == -1){
        fprintf(stderr, "Server limit reached\n");
        exit(-1);
    }
    return buff.client_id;
}

void stop(){
    time_t timestamp = time(NULL);
    struct msgbuf buff;
    buff.mtype = STOP;
    buff.time = *localtime(&timestamp);
    buff.client_id = client_id;

    msgsnd(server, &buff, MAXSIZE,0);
    msgctl(client,IPC_RMID,NULL);

    exit(0);
}

void toOne(char* id, char* message){
    time_t timestamp = time(NULL);
    struct msgbuf buff;
    buff.mtype = TOONE;
    buff.time = *localtime(&timestamp);
    buff.client_id = client_id;
    buff.receiver_id = atoi(id);
    strcpy(buff.message,message);

    msgsnd(server, &buff, MAXSIZE,0);
}

void toAll(char* message){
    time_t timestamp = time(NULL);
    struct msgbuf buff;
    buff.mtype = TOALL;
    buff.time = *localtime(&timestamp);
    buff.client_id = client_id;
    strcpy(buff.message,message);

    msgsnd(server, &buff, MAXSIZE,0);
}

void list(){
    time_t timestamp = time(NULL);
    struct msgbuf buff;
    buff.mtype = LIST;
    buff.time = *localtime(&timestamp);
    buff.client_id = client_id;

    msgsnd(server, &buff, MAXSIZE,0);
}

int main(int argc, char *argv[]){
    char repl[255];
    char* arg1;
    char* arg2;

    struct msgbuf reciver;
   server_key = ftok(getenv("HOME"),'6');
   client_key = ftok(getenv("HOME"), '7');

   server = msgget(server_key,0);
   client = msgget(client_key, IPC_CREAT | 0666);

    signal(SIGINT, stop);
    printf("--%d--%d--", client, server);
    client_id = init();
    printf("%d", client_id);

    while(1){
        fgets(repl,255,stdin);
        strtok(repl," ");
        char command[5];
        strncpy(command,repl,4);
        command[4]='\0';

        arg1=strtok(NULL," ");
        arg2=strtok(NULL," ");
        strtok(NULL," ");

        if(strcmp(command,"LIST") == 0){
            list();
        }
        else if(strcmp(command,"2ALL") == 0){
            toAll(arg1);
        }
        else if(strcmp(command,"2ONE") == 0){
            toOne(arg1, arg2);
        }
        else if(strcmp(command,"STOP") == 0){
            stop();
        }

        while(msgrcv(client,&reciver,MAXSIZE,0,IPC_NOWAIT)>0){
            if(reciver.mtype==STOP){
                exit(0);
            }
            printf("Message from id: %d\nTime: %d:%d;%d\n%s\n\n",
                   reciver.client_id, reciver.time.tm_hour,
                   reciver.time.tm_min, reciver.time.tm_sec, reciver.message);
        }
    }

    return 0;
}