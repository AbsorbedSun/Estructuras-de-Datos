#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*Implementacion de una funcion recurrente
Autores: Aldo Garcia Ambrosio (C) Marzo 2025
         Omar Ivan Salinas Aquino (C) Marzo 2025
Version: 1.0

Descripcion: El programa realiza la implementacion de una funcion recursiva
Observaciones: No valida que el dato ingresado sea entero, si se ingresa un numero que no pertenezca al conjunto de los numeros enteros podria generar un error.

Compilacion: gcc -o ecuacionR ecuacionR.c
Ejecucion: ./ecuacionR <numero>
*/

// Función recursiva
int funcionRecurrente(int n){

    // Caso base
    if(n == 0){
        return 1;
    }

    return funcionRecurrente(n - 1) + pow(2, n);

}


int main(int num_arg, char *arg_user[]) {
    // Validación del número de argumentos
    if (num_arg != 2) {
        exit(1);
    }
    // Conversión de argumentos a enteros
    int subA = atoi(arg_user[1]);

    // Validación de la entrada
    if(subA < 0){
        return 0;
    }

    // Mostrar el resultado
    printf("El resultado de la ecuacion es: %d\n", funcionRecurrente(subA));

    return 0;
}