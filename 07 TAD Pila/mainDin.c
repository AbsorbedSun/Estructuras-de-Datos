/*  
  mainDin.c 
  Autor: Aldo Garcia Ambrosio (C) Mayo 2025
  Uso de IA: Claude 3.7 Sonnet
  Version: 1.6
  
  Programa que implementa un sistema de gestión de mensajes utilizando el TDA Pila
  con operaciones extendidas Element() y Flip(). Permite al usuario ingresar hasta 5
  mensajes, consultar uno específico mediante su posición relativa al tope, y visualizar
  todos los mensajes en diferentes órdenes (temporal y cronológico).
  
  Observaciones: El programa demuestra la versatilidad del TDA Pila y su capacidad para
  resolver problemas prácticos cuando se extiende con operaciones adicionales. Se puede
  configurar para utilizar tanto la implementación dinámica como la estática del TDA Pila,
  comentando/descomentando las líneas correspondientes de los archivos de inclusión.
  
  Compilación: Windows / Linux: gcc mainDin.c pila_dinamica/pila_din.c -o dinamica
                                gcc mainDin.c pila_estatica/pila_est.c -o estatica (dependiendo de la pila que se use)
  
  Ejecución: ./dinamica  
             ./estatica
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pila_dinamica/pila_din.h"
//#include "pila_estatica/pila_est.h"

/*
int main()
Función principal que implementa un sistema de gestión de mensajes usando pilas.
Recibe: Ningún parámetro.
Devuelve: 0 si el programa termina correctamente.
Observaciones: Esta función permite al usuario ingresar hasta 5 mensajes que se almacenan 
en una pila, consultar cualquier mensaje mediante Element() sin modificar la estructura,
y mostrar los mensajes en diferentes órdenes (temporal y cronológico) usando Flip().
*/
int main() {
    // Pilas para almacenar los mensajes original y copia
    pila mensajes, copia;                
    Initialize(&mensajes);
    Initialize(&copia); 
    // Elemento auxiliar para operaciones de pila
    elemento e;                          
    // Variable para almacenar cada mensaje ingresado
    char temporal[100];                  
    // Variables para control de índices y selección
    int i = 0, opc;
    
    printf("Escribe hasta 5 mensajes:\n");
    for (i; i < 5; i++) {
        printf("Mensaje %d: ", i + 1); 
        fgets(temporal, sizeof(temporal), stdin);
        if (temporal[0] == '\n') break;  // Salir si se ingresa una línea vacía
        strcpy(e.simbolo, temporal); // Copia el mensaje al elemento de la pila
        Push(&mensajes, e); // Apila el mensaje en la estructura
    }
    
    // Crear una copia de la pila original antes de vaciarla
    pila temp;
    Initialize(&temp);
    
    // Copiamos mensajes a temp (invertirá el orden)
    while (!Empty(&mensajes)) {
        e = Pop(&mensajes); // Extrae el mensaje del tope
        Push(&temp, e); // Lo apila en la pila temporal
        Push(&copia, e); // Guardamos en copia también para uso posterior
    }
    
    // Restauramos mensajes (ahora en orden inverso)
    while (!Empty(&temp)) {
        e = Pop(&temp); // Extrae desde la pila temporal
        Push(&mensajes, e); // Restaura en la pila original
    }
    
    printf("\nEscriba el numero de mensaje que desee volver a ver:");
    scanf("%d", &opc);
    // Usa Element() para acceder sin modificar la pila
    elemento buscar = Element(&mensajes, opc);  
    printf("El %d° mensaje mas reciente: %s", opc, buscar.simbolo);
    
    printf("\n--- Mensajes en orden temporal (mas antiguo a mas reciente)---\n");
    while (!Empty(&mensajes)) {
        e = Pop(&mensajes); // Extrae y elimina cada mensaje
        printf("Mensaje: %d %s", (i), e.simbolo); 
        i--; // Actualiza el contador descendente
    }
    
    // Invierte el orden de los elementos
    Flip(&mensajes);                     
    
    // Imprimir en orden cronológico
    printf("\n--- Mensajes en orden cronologico ---\n");
    while (!Empty(&copia)) {
        e = Pop(&copia); // Extrae cada mensaje de la copia
        printf("Mensaje: %d %s", (i+1), e.simbolo);  
        i++; // Actualiza el contador ascendente
    }

    // Libera la memoria de la pila
    Destroy(&mensajes);
    Destroy(&copia); 
    Destroy(&temp);                      
    return 0;               
}