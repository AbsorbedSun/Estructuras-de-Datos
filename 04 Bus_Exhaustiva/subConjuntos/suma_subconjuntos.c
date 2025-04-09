#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

// Definición de un tipo de datos booleano
typedef char booleano;

// Prototipo de función SubsetSum
booleano SubsetSum(int *A, int n, int k);

int main(int narg, char *arg[]) 
{
	int i, n, *A, k;

	// Se verifica que se hayan pasado dos argumentos (además del nombre del programa)
	if(narg != 3)
		exit(1);
	
	// Se obtiene el valor de n y k a partir de los argumentos
	n = atoi(arg[1]);
	k = atoi(arg[2]);
	
    // Se reserva memoria para el arreglo A de tamaño n
    A = malloc(n * sizeof(int));
	
	// Si no se pudo reservar la memoria, se sale del programa
	if(A == NULL)
		exit(1);
	
	// Se leen los valores de A desde la entrada estándar
	for(i = 0; i < n; i++)
		scanf("%d", &A[i]);
	
	// Se llama a la función SubsetSum y se almacena el resultado en la variable result
    booleano result = SubsetSum(A, n, k);

    // Se imprime el resultado obtenido
    if (result)
        printf("\nSe puede formar un subconjunto de S que sume %d\n", k);
    else
        printf("\nNo se puede formar un subconjunto de S que sume %d\n", k);

    // Se libera la memoria utilizada para el arreglo A
	free(A);
    return 0;
}

/*
La función SubsetSum determina si es posible formar un subconjunto del arreglo A 
tal que la suma de sus elementos sea igual a k.

Parámetros:
- A: puntero a un arreglo de enteros
- n: tamaño del arreglo A
- k: valor de la suma deseada

Retorno:
- TRUE si es posible formar un subconjunto de A que sume k, FALSE en caso contrario
*/
booleano SubsetSum(int *A, int n, int k) {
	booleano r;

	// Caso base: si la suma deseada es cero, se puede formar un subconjunto vacío
	if (k == 0)
        r = TRUE;
    // Caso base: si el arreglo es vacío, no se puede formar un subconjunto
    else if (n == 0)
		r = FALSE;
    // Caso en el que el último elemento de A es mayor que k, no se puede incluir en el subconjunto
    else if (A[n-1] > k)
		r = SubsetSum(A, n-1, k);
	// Caso general: se verifica si es posible formar un subconjunto incluyendo o no el último elemento de A
	else
		r = SubsetSum(A, n-1, k) || SubsetSum(A, n-1, k-A[n-1]);

	// Se imprime el resultado obtenido para el subproblema (opcional)
	if(r == TRUE)
		printf("\nSS(%d,%d)=TRUE", n, k);
	else
		printf("\nSS(%d,%d)=FALSE", n, k);
    
    // Se devuelve el resultado obtenido
    return r;
}
