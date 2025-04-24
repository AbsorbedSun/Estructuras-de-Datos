/*  
  nReinas.c (Problema de las N Reinas con Backtracking)
  Autor: Aldo Garcia Ambrosio (C) Abril 2025
  Uso de IA: Claude 3.7 Sonnet
  Version: 1.9
  
  Programa que resuelve el problema de las N Reinas utilizando la técnica de backtracking.
  El problema consiste en colocar N reinas en un tablero de ajedrez de N×N sin que se amenacen.
  
  Complejidad: O(n!) en el peor caso.
  
  Observaciones: El programa utiliza asignación dinámica de memoria para crear un tablero
  de tamaño variable según el parámetro de entrada.
  
  Compilación: Windows / Linux: gcc nReinas.c -o nReinas
  
  Ejecución: ./nReinas {n}
            Donde n es el número de reinas y tamaño del tablero
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Prototipos de funciones
bool validarPosicion(int **tablero, int fila, int columna, int n);
bool nReinasBacktracking(int **tablero, int columna, int n);
void imprimirTablero(int **tablero, int n);

/*
int main(int argc, char *argv[])
Recibe: int argc como el total de argumentos ingresados al programa y char *argv[] 
como un arreglo que almacena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente, 1 si hay algún error
Observaciones: Función principal que valida los argumentos, crea el tablero,
llama el algoritmo de backtracking y libera la memoria al finalizar.
*/
int main(int argc, char *argv[]) {
    // Verificar que se haya proporcionado el número correcto de argumentos
    if (argc != 2) {
        printf("Uso: %s <n>\n", argv[0]);
        return 1;
    }

    // Convertir el argumento a un entero
    int n = atoi(argv[1]);
    
    // Validar que n sea un número positivo
    if (n <= 0) {
        printf("El numero de reinas debe ser un entero positivo.\n");
        return 1;
    }

    // Crear tablero dinámico de n x n
    int **tablero = (int **)malloc(n * sizeof(int *));
    if (tablero == NULL) {
        printf("Error de memoria al crear el tablero.\n");
        return 1;
    }
    
    // Inicializar cada fila del tablero con ceros
    for (int i = 0; i < n; i++) {
        tablero[i] = (int *)malloc(n * sizeof(int));
        if (tablero[i] == NULL) {
            printf("Error de memoria al crear el tablero.\n");
            // Liberar la memoria ya asignada
            for (int j = 0; j < i; j++) {
                free(tablero[j]);
            }
            free(tablero);
            return 1;
        }
        for (int j = 0; j < n; j++) {
            tablero[i][j] = 0;
        }
    }

    // Resolver el problema de las N Reinas usando backtracking
    if (nReinasBacktracking(tablero, 0, n)) {
        // Imprimir el tablero si se encuentra una solución
        printf("Solucion encontrada:\n\n");
        imprimirTablero(tablero, n);
    } else {
        printf("No se encontro solucion\n");
    }

    // Liberar memoria del tablero
    for (int i = 0; i < n; i++) {
        free(tablero[i]);
    }
    free(tablero);

    return 0;
}

/* 
bool validarPosicion(int **tablero, int fila, int columna, int n)
Recibe: int **tablero como matriz del tablero, fila y columna como posición a validar, n como tamaño del tablero
Devuelve: bool que indica si es seguro colocar una reina en la posición (fila, columna)
Observaciones: Verifica si no hay reinas en la misma fila, columna o en las diagonales que amenacen la posición.
*/
bool validarPosicion(int **tablero, int fila, int columna, int n) {
    // Verificar si hay reinas en la misma fila (hacia la izquierda)
    for (int i = 0; i < columna; i++) {
        if (tablero[fila][i] == 1)
            return false;
    }
    
    // Verificar diagonal superior izquierda
    for (int i = fila, j = columna; i >= 0 && j >= 0; i--, j--) {
        if (tablero[i][j] == 1)
            return false;
    }
    
    // Verificar diagonal inferior izquierda
    for (int i = fila, j = columna; i < n && j >= 0; i++, j--) {
        if (tablero[i][j] == 1)
            return false;
    }
    
    // Si no hay reinas que amenacen, la posición es válida
    return true;
}

/* 
bool nReinasBacktracking(int **tablero, int columna, int n)
Recibe: int **tablero como matriz del tablero, columna actual a procesar, n como tamaño del tablero
Devuelve: bool que indica si se encontró una solución
Observaciones: Función recursiva que implementa el algoritmo de backtracking para resolver
el problema de las N Reinas, intentando colocar una reina en cada columna.
*/
bool nReinasBacktracking(int **tablero, int columna, int n) {
    // Caso base: si todas las reinas están colocadas (se ha llegado a la última columna + 1)
    if (columna >= n)
        return true;
    
    // Intentar colocar una reina en cada fila de la columna actual
    for (int fila = 0; fila < n; fila++) {
        // Verificar si es seguro colocar una reina en la posición (fila, columna)
        if (validarPosicion(tablero, fila, columna, n)) {
            // Colocar la reina (marcar con 1)
            tablero[fila][columna] = 1;
            // Imprimir el tablero después de colocar la reina
            imprimirTablero(tablero, n);

            // Recurrencia para colocar el resto de reinas en las columnas siguientes
            if (nReinasBacktracking(tablero, columna + 1, n)){
                return true;
            }
            // Si colocar la reina en la posición (fila, columna) no lleva a una solución
            // entonces retroceder (backtrack) y quitar la reina (marcar con 0)
            tablero[fila][columna] = 0;
            // Imprimir el tablero después de quitar la reina
            imprimirTablero(tablero, n);
        }
    }
    
    // Si no se puede colocar una reina en ninguna fila de la columna actual
    return false;
}

/* 
void imprimirTablero(int **tablero, int n)
Recibe: int **tablero como matriz del tablero y n como tamaño del tablero
Devuelve: void (No retorna valor explícito)
Observaciones: Imprime la configuración del tablero, donde 1 representa una reina y 0 representa una casilla vacía.
*/
void imprimirTablero(int **tablero, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tablero[i][j] == 1)
                printf("R "); // R representa una reina
            else if(tablero[i][j] == 0)
                printf("- "); // . representa una casilla vacía
        }
        printf("\n");
    }
    printf("\n");
}