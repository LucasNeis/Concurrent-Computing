#include <pthread.h>	
#include <stdio.h>

void *PrintHello(void *arg)	{	
	long int val = (long int) arg;	
	pthread_t Dd = pthread_self();	
	printf("Thread %u: Hello! arg = %li\n", (unsigned int)Dd,	val);	
	pthread_exit(NULL);	
}	
int main (int argc, char *argv)	{	
	int numero;
	printf("Digite o número de threads\n");
	scanf("%u", &numero);
	pthread_t thread[numero];	
	for (int i = 0; i < numero; i++) {	
		pthread_create(&thread[i],	NULL, PrintHello, (void	*)(i + 1));
	}
	for (int i = 0; i < numero; i++) {	
		pthread_join(thread[i], NULL);	
	}
	printf("\nForam executadas %li threads(%li filhos + main)\n\n", numero + 1, numero);
	pthread_exit(NULL);	
}