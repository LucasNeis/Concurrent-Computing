#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_THREAD 21474

long long int global = 0;
pthread_mutex_t mutexzoeira;

void* PrintName(void* oie) {
	pthread_t id = pthread_self();
//	printf("\nNova thread criada. TID = %ld\n", (unsigned long int)id);
	for(int i = 0; i < 100; i++) {
	pthread_mutex_lock(&mutexzoeira);
		global++;
	pthread_mutex_unlock(&mutexzoeira);
	}
	pthread_exit(NULL);
}

int main() {
	pthread_mutex_init(&mutexzoeira, NULL);
	pthread_t thread[MAX_THREAD];
	for(int i = 0; i < MAX_THREAD; i++) {
		pthread_create(&thread[i],	NULL, PrintName, (void *)10);
	}
	for(int i = 0; i < MAX_THREAD; i++) {
		pthread_join(thread[i], NULL);
	}
//	printf("global = %lli\n", global);
	pthread_mutex_destroy(&mutexzoeira);
	return 0;
}