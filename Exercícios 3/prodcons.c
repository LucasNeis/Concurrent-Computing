#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_THREADS 100
#define MAX_N 100


sem_t cheio;
sem_t vazio;
sem_t lockProd;
sem_t lockCons;
int buffer[MAX_N];
int var_glob = 0;
int i = 0;
int f = 1;

void *produtor() {
	while(1) {
		sem_wait(&vazio);
		sem_wait(&lockProd);
		f = (++f) % MAX_N;
		buffer[f] = produz();
		sem_post(&lockProd);
		sem_post(&cheio);
	}
}

void *consumidor() {
    while(1){
        sem_wait(&cheio);
        sem_wait(&lockCons);
        i = (++i) % MAX_N;
        consome(buffer[i], i);
        sem_post(&lockCons);
        sem_post(&vazio);
    }
}

int produz() {
	var_glob++;
	return var_glob;
}


void consome(int p, int i) {
	printf("O produto é %i na posição %i\n", p, i);
	return;
}

int main(int argc, char **argv) {
        sem_init(&cheio, 0, 0);
        sem_init(&vazio, 0, MAX_N);
        sem_init(&lockProd, 0, 1);
        sem_init(&lockCons, 0, 1);
        int i;
        pthread_t threads[MAX_THREADS];

        printf("Main thread iniciada.\n");

        for(i = 0; i < MAX_THREADS/2; i+2){
            	pthread_create(&threads[i], NULL, produtor, NULL);
                pthread_create(&threads[++i], NULL, consumidor, NULL);
        }

        for(i = 0; i < MAX_THREADS; i++)
                pthread_join(threads[i], NULL);

        sem_destroy(&vazio);
        sem_destroy(&cheio);
        sem_destroy(&lockProd);
        sem_destroy(&lockCons);
        return 0;
}