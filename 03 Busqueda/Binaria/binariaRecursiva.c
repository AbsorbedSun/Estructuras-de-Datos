/*  
binariaRecursiva.c
Autor: Aldo Garcia Ambrosio (C) Abril 2025
Autor origina: Edgardo Adrián Franco Martínez (pseudocódigo)
Uso de IA (GPT o4, Claude)
Version: 1.0
  
Programa que recibe aun arreglo ordenado y un elemento a buscar y realiza una busqueda binaria recursiva a un arreglo de n elementos.
  
Complejidad: O(log n) en el mejor y peor caso, O(n) en el caso de que el arreglo no esté ordenado.
  
Observaciones: 

Compilación: Windows / Linux: gcc binariaRecursiva.c -o binariaR

Ejecución: ./binariaR {elementos del arreglo} {elemento a buscar} < lista_caracteres.txt
            ./binariaR {elementos del arreglo} {elemento a buscar} < lista_caracteres.txt > salida.txt
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
Observaciones: Función principal que permite hacer uso de la función de busqueda binaria
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
Devuelve: int que representa la posición del elemento buscado o -1 si no se encuentra
Observaciones: Función que implementa el algoritmo de búsqueda binaria para arreglos ordenados,
dividiendo el espacio de búsqueda a la mitad en cada iteración recursiva. Este algoritmo 
tiene una complejidad de O(log n), siendo más eficiente que la búsqueda lineal para arreglos grandes.
*/
int busquedaBinaria(int *ARREGLO, int elemento, int inicio, int fin, int *comparaciones) {
   // Variable para almacenar el resultado, inicializada como -1 (no encontrado)
   int resultado = -1;
   
   // Caso base: si el índice inicial supera al final, el elemento no está en el arreglo
   if(inicio > fin){
       return -1;
   } else{
       // Calcula el índice del elemento medio
       int mitad = (inicio + fin) / 2;

       // Incrementa el contador de comparaciones
       (*comparaciones)++;
       
       // Si el elemento del medio es el buscado, retorna su posición
       if (elemento == ARREGLO[mitad]) {
           return mitad;
       } 
       // Si el elemento buscado es mayor, busca en la mitad derecha del subarreglo
       else if (elemento > ARREGLO[mitad]) {
           return busquedaBinaria(ARREGLO, elemento, mitad+1, fin, comparaciones);
       } 
       // Si el elemento buscado es menor, busca en la mitad izquierda del subarreglo
       else {
           return busquedaBinaria(ARREGLO, elemento, inicio, mitad-1, comparaciones);
       }
   }
}