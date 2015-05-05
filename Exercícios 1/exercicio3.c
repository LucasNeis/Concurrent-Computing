#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
	int pid = fork();
	if(pid > 0)
		pid = fork();
	if(pid == 0) {
		pid = fork();
		if(pid > 0) {
			fork();
		}
		printf("Processo %d filho de %d\n", getpid(), getppid());
		if(pid > 0) {
			wait(NULL);
			wait(NULL);
		}
	} else {
		wait(NULL);
	}
	return 0;
}
