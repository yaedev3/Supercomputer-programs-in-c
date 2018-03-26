#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

void FillMatrix(float* array, int size);
void PrintArray(float* array, int row, int column, char name[], int id);
void Multiply(float* matrixA, float* matrixB, float* matrixC, int sizeX, int sizeY);
float MultiplyArray(float* matrixA, float* matrixB, int size, int x, int y);
void Init(int *waste, int *n, int size, int *sum, int proccess);
void PrintArray1(float* array, int size);

/**
* Enriquez Capetillo Gerardo Arturo
* Gutierrez Galan Carlos Manuel
* 23 de marzo del 2018
* Septimo programa en c de supercomputo.
* Programa para multiplicar dos matrices cuadradas e imprimir 
* el resultado en pantalla de forma paralela.
* Separa la primera matriz A en renglones mientras que manda 
* la matriz B a todos los procesos.
*/
int main(int argc, char *argv[])
{
	int rank, proccess, size = 5;
	int i, j, k, waste, n, processSize;
	float *matrixA, *matrixB, *matrixC;
	srand(time(NULL)); 
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&proccess);
	
	matrixB = (float*)malloc(sizeof(float)*size*size);
	Init(&waste, &n, size, &i, proccess);

	//printf("--------------------------------------INICIO------------------------------------------------------\n");
	if(rank == 0)
	{
		matrixA = (float*)malloc(sizeof(float)*size*size);
		FillMatrix(matrixA,size);
		FillMatrix(matrixB,size);
		PrintArray(matrixA,size,size,"matrixA",rank);
		PrintArray(matrixB,size,size,"matrixB",rank);
	}	
	
	MPI_Bcast(matrixB, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
		matrixC = (float*)malloc(sizeof(float)*size*size);
		processSize = n;
		printf("id:%d - waste:%d - n:%d\n",rank, waste, n, processSize);
		for(i = 1; i <= (proccess - 1);i++)
		{
			if(i == proccess - 1)
				processSize = waste + n;
			//printf("Enviado matrixA al proceso %d\n", i);
			// Error por aquí
			//PrintArray1(matrixA + (( i - 1 ) * (size*n)), processSize*size);
			//PrintArray(matrixA + (( i - 1 ) * (size*n)),processSize,size,"algo",i);
			MPI_Send(matrixA + (( i - 1 ) * (size * n)), processSize*size, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		}
		processSize = n;
		//printf("Empezando a recibir la matrixC\n");
		for(i = 1; i <= (proccess - 1);i++)
		{
			if(i == proccess - 1)
				processSize = waste + n;
			//printf("Recibiendo la matrixC - id:%d\n",rank);
			MPI_Recv(matrixC + (( i - 1 ) * size*n), processSize*size, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		PrintArray(matrixA,size,size,"matrixA",rank);
		PrintArray(matrixB,size,size,"matrixB",rank);
		PrintArray(matrixC,size,size,"matrixC",rank);
//		PrintArray1(matrixC, size*size);
	}
	else
	{
		if(rank == proccess - 1)
			processSize = waste + n;
		else processSize = n;
		printf("id:%d - waste:%d - n:%d - processSize:%d\n",rank, waste, n, processSize);
		matrixA = (float*)malloc(sizeof(float)*processSize*size);
		matrixC = (float*)malloc(sizeof(float)*processSize*size);
		MPI_Recv(matrixA, processSize*size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//printf("Terminado de recibir la matrixA - id:%d\n",rank);
		
		Multiply(matrixA,matrixB,matrixC, processSize, size);
		
		//printf("Terminado de Multiply - id:%d\n",rank);
//		PrintArray(matrixA,processSize,size,"matrixA recibida",rank);
//		PrintArray(matrixB,size,size,"matrixB recibida",rank);
//		PrintArray(matrixC,processSize,size,"matrixC resultado",rank);
//		PrintArray1(matrixC, processSize*size);
		MPI_Send(matrixC, processSize*size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
		//printf("Terminado de enivar la matrixC - id:%d\n",rank);
	}


	MPI_Finalize();
	return 0;
}

/**
* Llena una matriz cuadrada con valores aleatorios.
* array - Matriz cuadrada.
* size - Tamaño de la matriz.
*/
void FillMatrix(float* array, int size)
{
	int i, j;	
	
	for(i = 0; i < size ;i++)
		for(j = 0; j < size; j++)
		*(array + (i * size) + j) = rand() % (11 * (i + 1))*1.12;
}

/**
* Imprime una matriz con un mensaje y un identificador.
* array - Matriz.
* row - Numero de renglones de la matriz.
* column - Numero de columnas de la matriz.
* name - Mensaje para imprimir.
* id - Identificador del proceso.
*/
void PrintArray(float* array, int row, int column, char name[], int id)
{
	int i, j;

	printf("%s - id:%d\n", name, id);
	
	for(i = 0; i < row ;i++)
	{
		for(j = 0; j < column; j++)
			printf("%.2f\t",*(array+(i * column) + j));
		printf("\n");
	}
}

/**
* Multiplica dos matrices cuadradas A, B y almacena el resultado en una tercera matriz C.
* matrixA - Primera matriz cuadrada.
* matrixB - Segunda matriz cuadrada.
* matrixC - Matriz resultado.
* size - Tamaño de las matrices.
*/
void Multiply(float* matrixA, float* matrixB, float* matrixC, int sizeX, int sizeY)
{
	int i, j;

	for(i = 0; i < sizeX ;i++)
		for(j = 0; j < sizeY; j++)
			*(matrixC + (i * sizeX) + j) = MultiplyArray(matrixA, matrixB, sizeY, i, j);
}

/**
* Multiplica el renglon x de la matriz A por la columna y de la matriz B 
* y almacena el resultado una variable de regreso.
* matrixA - Primera matriz.
* matrixB - Segunda matriz.
* size - Tamaño de los arreglos.
* x - Renglon seleccionado de la matriz A. 
* y - Columna seleccionada de la matriz B.
* Forma de acceder a las posiciones de la matriz: matriz + renglon * tamaño + columna
*/
float MultiplyArray(float* matrixA, float* matrixB, int size, int x, int y)
{
	int i;
	float r = 0.0;

	for(i = 0; i < size ;i++)
		r += *(matrixA + (x * size) + i) * *(matrixB + (i * size) + y);

	return r;
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
* Imprime una matriz cuadrada de forma lineal.
* array - Matriz cuadrada.
* size - Tamaño de la matriz.
*/
void PrintArray1(float* array, int size)
{
	int i;
	printf("Print Array\n");
	for(i = 0; i < size ;i++)
		printf("%.2f\n",*(array+i));
}