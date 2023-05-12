#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

int global = 0;
pthread_mutex_t x_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

int itr = 0;
pthread_mutex_t lock;

void * do_staff(void * arg){
    while(1){
        pthread_mutex_unlock(&x_mutex);
        global++;
        printf("%lu has incremented global to %d\n", pthread_self(), global);
        sleep(1);
        pthread_mutex_lock(&x_mutex);
    }
}

int main(int argc, char **argv) {

    int n = atoi(argv[1]);

    pthread_t* tab = calloc(n, sizeof(pthread_t));

    for (int i = 0; i < n; i++){
        pthread_create(&(tab[i]), NULL, &do_staff, NULL);
    }

    while(1){
    };

    return 0;
}
