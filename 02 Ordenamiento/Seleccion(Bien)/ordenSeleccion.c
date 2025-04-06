/*  
  Autor: Aldo Garcia Ambrosio (C) Marzo 2025
  Autor origina: Edgardo Adrián Franco Martínez
  Uso de IA (GPT o4, Claude)
  Version: 1.2
  
  Programa que lee n caracteres de la entrada estándar y los ordena con el algoritmo de ordenamiento Seleccion.
  
  Complejidad: O(n^2).
  
  Observaciones: Se tomara la estructura Main de ordenBurbuja.c para realizar agilizar la tarea y realizar el ordenamiento por seleccion.
  
  Compilación: Windows / Linux: gcc ordenSeleccion.c -o seleccion

  Ejecución: ./seleccion {elementos a ordenar} < lista_caracteres.txt
            ./seleccion {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
void ordenSeleccion(int *arregloDes, int n)
Recibe: int * arreglo (puntero) como arregloDes y n como tamaño del arreglo
Devuelve: void (No retorna valor explicito)
Observaciones: Función que ordena el arregloDes de menor a mayor haciendo uso del algoritmo de ordenamiento 
Seleccion (busca el mínimo en la parte desordenada y lo intercambia con el primer elemento de esa parte ademas de dividir en parte ordenada y no ordenada).
*/
void ordenSeleccion(int *arregloDes, int n) {
    // Iterar sobre el arreglo partiendo del segundo elemento¿?
    for(int k = 0; k <= (n - 2); k++){
        // Indice del menos valor en la parte ordenada, inicia tomando el primer elemento de la parte desordenada
        int posMin = k;
        // Iteracion para encontrar el minimo en la parte desordenada
        for(int i = k + 1; i <= (n - 1); i++){
            if(arregloDes[i] < arregloDes[posMin]){
                // Asignar el nuevo valor minimo
                posMin = i;
            }
        }
        // Intercambiar el valor minimo con el primer elemento de la parte desordenada
        int temp = arregloDes[posMin]; // Asigna el valor minimo a una variable temporal
        arregloDes[posMin] = arregloDes[k]; // Reemplaza el menor valor encontrado por el primer elemento de la parte desordenada
        arregloDes[k] = temp; // Coloca el menor valor en la posición k (moviendolo a la parte ordenada)
    }
}

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento seleccion
para ordenar un arreglo de n elementos, ademas de imprimir el resultado en lista.
*/
int main(int num_arg, char *arg_user[]) {

    //Recibir por argumento el tamaño de n y el valor / intervalo de valores a usar
	if (num_arg != 2) 
	{
		printf("\nIndique el tamanio de n y el valor / intervalo de valores a usar - Ejemplo: [cantidad] < {numeros.txt}\n",arg_user[0]);
		exit(1);
	} 

    // Variable que define el numero de elementos a leer y ordenar
    int n = atoi(arg_user[1]);
    
    // Apartar memoria para n números enteros
    int *arreglo = malloc(n * sizeof(int));
    
    // Valir que se haya apartado la memoria correctamente
    if (arreglo == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    
    //Lee de la entrada estandar los n valores y los coloca en un arreglo
    for (int i = 0; i < n; i++) {
        scanf("%d", &arreglo[i]);
    }
    
    // Mostrar el arreglo antes del ordenamiento
    printf("Arreglo antes del ordenamiento: \n");
    for (int i = 0; i < n; i++) {
        printf("%d \n", arreglo[i]);
    }
    printf("\n");

    //*****************************************  
	// Algoritmo de Ordenamiento Seleccion
	//*****************************************
    // Llamar a la función de ordenamiento
    ordenSeleccion(arreglo, n);
    
    // Mostrar el arreglo ordenado
    printf("Arreglo despues del ordenamiento: \n");
    for (int i = 0; i < n; i++) {
        printf("%d \n", arreglo[i]);
    }
    
    // Liberar la memoria
    free(arreglo);
    
    return 0;
}