/*
AUTOR: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Uso de IA (GPT o4, Copilot) -
VERSIÓN: 1.3

DESCRIPCIÓN: Algoritmo que devuelve el numero de combinaciones posible de un numero 'n' y 'r' ingresado por el usuario.

OBSERVACIONES: Se tomara la estructura Main de los programas anteriores para la validación de argumentos.

COMPILACIÓN: gcc .\Combinatoria.c -o  CombinacionR

EJECUCIÓN: ./CombinacionR (Linux / Mac) - CombinacionR.exe (Windows)
*/

#include <stdio.h>
#include <stdlib.h>

int combinacionR(int num_total, int num_seleccion) {
    if (num_seleccion == 0 || num_seleccion == num_total) 
        return 1; // Casos base

    return combinacionR(num_total - 1, num_seleccion - 1) + combinacionR(num_total - 1, num_seleccion);
}

// Funcion principal
int main(int num_arg, char *arg_user[]) {
    // Validacion del numero de argumentos
    if (num_arg != 3)
    {
        printf("Uso: %s <n> <r>\n", arg_user[0]);
        exit(1);
    }
    // Conversion de argumento a entero
    int numTotal = atoi(arg_user[1]);
    int numSeleccion = atoi(arg_user[2]);

    if (numTotal < 0 || numSeleccion < 0 || numSeleccion >= numTotal) {
        printf("Error: No se permiten numeros negativos o que el numero de opciones sea mayor al total.\n");
        return 1;
    }

    printf("La combinacion C(%d,%d) es %d\n", numTotal, numSeleccion, combinacionR(numTotal,numSeleccion));
    
    return 0;
}
