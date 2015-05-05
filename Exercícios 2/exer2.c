#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_THREAD 5

void* PrintName(void* oie) {
	pthread_t id = pthread_self();
	printf("\nNova thread criada. TID = %ld\n", (unsigned long int)id);
	pthread_exit(NULL);
}

int main() {
	pthread_t thread[MAX_THREAD];
	for(int i = 0; i < MAX_THREAD; i++) {
		pthread_create(&thread[i],	NULL, PrintName, (void *)10);
	}
	for(int i = 0; i < MAX_THREAD; i++) {
		pthread_join(thread[i], NULL);
	}
	return 0;
}
