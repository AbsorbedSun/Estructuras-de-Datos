/*  
  Autor: Aldo Garcia Ambrosio (C) Marzo 2025
  Autor origina: Uso de IA (GPT o4, Claude)
  Version: 1.4
  
  Programa que lee n caracteres de la entrada estándar y los ordena con el algoritmo
  de ordenamiento Shell.
  
  Observaciones: Se tomara la estructura Main de ordenBurbuja.c para realizar agilizar la tarea y realizar el ordenamiento por inserción.
  
  Compilación: Windows / Linux: gcc ordenShell.c -o shell

  Ejecución: ./shell {elementos a ordenar}  < lista_caracteres.txt
            ./shell {elementos a ordenar} < lista_caracteres.txt > salida.txt
*/

#include <stdio.h>
#include <stdlib.h>
#define TRUNC(x) ((int)(x))

/*
void ordenShell(int *arregloDes, int n)
Recibe: int *arregloDes como puntero al arreglo y n como tamaño del arreglo
Devuelve: void (No retorna valor explícito)
Observaciones: Función que ordena el arregloDes de menor a mayor haciendo uso del
algoritmo de ordenamiento Shell (una mejora del ordenamiento por inserción que compara
elementos distantes entre sí).
*/
void ordenShell(int *arregloDes, int n) {
    // Inicializa k como la mitad del tamaño del arreglo (truncado)
    int k = TRUNC(n/2);
    // Continúa mientras el intervalo sea al menos 1
    while(k >= 1){
        // Variable que indica si hubo intercambios en la iteración
        int b = 1;
        // Repite hasta que no haya intercambios
        while(b != 0){
            // Reinicio del contador
            b = 0;
            // Recorre el arreglo comparando elementos separados por k posiciones
            for(int i = k; i <= (n - 1); i++){
                // Si el elemento k posiciones atrás es mayor, intercambia
                if(arregloDes[i - k] > arregloDes[i]){
                    // Realiza el intercambio usando una variable temporal
                    int temp = arregloDes[i];
                    arregloDes[i] = arregloDes[i - k];
                    arregloDes[i - k] = temp;
                    b++;
                }
            }
        }
        // Reduce el intervalo a la mitad (truncado)
        k = TRUNC(k/2);
    }
}

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresadoros al programa y char * arg_user (puntero) 
como un arerglo que alamcena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite hacer uso de la función de ordenamiento shell
para ordenar un arreglo de n elementos, ademas de imprimir el resultado en lista.
*/
int main(int num_arg, char *arg_user[]) {

    //Recibir por argumento el tamaño de n y el valor / intervalo de valores a usar
	if (num_arg != 2) 
	{
		printf("\nIndique el tamanio de n y el valor / intervalo de valores a usar - Ejemplo: [cantidad] < {numeros.txt}\n",arg_user[0]);
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
	// Algoritmo de Ordenamiento Shell
	//*****************************************
    // Llamar a la función de ordenamiento
    ordenShell(arreglo, n);
    
    // Mostrar el arreglo ordenado
    printf("Arreglo despues del ordenamiento: \n");
    for (int i = 0; i < n; i++) {
        printf("%d \n", arreglo[i]);
    }
    
    // Liberar la memoria
    free(arreglo);
    
    return 0;
}