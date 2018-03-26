#include <stdio.h>
#include <string.h>
#include "mpi.h"

void FillArray(int* array, int size);
void AddArray(int id, int* array, int n, int waste, int proccess, int *sum, int size);
void Init(int *waste, int *n, int size, int *sum, int proccess);
void PrintArray(int* array, int size);

/**
* Enriquez Capetillo Gerardo Arturo
* Tercer programa en c de supercomputo.
* Programa para sumar los elementos de un arreglo de tipo entero
* de forma paralela.
*/
int main(int argc, char *argv[])
{
	int rank, proccess, waste, size = 30, sum, n;
	int* array;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&proccess);
	
	array = (int*)malloc(sizeof(int)*size);
	FillArray(array, size);
	Init(&waste, &n, size, &sum, proccess);	
	AddArray(rank, array, n, waste, proccess, &sum, size);
	free(array);

	
	return 0;
}

/**
* Inicializa el tamaño de informacion que debe repartir el primer proceso
* con todos los demas.
* waste - Residuo para el ultimo proceso.
* n -Tamaño de la informacion que se debe repartir entre los procesos.
* size -Tamaño de la informacion total.
* sum -Variable entera para inicializar en 0.
* process - Numero de procesos.
*/
void Init(int *waste, int *n, int size, int *sum, int proccess)
{
	*waste = size % (proccess - 1);
	*n = (size - *waste) / (proccess - 1);
	*sum = 0;
}

/**
* Llena un arreglo con valores enteros.
* array - Arreglo de tipo entero.
* size - Tamaño del arreglo.
*/
void FillArray(int* array, int size)
{
	int i;
	srand(time(NULL)); 
	for(i = 0; i < size ;i++)
		*(array + i) = rand() % (11 * (i+1));
}

/**
* Imprime un arreglo de tipo entero en pantalla.
* array - Arreglo de enteros.
* size - Tamaño del arreglo.
*/
void PrintArray(int* array, int size)
{
	int i;
	printf("Print Array\n");
	for(i = 0; i < size ;i++)
		printf("%d\n",*(array+i));
}

/**
* El proceso 0 divide el arreglo en varios pedazos de informacion definida por 
* la variable n entonces los envia a los demas procesos y estos suman su parte 
* luego la envian de regreso.
* id - Indicador del proceso actual.
* array - Arreglo de enteros.
* n - Tamaño de la informacion para procesar.
* waste - Residuo de la informacion.
* proccess - Numero de procesos.
* sum - Variable para almacenar el resultado.
* size - Tamaño del arreglo.
*/
void AddArray(int id, int* array, int n, int waste, int proccess, int *sum, int size)
{
	int i, miniSum = 0, aux=0;
	int* miniArray;
	
	if(id == 0)
	{
		printf("proccess:%d\n", proccess);
		printf("id:%d\nwaste:%d\nn:%d\n",id,waste,n);
		for(i = 1; i <= (proccess - 1);i++)
		{
			printf("enviado a:%d\n", i);
			if(i == proccess - 1)
				MPI_Send(array+(i-1) * n, n + waste, MPI_INT, i, 0, MPI_COMM_WORLD);
			else MPI_Send(array+(i-1) * n, n, MPI_INT, i, 0, MPI_COMM_WORLD);
		}			

		for(i = 1; i <= (proccess - 1);i++)
		{
			MPI_Recv(&aux,  1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		
			miniSum += aux;
			printf("valor recibido:%d - id:%d valor actual:%d\n", aux, i,miniSum);
		}
		printf("El resultado es: %d\n", miniSum);
	}
	else if(id == proccess - 1)
	{
		miniArray = (int*)malloc(sizeof(int)*n + waste);
		MPI_Recv(miniArray, n + waste, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(i = 0; i < n + waste; i++)
			miniSum += *(miniArray + i);
		printf("id:%d ", id);
		PrintArray(miniArray,n + waste);
		printf("valor enviado:%d - id:%d\n", miniSum,id);
		MPI_Send(&miniSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		miniArray = (int*)malloc(sizeof(int)*n);
		MPI_Recv(miniArray, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(i = 0; i < n; i++)
			miniSum += *(miniArray + i);
		printf("id:%d ", id);
		PrintArray(miniArray,n);
		printf("valor enviado:%d - id:%d\n", miniSum,id);
		MPI_Send(&miniSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}