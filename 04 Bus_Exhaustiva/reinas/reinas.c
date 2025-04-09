#include <stdio.h>
#include <stdlib.h>

void imprimirTablero(int **tablero, int n) {
    printf("\nTablero evaluado:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int validarPosicion(int **tablero, int fila, int columna, int n) {
    // Verificar fila
    for (int i = 0; i < columna; i++) {
        if (tablero[fila][i] == 1)
            return 0;
    }

    // Diagonal superior izquierda
    for (int i = fila, j = columna; i >= 0 && j >= 0; i--, j--) {
        if (tablero[i][j] == 1)
            return 0;
    }

    // Diagonal inferior izquierda
    for (int i = fila, j = columna; i < n && j >= 0; i++, j--) {
        if (tablero[i][j] == 1)
            return 0;
    }

    return 1;
}

int nReinasBacktracking(int **tablero, int columna, int n) {
    if (columna >= n)
        return 1;

    for (int fila = 0; fila < n; fila++) {
        if (validarPosicion(tablero, fila, columna, n)) {
            tablero[fila][columna] = 1;
            imprimirTablero(tablero, n);

            if (nReinasBacktracking(tablero, columna + 1, n))
                return 1;

            // Backtrack
            tablero[fila][columna] = 0;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    // Crear tablero dinámico de n x n
    int **tablero = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        tablero[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            tablero[i][j] = 0;
        }
    }

    // Resolver problema
    if (nReinasBacktracking(tablero, 0, n)) {
        printf("¡Solución encontrada!\n");
        imprimirTablero(tablero, n);
    } else {
        printf("No se encontró solución.\n");
    }

    // Liberar memoria
    for (int i = 0; i < n; i++)
        free(tablero[i]);
    free(tablero);

    return 0;
}
