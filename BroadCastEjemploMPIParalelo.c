#include <stdio.h>
#include <string.h>
#include "mpi.h"

void FillArray(float* array, int size);
void PrintArray(float* array, int size, char name[], int id);

/**
* Enriquez Capetillo Gerardo Arturo
* Sexto programa en c de supercomputo.
* Programa para compartir un arreglo inicializado en el proceso 0
* con todos los procesos usando la funcion "Bcast" e imprimirlos en pantalla.
* rank - Numero de proceso.
* process - Total de procesos.
*/
int main(int argc, char *argv[])
{
	int rank, proccess, size = 10;
	float *arrayA;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&proccess);

	arrayA = (float*)malloc(sizeof(float)*size);
	if(rank == 0)
	{
		FillArray(arrayA, size);
	}	
	
	MPI_Bcast(arrayA, size, MPI_FLOAT, 0, MPI_COMM_WORLD);

	PrintArray(arrayA,size, "Proceso:", rank);



	MPI_Finalize();
	return 0;
}
/**
* Llena un arreglo con valores aleatorios de tipo flotante.
* array - Arreglo de tipo flotante.
* size - Tamaño del arreglo.
*/
void FillArray(float* array, int size)
{
	int i;	
	
	for(i = 0; i < size ;i++)
		*(array + i) = rand() % (11 * (i+1))*1.12;
}

/**
* Imprime un arreglo en pantalla con un mensaje y un indicador.
* array - Arreglo de tipo flotante.
* size - Tamaño del arreglo.
* name - Mensaje para imprimir.
* id - Indicador de tipo entero.
*/
void PrintArray(float* array, int size, char name[], int id)
{
	int i;
	printf("%s %d\n", name, id);
	for(i = 0; i < size ;i++)
		printf("%.2f\n",*(array+i));
}