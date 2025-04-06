/*  
  Autor: Aldo Garcia Ambrosio (C) Marzo 2025
  Autor origina: Edgardo Adrián Franco Martínez
  Uso de IA (GPT o4, Claude)
  Version: 1.0
  
  Programa que lee n caracteres de la entrada estándar y los ordena con el algoritmo
  de ordenamiento Rapido.
  
  Complejidad: 
  
  Observaciones: Se tomara la estructura Main de ordenBurbuja.c para realizar agilizar la tarea y realizar el ordenamiento rapido.
  
  Compilación: Windows / Linux: gcc ordenRapido(mezcla).c -o rapido

  Ejecución: ./rapido {elementos a ordenar} < lista_caracteres.txt
            ./rapido {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 
void Intercambio(int *arregloDes, int inicial, int j)
Recibe: int *arregloDes como puntero al arreglo, i y j como índices de los elementos a intercambiar
Devuelve: void (No retorna valor explícito)
Observaciones: Función auxiliar que intercambia dos elementos del arreglo utilizando una 
variable temporal. 
*/
void Intercambio(int *arregloDes, int inicial, int j) {
    // Guarda el valor del elemento en la posición i en una variable temporal
    int temp = arregloDes[j];
    arregloDes[j] = arregloDes[inicial];
    // Asigna el valor temporal al elemento en la posición j
    arregloDes[inicial] = temp;
}


/* 
int Pivot(int *arregloDes, int posIni, int posFin)
Recibe: int *arregloDes como puntero al arreglo, posIni como posición inicial y posFin como posición final
Devuelve: int que representa la posición final del pivote después de la partición
Observaciones: Función que particiona el arreglo alrededor de un pivote (primer elemento), 
colocando los elementos menores a la izquierda y los mayores a la derecha del pivote. 
*/
int Pivot(int *arregloDes, int posIni, int posFin) {
    // Selecciona el primer elemento como pivote, i: índice que recorre desde posIni+1 hacia la derecha
    // j: índice que recorre desde la posFin hacia la izquierda
    int piv = arregloDes[posIni], i = posIni + 1, j = posFin;

    // Mientras los indices no se crucen...
    while (i <= j){
        // Avanza i mientras el elemento sea menor o igual al pivote y no se salga del límite
        while (arregloDes[i] <= piv && i <= posFin){
            i++;  
        }
        // Retrocede j mientras el elemento sea mayor que el pivote
        while (arregloDes[j] > piv){
            j--;
        }
        // Si los índices no se han cruzado, intercambia los elementos
        if(i < j){
            Intercambio(arregloDes, i, j);
        }
    }
    // Coloca el pivote en su posición final
    Intercambio(arregloDes, posIni, j);
    return j;
}


/* 
void quickSort(int *arregloDes, int posIni, int posFin)
Recibe: int *arregloDes como puntero al arreglo, posIni como posición inicial y posFin como posición final
Devuelve: void (No retorna valor explícito)
Observaciones: Función que ordena el arregloDes de menor a mayor haciendo uso del algoritmo 
de ordenamiento QuickSort (selecciona un pivote, particiona el arreglo y ordena recursivamente 
cada partición). 
*/
void quickSort(int *arregloDes, int posIni, int posFin) {
    // Verifica si hay al menos dos elementos para ordenar
    if(posIni < posFin){
        // Obtiene la posición final del pivote después de particionar
        int j = Pivot(arregloDes, posIni, posFin);
        // Ordena recursivamente la partición izquierda
        quickSort(arregloDes, posIni, j-1);
        // Ordena recursivamente la partición derecha
        quickSort(arregloDes, j+1, posFin);
    }
}


/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento rapido
para ordenar un arreglo de n elementos, ademas de imprimir el resultado en lista.
*/
int main(int num_arg, char *arg_user[]) {

    //Recibir por argumento el tamaño de n y el valor / intervalo de valores a usar
	if (num_arg != 2) 
	{
		printf("\nIndique el tamanio de n y el valor / intervalo de valores a usar - Ejemplo: [fin intervalo] < {numeros.txt}\n",arg_user[0]);
		exit(1);
	} 

    // Variable que define el numero de elementos a leer y ordenar
    int fin = atoi(arg_user[1]);
    
    // Apartar memoria para n números enteros
    int *arreglo = malloc((fin + 1) * sizeof(int));
    
    // Valir que se haya apartado la memoria correctamente
    if (arreglo == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    
    //Lee de la entrada estandar los n valores y los coloca en un arreglo
    for (int i = 0; i < fin; i++) {
        scanf("%d", &arreglo[i]);
    }
    //*****************************************  
	// Algoritmo de Ordenamiento Rapido
	//*****************************************
    // Llamar a la función de ordenamiento
    quickSort(arreglo, 0, fin - 1);
    
    // Mostrar el arreglo ordenado
    for (int i = 0; i < fin; i++) {
        printf("%d \n", arreglo[i]);
    }
    
    // Liberar la memoria
    free(arreglo);
    
    return 0;
}