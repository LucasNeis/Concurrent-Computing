#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_THREADS 100

int var_compartilhada = 0;

sem_t sem_global;

void *func_thread(void *argumento) {
        int i;
        pthread_t tid = pthread_self();

        for(i = 0; i < 100; i++) {
                sem_wait(&sem_global);
                var_compartilhada++;
                sem_post(&sem_global);
        }
        printf("Worker thread %ld: var_compartilhada = %d.\n", (long)tid, var_compartilhada);
        return 0;
}

int main(int argc, char **argv) {
        sem_init(&sem_global, 0, 1);
        int i;
        pthread_t threads[MAX_THREADS];


        printf("Main thread iniciada.\n");

        for(i = 0; i < MAX_THREADS; i++)
                pthread_create(&threads[i], NULL, func_thread, NULL);

        for(i = 0; i < MAX_THREADS; i++)
                pthread_join(threads[i], NULL);

        printf("Main thread: var_compartilhada = %d.\n", var_compartilhada);

        sem_destroy(&sem_global);

        return 0;
}
