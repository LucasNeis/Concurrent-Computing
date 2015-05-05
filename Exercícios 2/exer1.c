#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

void* PrintName(void* oie) {
	pthread_t id = pthread_self();
	printf("\nNova thread criada. TID = %ld!\n", id);
	pthread_exit(NULL);
}

int main() {
	pthread_t thread;
	pthread_create(&thread,	NULL, PrintName, (void *)10);
	pthread_join(thread, NULL);
	return 0;
}
