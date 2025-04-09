#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Variables globales para almacenar la mejor solución encontrada
int maxValorGlobal = 0;
int *mejorSolucion = NULL; // Se inicializará en main

// Función que resuelve el problema de la mochila mediante búsqueda exhaustiva
void mochilaBacktracking(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual, int *solActual);

int main(int num_arg, char *arg_user[]) {
    // Verificar argumentos
    if (num_arg != 3) {
        printf("\nIndique el peso maximo de la mochila y la cantidad de elementos - Ejemplo: %s K n < datos.txt\n", arg_user[0]);
        exit(1);
    }
    
    // Obtener peso máximo de la mochila (K) y número de elementos (n)
    int pesoMaximo = atoi(arg_user[1]);
    int n = atoi(arg_user[2]);
    
    // Reservar memoria para los arreglos
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
        printf("Error: No se pudo asignar memoria para la solución\n");
        exit(1);
    }
    // Inicializar las soluciones
    for (int i = 0; i < n; i++) {
        solActual[i] = 0;
        mejorSolucion[i] = 0;
    }
    
    // Llamamos a la función para resolver el problema de la mochila
    mochilaBacktracking(pesos, valores, n, 0, pesoMaximo, 0, solActual);
    
    // Imprimimos el resultado
    printf("\nLa maxima ganancia posible es: %d\n", maxValorGlobal);
    printf("La mejor solución es:\n");
    for (int i = 0; i < n; i++) {
        if (mejorSolucion[i] == 1)
            printf("Objeto %d incluido (peso = %d, valor = %d)\n", i+1, pesos[i], valores[i]);
    }
    
    // Liberar memorias
    free(pesos);
    free(valores);
    free(solActual);
    free(mejorSolucion);
    
    return 0;
}

/*
La función mochilaExhaustiva realiza una búsqueda exhaustiva para resolver el problema de la mochila.
Evalúa todas las posibles combinaciones de objetos y devuelve el valor máximo que se puede obtener
sin exceder la capacidad de la mochila.

Parámetros:
- pesos: arreglo con los pesos de los objetos
- valores: arreglo con los valores de los objetos
- n: número total de objetos
- objActual: índice del objeto actual que se está considerando
- capacidadRestante: capacidad restante de la mochila
- valorActual: valor acumulado hasta el momento

Retorno:
- El valor máximo que se puede obtener sin exceder la capacidad de la mochila
*/
void mochilaBacktracking(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual, int *solActual) {
    // Caso base: se han evaluado todos los objetos
    if (objActual == n) {
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