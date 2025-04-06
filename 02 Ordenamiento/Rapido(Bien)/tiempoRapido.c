/*
tiempoRapido.c
Autor: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Edgardo Adrián Franco Martínez
Uso de IA (GPT o4, Claude)
Version: 1.0

Programa que lee n números de la entrada estandar y los coloca en un ARREGLO A, 
después ejecuta un algoritmo que invierte el ARREGLO y posteriormente pierde el 
tiempo en orden O(n^2).

Compilación 
Windows: gcc tiempo.c -o tiempo.exe
Linux: gcc tiempo.c -o tiempo

Ejecución: 
./burbuja {elementos a ordenar}  < lista_caracteres.txt
./burbuja {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

void Intercambio(int *ARREGLODES, int inicial, int j); // Intercambia dos elementos del ARREGLODES
int Pivot(int *ARREGLODES, int posIni, int posFin); // Particiona el ARREGLODES alrededor de un pivote
void quickSort(int *ARREGLODES, int posIni, int posFin); // Ordena el ARREGLODES utilizando el algoritmo QuickSort
	
/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento rapido
para ordenar un ARREGLO de n elementos, ademas de imprimir el tiempo de ejecucion y elresultado en lista .
*/
int main(int num_arg, char* arg_user[]){	
    //Variables para la medición de tiempos    
    clock_t t_inicio, t_final;
    double t_intervalo;
	
	//Recibir por argumento el tamaño de n y el valor a buscar
	if (num_arg != 2) {
		printf("\nIndique el tamanio de n y el valor a buscar - Ejemplo: [user@equipo]$ %s 100\n",arg_user[0]);
		exit(1);
	} 
	
	//Tomar el argumento del main
	int n = atoi(arg_user[1]); //n
	
	// Apartar memoria para n números enteros
    int *ARREGLO = malloc((n + 1) * sizeof(int));
	if( ARREGLO == NULL){
		printf("\nError al intentar reservar memoria para %d elementos\n",n);		
		exit(1);
	}	
	
	//Leer de la entrada estandar los n valores y colocarlos en el ARREGLO de números
	for(int i = 0; i < n; i++){
		scanf("%d",&ARREGLO[i]);
	}
	
	//*****************************************  
	//Algoritmo a medir el tiempo
	//*****************************************
	//Inicia medición del tiempo
	t_inicio = clock();

	//*****************************************  
	// Algoritmo de Ordenamiento Rapido
	//*****************************************
    // Llamar a la función de ordenamiento
    quickSort(ARREGLO, 0, n - 1);
	
	//Termina medición del tiempo	
	t_final = clock();
	
	//Cálculo del tiempo y enviar mensaje a salida estandar con la medición  
    t_intervalo = (double)(t_final - t_inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo medido: %.10f segundos.", t_intervalo);
	
	//Enviar a la salida estandar el ARREGLO final
	for(int i = 0; i < n; i++)
		printf("\nArreglo[%d] = %d", i, ARREGLO[i]);

	// Liberar la memoria
    free(ARREGLO);
	
	return 0;
}


/* 
void Intercambio(int *ARREGLODES, int inicial, int j)
Recibe: int *ARREGLODES como puntero al ARREGLO, i y j como índices de los elementos a intercambiar
Devuelve: void (No retorna valor explícito)
Observaciones: Función auxiliar que intercambia dos elementos del ARREGLO utilizando una 
variable temporal. 
*/
void Intercambio(int *ARREGLODES, int inicial, int j) {
    // Guarda el valor del elemento en la posición i en una variable temporal
    int temp = ARREGLODES[j];
    ARREGLODES[j] = ARREGLODES[inicial];
    // Asigna el valor temporal al elemento en la posición j
    ARREGLODES[inicial] = temp;
}


/* 
int Pivot(int *ARREGLODES, int posIni, int posFin)
Recibe: int *ARREGLODES como puntero al ARREGLO, posIni como posición inicial y posFin como posición final
Devuelve: int que representa la posición final del pivote después de la partición
Observaciones: Función que particiona el ARREGLO alrededor de un pivote (primer elemento), 
colocando los elementos menores a la izquierda y los mayores a la derecha del pivote. 
*/
int Pivot(int *ARREGLODES, int posIni, int posFin) {
    // Selecciona el primer elemento como pivote, i: índice que recorre desde posIni+1 hacia la derecha
    // j: índice que recorre desde la posFin hacia la izquierda
    int piv = ARREGLODES[posIni], i = posIni + 1, j = posFin;

    // Mientras los indices no se crucen...
    while (i <= j){
        // Avanza i mientras el elemento sea menor o igual al pivote y no se salga del límite
        while (ARREGLODES[i] <= piv && i < posFin){
            i++;  
        }
        // Retrocede j mientras el elemento sea mayor que el pivote
        while (ARREGLODES[j] > piv){
            j--;
        }
        // Si los índices no se han cruzado, intercambia los elementos
        if(i < j){
            Intercambio(ARREGLODES, i, j);
        }
    }
    // Coloca el pivote en su posición final
    Intercambio(ARREGLODES, posIni, j);
    return j;
}


/* 
void quickSort(int *ARREGLODES, int posIni, int posFin)
Recibe: int *ARREGLODES como puntero al ARREGLO, posIni como posición inicial y posFin como posición final
Devuelve: void (No retorna valor explícito)
Observaciones: Función que ordena el ARREGLODES de menor a mayor haciendo uso del algoritmo 
de ordenamiento QuickSort (selecciona un pivote, particiona el ARREGLO y ordena recursivamente 
cada partición). 
*/
void quickSort(int *ARREGLODES, int posIni, int posFin) {
    // Verifica si hay al menos dos elementos para ordenar
    if(posIni < posFin){
        // Obtiene la posición final del pivote después de particionar
        int j = Pivot(ARREGLODES, posIni, posFin);
        // Ordena recursivamente la partición izquierda
        quickSort(ARREGLODES, posIni, j-1);
        // Ordena recursivamente la partición derecha
        quickSort(ARREGLODES, j+1, posFin);
    }
}