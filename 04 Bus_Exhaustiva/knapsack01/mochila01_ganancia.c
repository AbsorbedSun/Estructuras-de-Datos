/*  
  mochilaExhaustiva.c
  Autor: Aldo Garcia Ambrosio (C) Abril 2025
  Uso de IA: Claude 3.7 Sonnet
  Version: 1.3
  
  Programa que resuelve el problema  de la mochila mediante un algoritmo de búsqueda exhaustiva.
  Dados n objetos, cada uno con un peso y un valor, se busca determinar la combinación de objetos que
  maximice el valor total sin exceder la capacidad de la mochila.
  
  Complejidad: O(2^n)
  
  Observaciones: Utiliza recursividad para evaluar todas las posibles combinaciones de objetos.
  
  Compilación: Windows / Linux: gcc mochila01_ganancia.c -o gananciaMochila01
  
  Ejecución: ./gananciaMochila01 [peso] [num_objetos] < datos.txt
            Donde {peso} es la capacidad máxima de la mochila y {num_objetos} es el número de objetos
            datos.txt contiene en las primeras n líneas los pesos y en las siguientes n líneas los valores
*/

#include <stdio.h>
#include <stdlib.h>

// Prototipos de funciones
int mochilaExhaustiva(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual);

/*
int main(int num_arg, char *arg_user[])
Recibe: int num_arg como el total de argumentos ingresados al programa y char *arg_user[] 
como un arreglo que almacena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente, 1 si hay algún error
Observaciones: Función principal que valida los argumentos, crea los arreglos de pesos y valores,
invoca el algoritmo de búsqueda exhaustiva y libera la memoria al finalizar.
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
    
    // Llamamos a la función para resolver el problema de la mochila
    int maxValue = mochilaExhaustiva(pesos, valores, n, 0, pesoMaximo, 0);
    
    // Imprimimos el resultado
    printf("\nLa maxima ganancia posible es: %d\n", maxValue);
    
    // Liberar memoria asignada
    free(pesos);
    free(valores);
    
    return 0;
}

/* 
int mochilaExhaustiva(int *pesos, int *valores, int n, int objActual, int capacidadRestante, int valorActual)
Recibe: int *pesos como arreglo de pesos, int *valores como arreglo de valores, n como número total de objetos,
objActual como índice del objeto que se está evaluando, capacidadRestante como capacidad disponible en la mochila,
valorActual como valor acumulado hasta el momento.
Devuelve: int que representa el valor máximo que se puede obtener
Observaciones: Función recursiva que implementa la búsqueda exhaustiva, evaluando todas las posibles
combinaciones de objetos (incluir o no incluir cada objeto).
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