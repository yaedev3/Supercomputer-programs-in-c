#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void FillMatrix(float* array, int size);
void PrintArray(float* array, int size, char name[]);
void Multiply(float* matrixA, float* matrixB, float* matrixC, int size);
float MultiplyArray(float* matrixA, float* matrixB, int size, int x, int y);

/**
* Enriquez Capetillo Gerardo Arturo
* Gutierrez Galan Carlos Manuel
* 15 de marzo del 2018
* Quinto programa en c de supercomputo.
* Programa para multiplicar dos matrices cuadradas e imprimir 
* el resultado en pantalla de forma secuencial.
*/
int main(int argc, char *argv[])
{
	int size = 3;
	float *matrixA, *matrixB, *matrixC;
	srand(time(NULL)); 

	matrixA = (float*)malloc(sizeof(float)*size*size);
	matrixB = (float*)malloc(sizeof(float)*size*size);
	matrixC = (float*)malloc(sizeof(float)*size*size);
	FillMatrix(matrixA,size);
	FillMatrix(matrixB,size);

	PrintArray(matrixA,size,"matrixA");
	PrintArray(matrixB,size,"matrixB");
	Multiply(matrixA, matrixB, matrixC, size);
	PrintArray(matrixC,size,"matrixC");
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
	//*(array + (i * size) + j) = rand() % (11 * (i + 1))*1.12;
}

/**
* Imprime una matriz cuadrada con un mensaje.
* array - Matriz cuadrada.
* size - Tamaño de la matriz.
* name - Mensaje para imprimir.
*/
void PrintArray(float* array, int size, char name[])
{
	int i, j;

	printf("Print Array %s\n", name);
	for(i = 0; i < size ;i++)
	{
		for(j = 0; j < size; j++)
			printf("%.2f\t",*(array+(i * size) + j));
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
void Multiply(float* matrixA, float* matrixB, float* matrixC, int size)
{
	int i, j;

	for(i = 0; i < size ;i++)
		for(j = 0; j < size; j++)
			*(matrixC + (i * size) + j) = MultiplyArray(matrixA, matrixB, size, i, j);
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