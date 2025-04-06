/*
AUTOR: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Uso de IA (GPT o4, Copilot) -
VERSIÓN: 1.4

DESCRIPCIÓN: Algoritmo que devuelve el modelo de Fibonacci de un número 'n' ingresado por el usuario.

OBSERVACIONES: Se tomara la estructura Main de los programas anteriores para la validación de argumentos.

COMPILACIÓN: gcc .\Fibonacci.c -o  FibonacciR

EJECUCIÓN: ./FibonacciR (Linux / Mac) - FibonacciR.exe (Windows)
*/

#include <stdio.h>
#include <stdlib.h>

int fibonacciR(int numFib) {
    // Caso base
    if (numFib == 0)
        return 0;
    if (numFib == 1)
        return 1;
    
    return fibonacciR(numFib - 1) + fibonacciR(numFib - 2);
}

// Funcion principal
int main(int num_arg, char *arg_user[]) {
    // Validacion del numero de argumentos
    if (num_arg != 2)
    {
        printf("Uso: %s <numero>\n", arg_user[0]);
        exit(1);
    }
    // Conversion de argumento a entero
    int num_fibonacci = atoi(arg_user[1]);

    printf("El termino %d de la secuencia de Fibonacci es:\n", num_fibonacci);
    for (int i = 0; i < num_fibonacci; i++) {
        printf("%d ", fibonacciR(i));
    }
    
    return 0;
}