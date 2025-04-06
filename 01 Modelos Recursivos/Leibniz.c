/*
AUTOR: Aldo Garcia Ambrosio (C) Marzo 2025
Autor origina: Uso de IA (GPT o4, Copilot, ClaudeAI) -
VERSIÓN: 1

DESCRIPCIÓN: Algoritmo que devuelve el valor de pi con base a la serie de Leibniz y un numero 'n' ingresado por el usuario.

OBSERVACIONES: Se tomara la estructura Main de los programas anteriores para la validación de argumentos.

COMPILACIÓN: gcc .\Leibniz.c -o Leibniz

EJECUCIÓN: ./Leibniz (Linux / Mac) - Leibniz.exe (Windows)
*/
#include <stdio.h>
#include <stdlib.h>

double Leibniz(int numUser, int i) {
    int signo;
    // Caso base: si llegamos al número de términos
    if (i >= numUser)
        return 0;
    
    // Cálculo del término actual
    int denominador = 2 * i + 1;
    
    if(i % 2 == 0){
        signo = 1;
    }else{
        signo = -1;
    }
    
    double termino_actual = signo * (1.0 / denominador);
    
    // Llamada recursiva con el siguiente índice
    return termino_actual + Leibniz(numUser, i + 1);
    
}

// Función principal
int main(int num_arg, char *arg_user[]) {
    // Validación del número de argumentos
    if (num_arg != 2) {
        printf("Uso: %s <numero_de_terminos>\n", arg_user[0]);
        exit(1);
    }

    // Conversión de argumentos a enteros
    int num1 = atoi(arg_user[1]);

    //Llamada a la función recursiva
    double pi = 4 * Leibniz(num1, 0);

    // Mostrar el resultado
    printf("El valor de pi con base a %d es %lf\n", num1, pi);

    return 0;
}

