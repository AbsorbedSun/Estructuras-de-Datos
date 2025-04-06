/*
tiempoShell.c
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
#define TRUNC(x) ((int)(x))

void ordenShell(int *ARREGLODES, int n); // Prototipo de la función ordenShell

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento shell
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
	ordenShell(ARREGLO,n);
	
	//Termina medición del tiempo	
	t_final = clock();
	
	//Cálculo del tiempo y enviar mensaje a salida estandar con la medición  
    t_intervalo = (double)(t_final - t_inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo medido: %.10f segundos.", t_intervalo);
	
	//Enviar a la salida estandar el ARREGLO final
	//for(int i = 0; i < n; i++)
	//	printf("\nArreglo[%d] = %d", i, ARREGLO[i]);

	// Liberar la memoria
    free(ARREGLO);
	
	return 0;
}

/*
void ordenShell(int *ARREGLODES, int n)
Recibe: int *ARREGLODES como puntero al ARREGLO y n como tamaño del ARREGLO
Devuelve: void (No retorna valor explícito)
Observaciones: Función que ordena el ARREGLODES de menor a mayor haciendo uso del
algoritmo de ordenamiento Shell (una mejora del ordenamiento por inserción que compara
elementos distantes entre sí).
*/
void ordenShell(int *ARREGLODES, int n) {
    // Inicializa k como la mitad del tamaño del ARREGLO (truncado)
    int k = TRUNC(n/2);
    // Continúa mientras el intervalo sea al menos 1
    while(k >= 1){
        // Variable que indica si hubo intercambios en la iteración
        int b = 1;
        // Repite hasta que no haya intercambios
        while(b != 0){
            // Reinicio del contador
            b = 0;
            // Recorre el ARREGLO comparando elementos separados por k posiciones
            for(int i = k; i <= (n - 1); i++){
                // Si el elemento k posiciones atrás es mayor, intercambia
                if(ARREGLODES[i - k] > ARREGLODES[i]){
                    // Realiza el intercambio usando una variable temporal
                    int temp = ARREGLODES[i];
                    ARREGLODES[i] = ARREGLODES[i - k];
                    ARREGLODES[i - k] = temp;
                    b++;
                }
            }
        }
        // Reduce el intervalo a la mitad (truncado)
        k = TRUNC(k/2);
    }
}
