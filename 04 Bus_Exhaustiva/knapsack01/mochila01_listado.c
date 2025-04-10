/*  
  mochilaBacktracking.c (Problema de la Mochila con Backtracking)
  Autor: Aldo Garcia Ambrosio (C) Abril 2025
  Uso de IA: Claude 3.7 Sonnet
  Version: 1.3
  
  Programa que resuelve el problema de la mochila mediante un algoritmo de backtracking.
  Dados n objetos, cada uno con un peso y un valor, se busca determinar la combinación de objetos que
  maximice el valor total sin exceder la capacidad de la mochila.
  
  Complejidad: O(2^n) 

  Observaciones: Utiliza backtracking para explorar el espacio de soluciones y registra la mejor
  combinación encontrada usando variables globales.
  
  Compilación: Windows / Linux: gcc mochila01_listado.c -o listadoMochila01
  
  Ejecución: ./listadoMochila01 [peso] [num_objetos] < datos.txt
            Donde {peso} es la capacidad máxima de la mochila y {num_objetos} es el número de objetos
            datos.txt contiene en las primeras n líneas los pesos y en las siguientes n líneas los valores
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Variables globales para almacenar la mejor solución encontrada
int maxValorGlobal = 0;                 // Almacena el valor máximo encontrado
int *mejorSolucion = NULL;              // Arreglo que almacena la mejor combinación de objetos (se inicializará en main)

// Prototipos de funciones
void mochilaBacktracking(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual, int *solActual);

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresados al programa y char *arg_user[] 
como un arreglo que almacena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente, 1 si hay algún error
Observaciones: Función principal que valida los argumentos, crea los arreglos necesarios,
invoca el algoritmo de backtracking y libera la memoria al finalizar.
*/
int main(int num_arg, char *arg_user[]) {
    // Verificar argumentos
    if (num_arg != 3) {
        printf("\nIndique el peso maximo de la mochila y la cantidad de elementos - Ejemplo: %s K n < datos.txt\n", arg_user[0]);
        exit(1);
    }
    
    // Obtener peso máximo de la mochila (K) y número de elementos (n)
    int pesoMaximo = atoi(arg_user[1]);
    int n = atoi(arg_user[2]);
    
    // Reservar memoria para los arreglos de pesos y valores
    int *pesos = malloc(n * sizeof(int));
    int *valores = malloc(n * sizeof(int));
    
    // Verificar si la asignación de memoria fue exitosa
    if (pesos == NULL || valores == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    
    // Leer pesos y valores desde la entrada estándar
    for (int i = 0; i < n; i++) {
        scanf("%d", &pesos[i]);
    }
    
    for (int i = 0; i < n; i++) {
        scanf("%d", &valores[i]);
    }
    
    // Mostrar datos de entrada
    printf("\nDatos de entrada:\n");
    printf("Peso maximo de la mochila (K): %d\n", pesoMaximo);
    printf("Cantidad de elementos (n): %d\n", n);
    printf("Pesos y valores de los objetos:\n");
    for (int i = 0; i < n; i++) {
        printf("Objeto %d: peso = %d, valor = %d\n", i+1, pesos[i], valores[i]);
    }
    printf("\n");

    // Reservar memoria para los arreglos que contendrán las soluciones
    int *solActual = malloc(n * sizeof(int));
    mejorSolucion = malloc(n * sizeof(int));
    if (solActual == NULL || mejorSolucion == NULL) {
        printf("Error: No se pudo asignar memoria para la solucion\n");
        exit(1);
    }
    // Inicializar las soluciones con ceros (ningún objeto seleccionado)
    for (int i = 0; i < n; i++) {
        solActual[i] = 0;
        mejorSolucion[i] = 0;
    }
    
    // Llamamos a la función para resolver el problema de la mochila
    mochilaBacktracking(pesos, valores, n, 0, pesoMaximo, 0, solActual);
    
    // Imprimimos el resultado
    printf("\nLa maxima ganancia posible es: %d\n", maxValorGlobal);
    printf("La mejor solucion es:\n");
    for (int i = 0; i < n; i++) {
        if (mejorSolucion[i] == 1)
            printf("Objeto %d incluido (peso = %d, valor = %d)\n", i+1, pesos[i], valores[i]);
    }
    
    // Liberar toda la memoria asignada
    free(pesos);
    free(valores);
    free(solActual);
    free(mejorSolucion);
    
    return 0;
}

/* 
void mochilaBacktracking(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual, int *solActual)
Recibe: int *pesos como arreglo de pesos, int *valores como arreglo de valores, n como número total de objetos,
objActual como índice del objeto que se está evaluando, capacidadRestante como capacidad disponible en la mochila,
valorActual como valor acumulado hasta el momento, *solActual como arreglo que almacena la solución actual
Devuelve: void (No retorna valor explícito, actualiza las variables globales maxValorGlobal y mejorSolucion)
Observaciones: Función recursiva que implementa el algoritmo de backtracking, evaluando todas las posibles
combinaciones de objetos y actualizando la mejor solución encontrada.
*/
void mochilaBacktracking(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual, int *solActual) {
    // Caso base: se han evaluado todos los objetos
    if (objActual == n) {
        // Si la solución actual es mejor que la mejor encontrada hasta ahora
        if (valorActual > maxValorGlobal) {
            maxValorGlobal = valorActual;
            // Actualizamos la mejor solución encontrada
            for (int i = 0; i < n; i++)
                mejorSolucion[i] = solActual[i];
        }
        return;
    }
    
    // Opción 1: No incluir el objeto actual
    solActual[objActual] = false;
    mochilaBacktracking(pesos, valores, n, objActual + 1, capacidadRestante, valorActual, solActual);
    
    // Opción 2: Incluir el objeto actual (si cabe en la mochila)
    if (pesos[objActual] <= capacidadRestante) {
        solActual[objActual] = true;
        mochilaBacktracking(pesos, valores, n, objActual + 1, capacidadRestante - pesos[objActual], valorActual + valores[objActual], solActual);
        // Backtracking: desmarcamos el objeto después de la llamada recursiva
        solActual[objActual] = false;
    }
}