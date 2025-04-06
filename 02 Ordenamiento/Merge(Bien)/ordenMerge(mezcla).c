/*  
  Autor: Aldo Garcia Ambrosio (C) Marzo 2025
  Autor origina: Edgardo Adrián Franco Martínez (pseudocodigo)
  Uso de IA (GPT o4, Claude)
  Version: 1.2
  
  Programa que lee n caracteres de la entrada estándar y los ordena con el algoritmo
  de ordenamiento Merge o mezcla.
  
  Complejidad: O(nlog2(n))
  
  Observaciones: Se tomara la estructura Main de ordenBurbuja.c para realizar agilizar la tarea y permitir la realización del ordenamiento por merge.
  
  Compilación: Windows / Linux: gcc ordenMerge(mezcla).c -o merge

  Ejecución: ./merge {elementos a ordenar} < lista_caracteres.txt
            ./merge {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
void Merge(int *arregloDes, int posIni, int mitad, int posFin)
Recibe: int * arreglo (puntero) como arregloDes, posIni como posición inicial, mitad como punto medio y posFin como posición final
Devuelve: void (No retorna valor explícito)
Observaciones: Función que mezcla dos subarreglos ordenados en un solo arreglo ordenado
*/
void Merge(int *arregloDes, int posIni, int mitad, int posFin){
    // Calcula el tamaño del arreglo a mezclar, indice para el subarreglo izquierdo, indice para el subarreglo derecho y un indice auxiliar
    int l = posFin - posIni + 1, i = posIni, j = mitad + 1, k;
    // Arreglo auxiliar para almacenar los valores ordenados
    int *arregloAux = malloc(l * sizeof(int));

    // Iteracion que recorre los elementos del arreglo auxiliar
    for(k = 0; k <= (l - 1); k++){
        // Verifica si los indices no han llegado al final de los subarreglos
        if(i <= mitad && j <= posFin){
            // Compara elementos de ambos subarreglos y toma el menor
            if(arregloDes[i] < arregloDes[j]){
                arregloAux[k] = arregloDes[i];
                i++;
            }else{
                arregloAux[k] = arregloDes[j];
                j++;
            }
        } else if(i <= mitad){
            // Si ya no hay elementos en el subarreglo derecho, toma los del izquierdo
            arregloAux[k] = arregloDes[i];
            i++;
        } else{
            // Si ya no hay elementos en el subarreglo izquierdo, toma los del derecho
            arregloAux[k] = arregloDes[j];
            j++;
        }
    }
    // Copia los elementos mezclados de vuelta al arreglo original
    k = posIni;
    for(int i = 0; i < (l); i++){ // ---------------------> Elinminar (l-1) a (l) para obtener el ordenamiento correcto
        arregloDes[k] = arregloAux[i]; 
        k++;
    }

    free(arregloAux);
}

/*
void mergeSort(int *arregloDes, int posIni, int posFin)
Recibe: int * arreglo (puntero)como arregloDes y posIni y posFin como enteros indica la posición inicial y final del arreglo.   
Devuelve: void (No retorna valor explicito)
Observaciones: Función que orderna el arregloDes de menor a mayor haciendo uso del
algoritmo de ordenamiento Merge o mezcla de forma recursiva (divide el arreglo en mitades, ordena cada mitad y luego las mezcla).
*/
void mergeSort(int *arregloDes, int posIni, int posFin) {
    if (posIni < posFin){
        // Calcula el punto medio/mitad del arreglo
        int mitad = (int)((posIni + posFin) / 2);
        // Llamada recursiva para la primera mitad
        mergeSort(arregloDes, posIni, mitad);
        // Llamada recursiva para la segunda mitad
        mergeSort(arregloDes, (mitad + 1), posFin);
        // Mezcla las dos mitades ordenadas
        Merge(arregloDes, posIni, mitad, posFin);
    }
}

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento mergeSort
para ordenar un arreglo de n elementos, ademas de imprimir el resultado en lista.
*/
int main(int num_arg, char *arg_user[]) {

    //Recibir por argumento el tamaño de n y el valor / intervalo de valores a usar
	if (num_arg != 2) 
	{
		printf("\nIndique el tamaño de n y el valor / intervalo de valores a usar - Ejemplo: [fin intervalo] < {numeros.txt}\n",arg_user[0]);
		exit(1);
	} 

    // Variable que define el numero de elementos a leer y ordenar
    int fin = atoi(arg_user[1]);
    
    // Apartar memoria para n números enteros
    // (fin + 1) tiene en cuenta los índices 0 a fin
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
    
    // Mostrar el arreglo antes del ordenamiento
    printf("Arreglo antes del ordenamiento: \n");
    for (int i = 0; i < fin; i++) {
        printf("%d \n", arreglo[i]);
    }
    printf("\n");

    //*****************************************  
	// Algoritmo de Ordenamiento Merge
	//*****************************************
    // Llamar a la función de ordenamiento
    mergeSort(arreglo, 0, fin - 1);
    
    // Mostrar el arreglo ordenado
    printf("Arreglo despues del ordenamiento: \n");
    for (int i = 0; i < fin; i++) {
        printf("%d \n", arreglo[i]);
    }
    
    // Liberar la memoria
    free(arreglo);
    
    return 0;
}