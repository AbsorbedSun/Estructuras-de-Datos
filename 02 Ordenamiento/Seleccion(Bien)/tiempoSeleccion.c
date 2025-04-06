/*
tiempoSeleccion.c
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


void ordenSeleccion(int *ARREGLODES, int n); // Prototipo de la función ordenSeleccion
	
/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento seleccion
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
    int *ARREGLO = malloc(n * sizeof(int));
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
	// Algoritmo de Ordenamiento Seleccion
	//*****************************************
	//Llamar al algoritmo
	ordenSeleccion(ARREGLO,n);
	
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
void ordenSeleccion(int *ARREGLODES, int n)
Recibe: int * ARREGLO (puntero) como ARREGLODES y n como tamaño del ARREGLO
Devuelve: void (No retorna valor explicito)
Observaciones: Función que ordena el ARREGLODES de menor a mayor haciendo uso del algoritmo de ordenamiento 
Seleccion (busca el mínimo en la parte desordenada y lo intercambia con el primer elemento de esa parte ademas de dividir en parte ordenada y no ordenada).
*/
void ordenSeleccion(int *ARREGLODES, int n) {
    // Iterar sobre el ARREGLO partiendo del segundo elemento¿?
    for(int k = 0; k <= (n - 2); k++){
        // Indice del menos valor en la parte ordenada, inicia tomando el primer elemento de la parte desordenada
        int posMin = k;
        // Iteracion para encontrar el minimo en la parte desordenada
        for(int i = k + 1; i <= (n - 1); i++){
            if(ARREGLODES[i] < ARREGLODES[posMin]){
                // Asignar el nuevo valor minimo
                posMin = i;
            }
        }
        // Intercambiar el valor minimo con el primer elemento de la parte desordenada
        int temp = ARREGLODES[posMin]; // Asigna el valor minimo a una variable temporal
        ARREGLODES[posMin] = ARREGLODES[k]; // Reemplaza el menor valor encontrado por el primer elemento de la parte desordenada
        ARREGLODES[k] = temp; // Coloca el menor valor en la posición k (moviendolo a la parte ordenada)
    }
}
