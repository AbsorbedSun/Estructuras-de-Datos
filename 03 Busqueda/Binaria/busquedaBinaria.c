/*  
busquedaLBinaria.c
Autor: Aldo Garcia Ambrosio (C) Abril 2025
Autor origina: Edgardo Adrián Franco Martínez (pseudocódigo)
Uso de IA (GPT o4, Claude)
Version: 1.0
  
Programa que recibe aun arreglo ordenado y un elemento a buscar y realiza una busqueda binaria a un arreglo de n elementos.
  
Complejidad: O(log n) en el mejor y peor caso, O(n) en el caso de que el arreglo no esté ordenado.
  
Observaciones: 

Compilación: Windows / Linux: gcc busquedaBunaria.c -o bBinaria

Ejecución: ./bBinaria {elementos del arreglo} {elemento a buscar} < lista_caracteres.txt
            ./bBinaria {elementos del arreglo} {elemento a buscar} < lista_caracteres.txt > salida.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Prototipos de la funcion busquedaBinaria
int busquedaBinaria(int *ARREGLO, int elemento, int inicio, int fin, int *comparaciones);


/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresados al programa y char *arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de busqueda binaria recursiva
para hallar un elemento dentro de un ARREGLO de n elementos, ademas de imprimir el numero de comparaciones realizadas.
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
	// Algoritmo de Busqueda Binaria
	//*****************************************

    // Variable para contar las comparaciones
    int comparaciones = 0;


    // Llamar al Algoritmo de Busqueda Binaria
    int posicion = busquedaBinaria(ARREGLO, elemento, 0, n - 1, &comparaciones);

    if(posicion != -1) {
        printf("\nEl elemento %d se encuentra en la posicion %d, relizo: %d comparaciones\n", elemento, posicion, comparaciones);
    } else {
        printf("\nEl elemento %d no se encuentra en el arreglo, relizo: %d comparaciones\n", elemento, comparaciones);
    }
    
    // Liberar la memoria
    free(ARREGLO);
    
    return 0;

}


/* 
int busquedaBinaria(int *ARREGLO, int elemento, int inicio, int fin)
Recibe: int *ARREGLO como puntero al arreglo ordenado, elemento como valor a buscar,
inicio como índice inicial de búsqueda y fin como índice final de búsqueda
Devuelve: int que representa la posición del elemento si se encuentra, -1 en caso contrario
Observaciones: Función que implementa el algoritmo de búsqueda binaria para encontrar
un elemento en un arreglo ordenado, dividiendo repetidamente el espacio
de búsqueda a la mitad.
 */
int busquedaBinaria(int *ARREGLO, int elemento, int inicio, int fin, int *comparaciones) {
    // Inicializa el resultado como -1 (no encontrado)
    int resultado = -1;

    // Inicializa el contador de comparaciones
    *comparaciones = 0;
    
    // Mientras el espacio de búsqueda no se haya agotado
    while (inicio <= fin) {
        // Calcula el punto medio del espacio de búsqueda
        int mitad = (inicio + fin) / 2;

        // Incrementa el contador de comparaciones
        (*comparaciones)++;
        
        // Si encuentra el elemento, guarda su posición y termina la búsqueda
        if (elemento == ARREGLO[mitad]) {
            resultado = mitad;
            break; 
        } 
        // Si el elemento buscado es mayor, busca dentro de la mitad derecha
        else if (elemento > ARREGLO[mitad]) {
            inicio = mitad + 1;
        } 
        // Si el elemento buscado es menor, busca dentro de la mitad izquierda
        else {
            fin = mitad - 1;
        }
    }
    
    // Retorna la posición del elemento o -1 si no se encontró
    return resultado;
}