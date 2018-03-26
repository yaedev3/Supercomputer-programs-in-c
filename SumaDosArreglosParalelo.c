#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <time.h>

void FillArray(float* array, int size);
void Init(int *waste, int *n, int size, int *sum, int proccess);
void PrintArray(float* array, int size, char name[]);

/**
* Enriquez Capetillo Gerardo Arturo
* Cuarto programa en c de supercomputo.
* Programa para sumar los elementos de dos arreglos de tipo 
* flotanteA, B y almacenarlos en un tercer arreglo C.
* El proceso 0 divide los arreglos en varios pedazos de informacion definida por 
* la variable n entonces los envia a los demas procesos y estos suman su parte 
* luego la envian de regreso.
*/
int main(int argc, char *argv[])
{
	int i, j, k, waste, n, processSize;
	int rank, proccess, size = 25;
	float *arrayA, *arrayB, *arrayC;
	float *auxArrayA, *auxArrayB, *auxArrayC;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&proccess);	
	arrayA = (float*)malloc(sizeof(float)*size);
	arrayB = (float*)malloc(sizeof(float)*size);
	arrayC = (float*)malloc(sizeof(float)*size);
	srand(time(NULL)); 
	FillArray(arrayA, size);
	FillArray(arrayB, size);
	Init(&waste, &n, size, &i, proccess);

	if(rank == 0)
	{
		processSize = n;
		printf("id:%d - waste:%d - n:%d\n",rank, waste, n, processSize);
		for(i = 1; i <= (proccess - 1);i++)
		{
			if(i == proccess - 1)
				processSize = waste + n;
			MPI_Send(arrayA + (i - 1) * n, processSize, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
//			printf("Enviado Arreglo A al proceso %d\n", i);
			MPI_Send(arrayB + (i - 1) * n, processSize, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
//			printf("Enviado Arreglo B al proceso %d\n", i);
		}
		processSize = n;
		printf("Empezando a recibir el arregloC\n");
		for(i = 1; i <= (proccess - 1);i++)
		{
			if(i == proccess - 1)
				processSize = waste + n;
			auxArrayC = (float*)malloc(sizeof(float)*processSize);
			MPI_Recv(auxArrayC, processSize, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(k = 0, j = (i - 1) * n; j < (i - 1) * n + (processSize );j++, k++)
			{
				*(arrayC + j) = *(auxArrayC + k);
				//printf("arrayC:%.2f - k:%d - j:%d\n", *(arrayC + j), k, j);
			}
		}
		printf("Terminado de recibir el arreglo C\n");
		PrintArray(arrayA,size, "A");
		PrintArray(arrayB,size, "B");
		PrintArray(arrayC,size, "C");
	}
	else 
	{
		if(rank == proccess - 1)
			processSize = waste + n;
		else processSize = n;
		printf("id:%d - waste:%d - n:%d - processSize:%d\n",rank, waste, n, processSize);
		auxArrayA = (float*)malloc(sizeof(float)*processSize);
		auxArrayB = (float*)malloc(sizeof(float)*processSize);
		auxArrayC = (float*)malloc(sizeof(float)*processSize);
		MPI_Recv(auxArrayA, processSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//		printf("Terminado de recibir el arreglo A - id:%d\n",rank);
		MPI_Recv(auxArrayB, processSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//		printf("Terminado de recibir el arreglo B - id:%d\n",rank);
		for(i = 0; i < processSize; i++)
		{
			*(auxArrayC + i) = *(auxArrayA + i) + *(auxArrayB + i);
			//printf("auxArrayA:%.2f + auxArrayB:%.2f = auxArrayC:%.2f - id:%d\n", *(auxArrayA + i), *(auxArrayB + i), *(auxArrayC + i), rank);
		}
		MPI_Send(auxArrayC, processSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
	}
	free(arrayA);
	free(arrayB);
	free(arrayC);
	free(auxArrayA);
	free(auxArrayB);
	free(auxArrayC);
	MPI_Finalize();
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
* Llena un arreglo con valores flotantes.
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
* Imprime un arreglo de tipo flotante en pantalla con un mensaje.
* array - Arreglo de flotantes.
* size - Tamaño del arreglo.
* name - Mensaje para imprimir.
*/
void PrintArray(float* array, int size, char name[])
{
	int i;
	printf("Print Array %s\n", name);
	for(i = 0; i < size ;i++)
		printf("%.2f\n",*(array+i));
}