#include "stdio.h"
#include "stdlib.h"

void FillArray(int* array, int size);

/**
* Enriquez Capetillo Gerardo Arturo
* Primer programa en c de supercomputo.
* Programa para sumar los elementos de un arreglo de tipo entero 
* de forma secuencial.
*/
void main()
{
	int size, i, sum;
	int* array;
	
	sum = 0;
	size = 30;
	array = (int*)malloc(sizeof(int)*size);

	FillArray(array,size);
	for(i = 0;i < size;i++)
	{
		sum += array[i];
		printf("%d\n",array[i]);
	}	
	printf("La suma es: %d\n",sum);

}
/**
* Llena un arreglo con valores enteros.
* array - Arreglo de tipo entero.
* size - Tamaño del arreglo.
*/
void FillArray(int* array, int size)
{
	int i;
	for(i = 0; i< size;i++)
		*(array+i) = i;
}

