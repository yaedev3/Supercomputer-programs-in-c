#include <stdio.h>
#include <string.h>
#include "mpi.h"

/**
* Segundo programa en c de supercomputo.
* Programa para hacer un "hola mundo" usando la libreria MPI de forma paralela.
* Se compila usando mpicc -o "prog".
* Se ejecuta usando mpirun #procesos "prog".
* rank - Numero de proceso.
* process - Total de procesos.
*/
int main(int argc, char *argv[])
{
	int rank, process;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&process);

	printf("Hola mundo, desde el proceso %d de %d\n",rank , process);
	MPI_Finalize();
	return 0;
}
