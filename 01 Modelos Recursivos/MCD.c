/*
AUTOR: Aldo Garcia Ambrosio (C) Marzo 2025
}Autor origina: Uso de IA (GPT o4, Copilot) -
VERSIÓN: 1.1

DESCRIPCIÓN: Algoritmo que devuelve el maximal común divisor de dos números ingresados por el usuario.

OBSERVACIONES: Se tomara la estructura Main de los programas anteriores para la validación de argumentos.

COMPILACIÓN: gcc .\MCD.c -o maxComunDiv

EJECUCIÓN: ./maxComunDiv (Linux / Mac) - maxComunDiv.exe (Windows)
*/

#include <stdio.h>
#include <stdlib.h>

int maximoComunDivisor(int num1, int num2) {
    // Algoritmo de Euclides
    if(num2 == 0)
        return num1; // El MCD es el último valor de num1 cuando num2 llega a 0

    return maximoComunDivisor(num2, num1 % num2);  // El MCD es el último valor de num1
}

// Función principal
int main(int num_arg, char *arg_user[]) {
    // Validación del número de argumentos
    if (num_arg != 3) {
        printf("Uso: %s <n1> <n2>\n", arg_user[0]);
        exit(1);
    }

    // Conversión de argumentos a enteros
    int num1 = atoi(arg_user[1]);
    int num2 = atoi(arg_user[2]);

    // Mostrar el resultado
    printf("El máximo común divisor de %d y %d es %d\n", num1, num2, maximoComunDivisor(num1, num2));

    return 0;
}
