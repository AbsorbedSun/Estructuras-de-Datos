/*
AUTOR: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Uso de IA (GPT o4, Copilot) -
VERSIÓN: 1.3

DESCRIPCIÓN: Algoritmo que devuelve el factorial de un número 'n' ingresado por el usuario.

OBSERVACIONES: Se tomara la estructura Main de los programas anteriores para la validación de argumentos.

COMPILACIÓN: gcc .\Factorial.c -o Factorial

EJECUCIÓN: ./Factorial (Linux / Mac) - Factorial.exe (Windows)
*/

#include <stdio.h>
#include <stdlib.h>

int factorialR(int factorial) {
    // Caso base Factoial de 0 es 1
    if (factorial == 0)
        return 1;
    
    return factorial * factorialR(factorial - 1);
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
    int num_factorial = atoi(arg_user[1]);

    printf("El factorial de %d es %d\n", num_factorial, factorialR(num_factorial));
    
    return 0;
}