/*
tiempoInsercion.c
Autor: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Edgardo Adrián Franco Martínez
Uso de IA (GPT o4, Claude)
Version: 1.0

Programa que lee n números de la entrada estandar y los coloca en un Arreglo A, 
después ejecuta un algoritmo que invierte el Arreglo y posteriormente pierde el 
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

void ordenInsercion(int *ArregloDES, int n); // Prototipo de la función ordenInsercion

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento insercion
para ordenar un Arreglo de n elementos, ademas de imprimir el tiempo de ejecucion y el resultado en lista .
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
    int *Arreglo = malloc(n * sizeof(int));
	if( Arreglo == NULL){
		printf("\nError al intentar reservar memoria para %d elementos\n",n);		
		exit(1);
	}	
	
	//Leer de la entrada estandar los n valores y colocarlos en el Arreglo de números
	for(int i = 0; i < n; i++){
		scanf("%d",&Arreglo[i]);
	}
	
	//*****************************************  
	//Algoritmo a medir el tiempo
	//*****************************************
	//Inicia medición del tiempo
	t_inicio = clock();

	//*****************************************  
	// Algoritmo de Ordenamiento Insercion
	//*****************************************
	//Llamar al algoritmo
	ordenInsercion(Arreglo,n);
	
	//Termina medición del tiempo	
	t_final = clock();
	
	//Cálculo del tiempo y enviar mensaje a salida estandar con la medición  
    t_intervalo = (double)(t_final - t_inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo medido: %.10f segundos.", t_intervalo);
	
	//Enviar a la salida estandar el Arreglo final
	for(int i = 0; i < n; i++)
		printf("\nArreglo[%d] = %d", i, Arreglo[i]);

	// Liberar la memoria
    free(Arreglo);
	
	return 0;
}

/*
void ordenInsercion(int *ArregloDES, int n)
Recibe: int * Arreglo (puntero) como ArregloDES y n como tamaño del Arreglo
Devuelve: void (No retorna valor explicito)
Observaciones: Función que orderna el ArregloDES de menor a mayor haciendo uso del
algoritmo de ordenamiento Insercion (acomodando los elementos en orden ascendente
tomando elemento por elemento acomodandolo en su correspondiente lugar en el Arreglo ya 
ordenado) a razón de O(n^2).
*/
void ordenInsercion(int *ArregloDES, int n) {
    // Iteracion para recorrer el Arreglo (recorriendo desde el segundo elemento hasta el último)
    for(int i = 0; i <= (n - 1); i++) { 
        // Variable de apoyo para el recorrido inverso del Arreglo (derecha a izquierda)
        int j = i;
        // Variable temporal para almacenar el valor actual
        int temp = ArregloDES[i];

        // Ciclo para recorrer/desplazar los elementos mayores del Arreglo hacia la derecha
        while(j > 0 && (temp < ArregloDES[j - 1])){
            // Movimiento de los elementos mayores a la derecha
            ArregloDES[j] = ArregloDES[j - 1];
            j--;
        }
        // Inserta el valor en su lugar correspondiente dentro del subArreglo ordenado
        ArregloDES[j] = temp; 
    }
    
}
	

