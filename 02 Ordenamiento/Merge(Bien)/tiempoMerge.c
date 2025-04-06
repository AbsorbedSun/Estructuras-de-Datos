/*
tiempoMerge.c
Autor: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Edgardo Adrián Franco Martínez
Uso de IA (GPT o4, Claude)
Version: 1.0

Programa que lee n números de la entrada estandar y los coloca en un ARREGLO A, 
después ejecuta un algoritmo que invierte el ARREGLO y posteriormente pierde el 
tiempo en orden O(n^2).

Compilación 
Windows: gcc tiempo.c -o tiempo.exe
LinUX: gcc tiempo.c -o tiempo

Ejecución: 
./burbuja {elementos a ordenar}  < lista_caracteres.txt
./burbuja {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

void Merge(int *ARREGLODES, int posIni, int mitad, int posFin); //Prototipo de la función Merge

void mergeSort(int *ARREGLODES, int posIni, int posFin); // Prototipo de la función mergeSort

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento merge
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
	// Algoritmo de Ordenamiento Merge
	//*****************************************
    // Llamar a la función de ordenamiento
    mergeSort(ARREGLO, 0, n - 1);
	
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
void Merge(int *ARREGLODES, int posIni, int mitad, int posFin)
Recibe: int * ARREGLO (puntero) como ARREGLODES, posIni como posición inicial, mitad como punto medio y posFin como posición final
Devuelve: void (No retorna valor explícito)
Observaciones: Función que mezcla dos subARREGLOs ordenados en un solo ARREGLO ordenado
*/
void Merge(int *ARREGLODES, int posIni, int mitad, int posFin){
    // Calcula el tamaño del ARREGLO a mezclar, indice para el subARREGLO izquierdo, indice para el subARREGLO derecho y un indice aUXiliar
    int l = posFin - posIni + 1, i = posIni, j = mitad + 1, k;
    // ARREGLO aUXiliar para almacenar los valores ordenados
    int *ARREGLOAUX = malloc(l * sizeof(int));

    // Iteracion que recorre los elementos del ARREGLO aUXiliar
    for(k = 0; k <= (l - 1); k++){
        // Verifica si los indices no han llegado al final de los subARREGLOs
        if(i <= mitad && j <= posFin){
            // Compara elementos de ambos subARREGLOs y toma el menor
            if(ARREGLODES[i] < ARREGLODES[j]){
                ARREGLOAUX[k] = ARREGLODES[i];
                i++;
            }else{
                ARREGLOAUX[k] = ARREGLODES[j];
                j++;
            }
        } else if(i <= mitad){
            // Si ya no hay elementos en el subARREGLO derecho, toma los del izquierdo
            ARREGLOAUX[k] = ARREGLODES[i];
            i++;
        } else{
            // Si ya no hay elementos en el subARREGLO izquierdo, toma los del derecho
            ARREGLOAUX[k] = ARREGLODES[j];
            j++;
        }
    }
    // Copia los elementos mezclados de vuelta al ARREGLO original
    k = posIni;
    for(int i = 0; i < (l); i++){ // ---------------------> Elinminar (l-1) a (l) para obtener el ordenamiento correcto
        ARREGLODES[k] = ARREGLOAUX[i]; 
        k++;
    }

    free(ARREGLOAUX);
}

/*
void mergeSort(int *ARREGLODES, int posIni, int posFin)
Recibe: int * ARREGLO (puntero)como ARREGLODES y posIni y posFin como enteros indica la posición inicial y final del ARREGLO.   
Devuelve: void (No retorna valor explicito)
Observaciones: Función que orderna el ARREGLODES de menor a mayor haciendo uso del
algoritmo de ordenamiento Merge o mezcla de forma recursiva (divide el ARREGLO en mitades, ordena cada mitad y luego las mezcla).
*/
void mergeSort(int *ARREGLODES, int posIni, int posFin) {
    if (posIni < posFin){
        // Calcula el punto medio/mitad del ARREGLO
        int mitad = (int)((posIni + posFin) / 2);
        // Llamada recursiva para la primera mitad
        mergeSort(ARREGLODES, posIni, mitad);
        // Llamada recursiva para la segunda mitad
        mergeSort(ARREGLODES, (mitad + 1), posFin);
        // Mezcla las dos mitades ordenadas
        Merge(ARREGLODES, posIni, mitad, posFin);
    }
}
	
