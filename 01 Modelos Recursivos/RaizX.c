/*
AUTOR: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Uso de IA (GPT o4, Copilot) -
VERSIÓN: 1.3

DESCRIPCIÓN: Algoritmo que devuelve la raiz cuadrada de un número ingresado por el usuario n veces, ademas de devolver el procenate de aproximacion a la raiz.

OBSERVACIONES: Se tomara la estructura Main de los programas anteriores para la validación de argumentos.

COMPILACIÓN: gcc .\RaizX.c -o Raiz 

EJECUCIÓN: ./Raiz (Linux / Mac) - Raiz.exe (Windows)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double raizRecursiva(double x, int n, int iterActual) {

    if (x < 0) {
        printf("Error: Raíz negativa.\n");
        return 0;
    }
    
    if (x == 0 || x == 1) {
        return x; // La raíz de 0 es 0, y la raíz de 1 es 1
    }
    
    // Caso base cuando llegamos al número de iteraciones solicitado
    if (iterActual >= n) {
        return 1.0;
    }
    
    // Uso de formula 
    double subRaiz = raizRecursiva(x, n, iterActual + 1);
    return 1.0 + (x - 1.0) / (1.0 + subRaiz);
}


// Funcion principal
int main(int num_arg, char *arg_user[]) {
    // Validacion del numero de argumentos
    if (num_arg != 3)
    {
        printf("Uso: %s <numero>\n", arg_user[0]);
        exit(1);
    }
    // Conversion de argumento a entero
    int valor_X = atoi(arg_user[1]);
    int num = atoi(arg_user[2]);

    // Validar entradas
    if (num < 1) {
        printf("El número de iteraciones debe ser al menos 1.\n");
        return 1;
    }
    
    double porcentajeAprox = ((double)(raizRecursiva(valor_X, num, 0)) / sqrt(valor_X)) * 100;
    
    printf("Numero X: %d\n", valor_X);
    printf("Iteraciones: %d\n", num);
    printf("Aproximacion calculada: %.10f\n", (double)(raizRecursiva(valor_X, num, 0)));
    printf("Valor real de la raiz: %.10f\n", (double)(sqrt(valor_X)));
    printf("Porcentaje de aproximacion: %.2f%%\n", porcentajeAprox);
    
    return 0;
}