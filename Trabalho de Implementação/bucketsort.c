#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
	int minimo, maximo, tamanho;
	int* conteudo;
} bucket;

typedef struct {
	int buckets; 
	int nome;
} argumentos;

int u_verificado = -1;
bucket* buckets;
int* array;
int terminou = 0;
sem_t watcher;

void bubble_sort (int *v, int tam) {
	int temp;
	int trocou = 1;
	for (int j = 0; j < tam - 1 && trocou; j ++) {
		trocou = 0;
		for (int i = 0; i < tam - 1; i ++) {
			if(v[i + 1] < v[i]) {
				temp = v[i];
				v[i] = v[i + 1];
				v[i + 1] = temp ;
				trocou = 1;
 			}
 		}
	}
}

void* BucketSort(void* argumento) {
	argumentos* args = (argumentos*) argumento;
	int numero_b = args->buckets;
	int id = args->nome;
	bucket buc;
	while(u_verificado < numero_b-1 && !terminou) {
		sem_wait(&watcher);
		u_verificado++;
		if(!terminou) {
			buc = buckets[u_verificado];

			if(buc.tamanho)
				printf("Thread %i processando bucket %i\n", id, u_verificado);
		}
		sem_post(&watcher);
		if(buc.tamanho && !terminou)
			bubble_sort(buc.conteudo, buc.tamanho);
	}
	terminou = 1;
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	if(argc < 4) {
		printf("Erro! Para usar o BucketSort, é necessario enviar, por linha de comando, três argumentos: tamanho do vetor, número de baldes e número de threads que resolverão o ordenamento, necessariamente nesta ordem. Segue exemplo:\n'./BucketSort 1000 55 40'\nNo exemplo, há 1000 elementos no vetor, 55 buckets e 40 threads.\n");
		return -1;
	} 
	srand((unsigned)time(NULL));
	int tamvet = atoi(argv[1]);
	int n_buckets = atoi(argv[2]);

	if (n_buckets > tamvet || n_buckets < 1) {
		printf("Erro! O número de buckets não pode ser maior do que o número de elementos no vetor ou ser zero. vetor: %i buckets: %i\n" , tamvet, n_buckets);
		return -1;
	}

	int n_thread = atoi(argv[3]);
	array = (int*) malloc(sizeof(int) * tamvet);
	int* bucs = (int*) malloc(sizeof(int) * n_buckets);
	for(int i = 0; i < tamvet; i++) {
		array[i] = rand() % tamvet;
		printf("%i ", array[i]);
	}
	printf("\n");

	int intervalo =  tamvet / n_buckets; 
	int resto = tamvet % n_buckets;
	buckets = (bucket*) malloc(sizeof(bucket) * n_buckets);
	int ultimo_maior = -1;
	int iterator = 0;
	for (int i = 0; i < n_buckets; i++) { 
		buckets[i].conteudo = (int*) malloc(sizeof(int) * tamvet);
		bucs[i] = i;

		if(resto) {
			buckets[i].minimo = ultimo_maior + 1;
			buckets[i].maximo = ultimo_maior + intervalo + 1;
			ultimo_maior = buckets[i].maximo;
			resto--;
		} else {
			buckets[i].minimo = ultimo_maior + 1;
			buckets[i].maximo = ultimo_maior + intervalo;
			ultimo_maior = buckets[i].maximo;
		}

		for(int j = 0; j < tamvet; j++) {
			if(array[j] <= buckets[i].maximo && array[j] >= buckets[i].minimo) {
				buckets[i].conteudo[iterator] = array[j];
				iterator++;
			}
		}

		buckets[i].tamanho = iterator;
		iterator = 0;
	}

	argumentos args;
	args.buckets = n_buckets;
	sem_init(&watcher, 0, 1);
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * n_thread);
	for(int i = 0; i < n_thread; i++) {
		args.nome = bucs[i];
		pthread_create(&threads[i],	NULL, BucketSort, &args);
	}

	for(int i = 0; i < n_thread; i++) {
		pthread_join(threads[i], NULL);
	}

	for(int i = 0; i < n_buckets; i++) {
		for(int j = 0; j < buckets[i].tamanho; j++) {
			array[iterator] = buckets[i].conteudo[j];
			printf("%i ", array[iterator]);
			iterator++;
		}
	}

	printf("\n");
	sem_destroy(&watcher);
	free (threads);
	free (buckets);
	free (array);
	return 0;
}

