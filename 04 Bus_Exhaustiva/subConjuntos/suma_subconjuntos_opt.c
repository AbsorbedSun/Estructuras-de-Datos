/*
Autor: Edgardo Adrián Franco Martínez
Versión: 1.3
Fecha: Abril de 2023
Descripción: Programa que emplea la función SubsetSum() utiliza una técnica de recursión 
para resolver el problema de suma de subconjuntos. La función toma como argumentos el 
conjunto de entrada A, el tamaño del conjunto n y el valor de la suma deseada k.
Se optimiza empleando la idea de guardar las soluciones a los subproblemas que ya se 
calcularon para no repetir calculos recursivos.
*/
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define NC -1

// Definición de un tipo de datos booleano
typedef char booleano;

// Prototipo de la función SubsetSum
booleano SubsetSum(int *A, int n, int k, booleano **M);

int main(int narg, char *arg[]) 
{
	int i, n, *A, k;
	booleano result;
	
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
	
	// Se reserva memoria para la matriz M de tamaño (n+1) x (k+1)
	booleano **M = malloc((n+1) * sizeof(booleano *));
	if(M == NULL)
		exit(1);
	for(i = 0; i <= n; i++)
	{
		M[i] = malloc((k+1) * sizeof(booleano));
		if(M[i] == NULL)
			exit(1);
	}
	
	// Se inicializa la matriz M con el valor NC
	for(i = 0; i <= n; i++)
	{
		for(int j = 0; j <= k; j++)
			M[i][j] = NC;
	}

	// Se llama a la función SubsetSum y se almacena el resultado en la variable result
    result = SubsetSum(A, n, k, M);

    // Se imprime el resultado obtenido
    if (result)
        printf("\nSe puede formar un subconjunto de S que sume %d\n", k);
    else
        printf("\nNo se puede formar un subconjunto de S que sume %d\n", k);

    // Se liberan las matrices y arreglos utilizados
    for(i = 0; i <= n; i++)
		free(M[i]);
	
	free(M);
	free(A);
    return 0;
}

/*
La función SubsetSum determina si es posible formar un subconjunto del arreglo A 
tal que la suma de sus elementos sea igual a k. Además, utiliza programación 
dinámica para mejorar su eficiencia.

Parámetros:
- A: puntero a un arreglo de enteros
- n: tamaño del arreglo A
- k: valor de la suma deseada
- M: matriz booleana utilizada para almacenar los resultados de los subproblemas

Retorno:
- TRUE si es posible formar un subconjunto de A que sume k, FALSE en caso contrario
*/
booleano SubsetSum(int *A, int n, int k, booleano **M) 
{
	booleano r;
	
	// Si el resultado del subproblema (n,k) ya se ha calculado, se devuelve el valor almacenado en la matriz M
	if(M[n][k] != NC)
	{
		r = M[n][k];
	}
	// Si k es cero, es posible formar un subconjunto que sume cero (el conjunto vacío)
	else if (k == 0)
	{
		M[n][k] = TRUE;
		r = M[n][k];
	}
	// Si n es cero y k no es cero, no es posible formar un subconjunto que sume k
	else if (n == 0)
	{
		M[n][k] = FALSE;
		r = M[n][k];
	}
	// Si el último elemento de A es mayor que k, entonces se excluye este elemento del subconjunto
	else if (A[n-1] > k)
	{
		M[n][k] = SubsetSum(A, n-1, k, M);
		r = M[n][k];		
	}
	// Si el último elemento de A es menor o igual que k, se evalúan ambos casos (incluirlo y excluirlo del subconjunto)
	else 
	{
		// Si el resultado de SubsetSum(A,n-1,k) no se ha calculado, se llama a la función y se almacena en la matriz M
		if(M[n-1][k] == NC)
			M[n-1][k] = SubsetSum(A, n-1, k, M);

		// Si el resultado de SubsetSum(A,n-1,k-A[n-1]) no se ha calculado, se llama a la función y se almacena en la matriz M
		if(M[n-1][k-A[n-1]] == NC)
			M[n-1][k-A[n-1]] = SubsetSum(A, n-1, k-A[n-1], M);
		
		// Se calcula el resultado para el subproblema (n,k) utilizando los resultados de los subproblemas (n-1,k) y (n-1,k-A[n-1])
		r = M[n-1][k] || M[n-1][k-A[n-1]];
	}

	// Se imprime el resultado obtenido
	if(r==TRUE)
		printf("\nSS(%d,%d)=TRUE",n,k);
	else if(r==FALSE)
		printf("\nSS(%d,%d)=FALSE",n,k);

	// Se almacena el resultado en la matriz M
	M[n][k] = r;
	return r;
}