#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
	int pid, pidpai;
	pid = fork();
	if(pid > 0) 
		printf("Processo pai %d criou %d\n", getpid(), pid);
	else
		printf("Processo filho %d\n", getpid());
	for(int i = 0; i<=2 && pid != 0; i++) {
		pid = fork();
		if(pid > 0) 
			printf("Processo pai %d criou %d\n", getpid(), pid);
		else
			printf("Processo filho %d\n", getpid());
	}
	return 0;
} 