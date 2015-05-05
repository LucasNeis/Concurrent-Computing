#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
	fork();
	printf("Novo Processo!\n");
	return 0;
} 