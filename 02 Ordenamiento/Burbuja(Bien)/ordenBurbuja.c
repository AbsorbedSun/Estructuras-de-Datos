/*  
  Autor: Aldo Garcia Ambrosio (C) Marzo 2025
  Autor origina: Edgardo Adrián Franco Martínez (pseudocódigo)
  Uso de IA (GPT o4, Claude)
  Version: 1.4
  
  Programa que lee n caracteres de la entrada estándar y los ordena con el algoritmo
  de ordenamiento Burbuja. 
  
  Complejidad: O(n^2).
  
  Observaciones: 
  
  Compilación: Windows / Linux: gcc ordenBurbuja.c -o burbuja

  Ejecución: ./burbuja {elementos a ordenar}  < lista_caracteres.txt
            ./burbuja {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
void ordenBurbuja(int *arregloDes, int n)
Recibe: int * arreglo (puntero) como arregloDes y n como tamaño del arreglo
Devuelve: void (No retorna valor explicito)
Observaciones: Función que orderna el arregloDes de menor a mayor haciendo 
uso del algoritmo de ordenamiento burbuja (compara de 2 en 2 y ordena) a razón de O(n^2).
*/
void ordenBurbuja(int *arregloDes, int n) {
    // Indica si se realizaron cambios en la iteracion
    bool cambios = true;
    // Variable de control de iteracion
    int i = 0;
    // Ciclo principal, se ejecuta mientras haya intercambios y no se haya recorrido todo el arreglo
    while (i <= (n - 2) && cambios != false) {
        cambios = false;

        // Iteracion para recorrer y comparar de 2 en 2 (elementos consecutivos) y ordenar
        for (int j = 0; j < (n - 1) - i; j++) {
            
            // Comparacion y ordenamiento (Si están desordenados, se intercambian)
            if (arregloDes[j] > arregloDes[j + 1]) {
                // Variable auxiliar para el intercambio (almacena el valor antes de sobreescribirlo)
                int aux = arregloDes[j];
                arregloDes[j] = arregloDes[j + 1];
                arregloDes[j + 1] = aux;
                cambios = true;
            }
        }
        i++;
    }
    
}

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento burbuja
para ordenar un arreglo de n elementos, ademas de imprimir el resultado en lista.
*/
int main(int num_arg, char *arg_user[]) {

    //Recibir por argumento el tamaño de n y el valor / intervalo de valores a usar
	if (num_arg != 2) {
		printf("\nIndique el tamaño de n y el valor / intervalo de valores a usar - Ejemplo: [cantidad] < {numeros.txt}\n",arg_user[0]);
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
	// Algoritmo de Ordenamiento Burbuja
	//*****************************************
    // Llamar a la función de ordenamiento
    ordenBurbuja(arreglo, n);
    
    // Mostrar el arreglo ordenado
    printf("Arreglo despues del ordenamiento: \n");
    for (int i = 0; i < n; i++) {
        printf("%d \n", arreglo[i]);
    }
    
    // Liberar la memoria
    free(arreglo);
    
    return 0;
}