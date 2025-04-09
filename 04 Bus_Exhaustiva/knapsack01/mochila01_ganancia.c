#include <stdio.h>
#include <stdlib.h>

// Función que resuelve el problema de la mochila mediante búsqueda exhaustiva
int mochilaExhaustiva(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual);

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
    
    // Llamamos a la función para resolver el problema de la mochila
    int maxValue = mochilaExhaustiva(pesos, valores, n, 0, pesoMaximo, 0);
    
    // Imprimimos el resultado
    printf("\nLa máxima ganancia posible es: %d\n", maxValue);
    
    // Liberar memoria asignada
    free(pesos);
    free(valores);
    
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
int mochilaExhaustiva(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual) 
{
    // Caso base: si ya hemos considerado todos los objetos
    if(objActual == n) {
        return valorActual;
    }
    
    // Si el objeto actual no cabe en la mochila, lo excluimos
    if(pesos[objActual] > capacidadRestante) {
        return mochilaExhaustiva(pesos, valores, n, objActual + 1, capacidadRestante, valorActual);
    }
    
    // Evaluamos dos opciones: incluir el objeto actual o excluirlo
    
    // Opción 1: Excluir el objeto actual
    int valorExcluido = mochilaExhaustiva(pesos, valores, n, objActual + 1, capacidadRestante, valorActual);
    
    // Opción 2: Incluir el objeto actual
    int valorIncluido = mochilaExhaustiva(pesos, valores, n, objActual + 1, capacidadRestante - pesos[objActual], valorActual + valores[objActual]);
    
    // Devolvemos el mayor valor entre las dos opciones
    if(valorExcluido > valorIncluido) {
        return valorExcluido;
    } else {
        return valorIncluido;
    }
}