/*
AUTOR: Edgardo Adrián Franco Martínez (C) Septiembre 2024
VERSIÓN: 1.5

DESCRIPCIÓN: Ejemplo clásico de la solución recursiva para el problema de las Torres de Hanoi, 
donde se utilizan tres postes para mover una serie de discos de un poste de origen a un poste de destino, 
usando un poste auxiliar y siguiendo ciertas reglas.

OBSERVACIONES: El problema resuelto es un ejemplo común de la aplicación de la recursión. 
El número de movimientos mínimos necesarios para resolver el problema con 'n' discos es 2^n - 1.

COMPILACIÓN: gcc -o hanoi hanoi.c

EJECUCIÓN: ./hanoi (En Linux o Mac) - hanoi.exe (En Windows)
*/

#include <stdio.h>

void torresDeHanoi(int n, char origen, char destino, char auxiliar) {
    // Si hay solo un disco, muévelo de origen a destino y detén la recursión
    if (n == 1) {
        printf("Mover disco 1 de %c a %c\n", origen, destino);
        return;
    }

    // Mueve n-1 discos de origen a auxiliar usando destino como auxiliar
    torresDeHanoi(n - 1, origen, auxiliar, destino);

    // Mueve el disco restante de origen a destino
    printf("Mover disco %d de %c a %c\n", n, origen, destino);

    // Mueve n-1 discos de auxiliar a destino usando origen como auxiliar
    torresDeHanoi(n - 1, auxiliar, destino, origen);
}

int main() {
    int n = 3; // Número de discos
    torresDeHanoi(n, 'A', 'B', 'C'); // A es el origen, C es el destino y B es el auxiliar

    return 0;
}
