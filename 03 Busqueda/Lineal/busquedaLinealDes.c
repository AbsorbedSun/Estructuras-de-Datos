/*  
busquedaLinealDes.c (Busqueda Lineal)
Autor: Aldo Garcia Ambrosio (C) Abril 2025
Autor origina: Edgardo Adrián Franco Martínez (pseudocódigo)
Uso de IA (GPT o4, Claude)
Version: 1.0
  
Programa que recibe aun arreglo deordenado y un elemento a buscar y realiza una busqueda lineal a un arreglo de n elementos desordenados.
  
Complejidad: O(n).
  
Observaciones: 
  
Compilación: Windows / Linux: gcc busquedaLinealDes.c -o bLinealDes

Ejecución: ./bLinealDes {elementos del arreglo} {elemento a buscar} < lista_caracteres.txt
            ./bLinealDes {elementos del arreglo} {elemento a buscar} < lista_caracteres.txt > salida.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Prototipos de la funcionbusqueda Secuencial
int busquedaLinealDes(int *ARREGLO, int n, int elemento, int *comparaciones);


/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresados al programa y char *arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de busqueda lineal
para hallar un elemento dentro de un ARREGLO de n elementos desordenados, ademas de imprimir el numero de comparaciones realizadas.
*/
int main(int numArg, char *argUser[]){
    //Recibir por argumento el tamaño de n y el valor / intervalo de valores a usar
	if (numArg != 3) {
		printf("\nIndique el tamaño de n y el valor / intervalo de valores a usar - Ejemplo: [cantidad] < {numeros.txt}\n",argUser[0]);
		exit(1);
	} 

    // Variable que define el numero de elementos a leer y ordenar (Arreglo)
    int n = atoi(argUser[1]);
    // Variable que define el elemento a buscar dentro del Arreglo
    int elemento = atoi(argUser[2]);
    
    // Apartar memoria para n números enteros
    int *ARREGLO = malloc(n * sizeof(int));
    
    // Validar que se haya apartado la memoria correctamente
    if (ARREGLO == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    
    //Lee de la entrada estandar los n valores y los coloca en un arreglo
    for (int i = 0; i < n; i++) {
        scanf("%d", &ARREGLO[i]);
    }

    //*****************************************  
	// Algoritmo de Busqueda Lineal
	//*****************************************
    
    // Variable para contar las comparaciones
    int comparaciones = 0;


    // Llamar al Algoritmo de Busqueda Lineal Desordenada
    int posicion = busquedaLinealDes(ARREGLO, n, elemento, &comparaciones);
    if(posicion != -1) {
        printf("\nEl elemento %d se encuentra en la posicion %d, realizo un total de: %d comparaciones\n", elemento, posicion, comparaciones);
    } else {
        printf("\nEl elemento %d no se encuentra en el arreglo, realizo un total de: %d comparaciones\n", elemento, comparaciones);
    }
    
    // Liberar la memoria
    free(ARREGLO);
    
    return 0;

}


/* 
int busquedaLinealDes(int *ARREGLO, int n, int elemento)
Recibe: int *ARREGLO como puntero al arreglo, n como tamaño del arreglo y elemento como valor a buscar
Devuelve: int que representa la posición del elemento buscado o -1 si no se encuentra
Observaciones: Función que implementa el algoritmo de búsqueda lineal, recorriendo el arreglo
elemento por elemento hasta encontrar el valor buscado o llegar al final del arreglo.
*/
int busquedaLinealDes(int *ARREGLO, int n, int elemento, int *comparaciones) {
    // Inicializa el resultado con -1, indicando que el elemento no se ha encontrado
    int resultado = -1;

    // Inicializa el contador de comparaciones
    *comparaciones = 0;
    
    // Recorre el arreglo hasta el ultimo elemento del arreglo
    for(int i = 0; i <= (n-1); i++) {
        // Incrementa el contador de comparaciones
        (*comparaciones)++;

        // Si el elemento actual coincide con el buscado, guarda su posición
        if (ARREGLO[i] == elemento) {
            resultado = i;
            break;  // Sale del ciclo al encontrar el valor solicitado
        }
    }
    
    // Retorna la posición del elemento si se encontró, o -1 si no existe en el arreglo
    return resultado;
}