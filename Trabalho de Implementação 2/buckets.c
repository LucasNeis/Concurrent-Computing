#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

typedef struct {
	int minimo, maximo, tamanho;
	int* conteudo;
} bucket;

bucket* buckets;
int* array;

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

int main(int argc, char** argv) {
	int rank, size, req, pos;
	MPI_Init(&argc, &argv);
	MPI_Status st;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0) {
	if(argc < 3) {
		printf("Erro! Para usar o BucketSort, é necessario enviar, por linha de comando, dois argumentos: tamanho do vetor e número de baldes, necessariamente nesta ordem. Segue exemplo:\n'./BucketSort 1000 55'\nNo exemplo, há 1000 elementos no vetor e 55 buckets.\n");
		return -1;
	} 
	srand((unsigned)time(NULL));
	int tamvet = atoi(argv[1]);
	int n_buckets = atoi(argv[2]);

	if (n_buckets > tamvet || n_buckets < 1) {
		printf("Erro! O número de buckets não pode ser maior do que o número de elementos no vetor ou ser zero. vetor: %i buckets: %i\n" , tamvet, n_buckets);
		return -1;
	}

	array = (int*) malloc(sizeof(int) * tamvet);
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
	//
		int finalizado = 0;
		int buc_contador = 0;
		int tamanho_bucket = 0;
		while(finalizado < n_buckets) {
			MPI_Recv(&req, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &st);
			if(req) {
				tamanho_bucket = buckets[buc_contador].tamanho;
				while(tamanho_bucket < 2 && buc_contador < n_buckets){
					buc_contador++;
					finalizado++;
					tamanho_bucket = buckets[buc_contador].tamanho;
				}
				if(buc_contador < n_buckets){ 
					MPI_Send(&buckets[buc_contador].tamanho, 1, MPI_INT, st.MPI_SOURCE, 1, MPI_COMM_WORLD);
					MPI_Send(buckets[buc_contador].conteudo, buckets[buc_contador].tamanho, MPI_INT, st.MPI_SOURCE, 2, MPI_COMM_WORLD);
					printf("Mestre ENVIOU bucket %i para Escravo %i\n", buc_contador, st.MPI_SOURCE);
					MPI_Send(&buc_contador, 1, MPI_INT, st.MPI_SOURCE, 3, MPI_COMM_WORLD);
					buc_contador++;
				}
			} else {
			 	MPI_Recv(&pos, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &st);
				MPI_Recv(buckets[pos].conteudo, buckets[pos].tamanho, MPI_INT, st.MPI_SOURCE, 5, MPI_COMM_WORLD, &st);
				printf("Mestre RECEBEU bucket %i do Escravo %i\n", pos, st.MPI_SOURCE);
				finalizado++;
			}
		}
		for(int i = 0; i < n_buckets; i++) {
			for(int j = 0; j < buckets[i].tamanho; j++) {
				array[iterator] = buckets[i].conteudo[j];
				printf("%i ", array[iterator]);
				iterator++;
			}
		}
		printf("\n");
		int terminator = -1;
		for(int i = 0; i < size; i++)
			MPI_Send(&terminator, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
	} else {
		int* buc;
		int valor = 1;
		while(valor != -1){
			req = 1;
			MPI_Send(&req, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Recv(&valor, 1 , MPI_INT, 0, 1, MPI_COMM_WORLD, &st);
			if(valor != -1) {
				buc = (int*) malloc(sizeof(int) * valor);
				MPI_Recv(buc, valor, MPI_INT, 0, 2, MPI_COMM_WORLD, &st);
				MPI_Recv(&pos, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &st);
				bubble_sort(buc, valor);
				req = 0;
				MPI_Send(&req, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
				MPI_Send(&pos, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
				MPI_Send(buc, valor, MPI_INT, 0, 5, MPI_COMM_WORLD);
			}
		}
	}
	free (buckets);
	free (array);
	
	MPI_Finalize();
	return 0;
}