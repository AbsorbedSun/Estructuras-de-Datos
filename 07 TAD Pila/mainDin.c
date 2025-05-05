#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include "pila_dinamica/pila_din.h"
#include "pila_estatica/pila_est.h"

int main() {
    pila mensajes, copia;
    Initialize(&mensajes);
    Initialize(&copia);
    elemento e;
    char temporal[100];
    int i = 0, opc;

    printf("Escribe hasta 5 mensajes:\n");
    for (i; i < 5; i++) {
        printf("Mensaje %d: ", i + 1);
        fgets(temporal, sizeof(temporal), stdin);
        if (temporal[0] == '\n') break; // Salir si se ingresa una línea vacía
        strcpy(e.simbolo, temporal);
        Push(&mensajes, e);
    }

    // Crear una copia de la pila original antes de vaciarla
    pila temp;
    Initialize(&temp);

    // Copiamos mensajes a temp (invertirá el orden)
    while (!Empty(&mensajes)) {
        e = Pop(&mensajes);
        Push(&temp, e);
        Push(&copia, e);  // Guardamos en copia también
    }

    // Restauramos mensajes (ahora en orden inverso)
    while (!Empty(&temp)) {
        e = Pop(&temp);
        Push(&mensajes, e);
    }

    printf("\nEscriba el numero de mensaje que desee volver a ver:");
    scanf("%d", &opc);
    elemento buscar = Element(&mensajes, opc);
    printf("El %d° mensaje mas reciente: %s", opc, buscar.simbolo);

    printf("\n--- Mensajes en orden temporal (mas antiguo a mas reciente)---\n");
    while (!Empty(&mensajes)) {
        e = Pop(&mensajes);
        printf("Mensaje: %d %s", (i),  e.simbolo);
        i--;
    }

    // Dar la vuelta a la pila
    Flip(&mensajes);
    
    // Imprimir en orden cronológico
    printf("\n--- Mensajes en orden cronologico ---\n");
    while (!Empty(&copia)) {
        e = Pop(&copia);
        printf("Mensaje: %d %s", (i+1),  e.simbolo);
        i++;
    }

    Destroy(&mensajes);
    Destroy(&copia);
    Destroy(&temp);
    return 0;
}