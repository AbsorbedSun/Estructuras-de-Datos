/*  
  Autor: Aldo Garcia Ambrosio (C) Marzo 2025
  Autor origina: Edgardo Adrián Franco Martínez (pseudocódigo)
  Uso de IA (GPT o4, Claude)
  Version: 1.3
  
  Programa que lee n caracteres de la entrada estándar y los ordena con el algoritmo de ordenamiento Insercion.
  
  Complejidad: O(n^2).
  
  Observaciones: Se tomara la estructura Main de ordenBurbuja.c para realizar, agilizar la tarea y permitir realizar el ordenamiento por inserción.
  
  Compilación: Windows / Linux: gcc ordenInsercion.c -o insercion

  Ejecución: ./insercion {elementos a ordenar}  < lista_caracteres.txt
            ./insercion {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
void ordenInsercion(int *arregloDes, int n)
Recibe: int * arreglo (puntero) como arregloDes y n como tamaño del arreglo
Devuelve: void (No retorna valor explicito)
Observaciones: Función que orderna el arregloDes de menor a mayor haciendo uso del
algoritmo de ordenamiento Insercion (acomodando los elementos en orden ascendente
tomando elemento por elemento acomodandolo en su correspondiente lugar en el arreglo ya 
ordenado) a razón de O(n^2).
*/
void ordenInsercion(int *arregloDes, int n) {
    // Iteracion para recorrer el arreglo (recorriendo desde el segundo elemento hasta el último)
    for(int i = 0; i <= (n - 1); i++) { 
        // Variable de apoyo para el recorrido inverso del arreglo (derecha a izquierda)
        int j = i;
        // Variable temporal para almacenar el valor actual
        int temp = arregloDes[i];

        // Ciclo para recorrer/desplazar los elementos mayores del arreglo hacia la derecha
        while(j > 0 && (temp < arregloDes[j - 1])){
            // Movimiento de los elementos mayores a la derecha
            arregloDes[j] = arregloDes[j - 1];
            j--;
        }
        // Inserta el valor en su lugar correspondiente dentro del subarreglo ordenado
        arregloDes[j] = temp; 
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
	// Algoritmo de Ordenamiento Insercion
	//*****************************************
    // Llamar a la función de ordenamiento
    ordenInsercion(arreglo, n);
    
    // Mostrar el arreglo ordenado
    printf("Arreglo despues del ordenamiento: \n");
    for (int i = 0; i < n; i++) {
        printf("%d \n", arreglo[i]);
    }
    
    // Liberar la memoria
    free(arreglo);
    
    return 0;
}