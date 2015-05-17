#include <mpi.h>	
#include <stdio.h>

int	main(int argc, char	**argv)	{	
	int	size, rank, valor = 0;
	MPI_Status st;
	MPI_Init(&argc,	&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	
	if(rank == 0) {
 		for(int i = 1; i < size; i++) {
	 		MPI_Send(&rank, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	 		MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
	 		printf ("rank: %i\n", valor);
	 	}

	} else {
	 	int message;
	 	MPI_Recv(&message, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
	 	MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	 }

	MPI_Finalize();		
	return	0;	
}		