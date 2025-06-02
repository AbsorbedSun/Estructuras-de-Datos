/*  
  TestCola.c 
  Autor: Aldo Garcia Ambrosio (C) Mayo 2025
  Uso de IA: Claude 3.7 Sonnet, ChatGPT 4.0
  Version: 2.3
  
  Programa que implementa un “Sistema de Gestión de Correos Salientes” para una pequeña oficina. 
  Cada vez que un usuario redacta un mensaje, este se encola en la cola de envío; más tarde, podrán 
  buscar un correo concreto por su posición en la cola (por ejemplo, para editarlo antes de enviarlo) 
  o bien invertir el orden de envío—por si deciden primero procesar los mensajes más antiguos

  Observaciones: 
  
  Compilación: Windows / Linux: gcc TestCola.c Recursos/Cola_Dinamica/TADColaDin.c -o dinamica
                                gcc TestCola.c Recursos/Cola_Estatica/TADColaEst.c -o estatica 
                                gcc TestCola.c Recursos/Cola_Estatica/TADColaEstCir.c -o estaticaCir (dependiendo de la pila que se use)
  
  Ejecución: ./dinamica  
             ./estatica
             ./estaticaCir
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Incluir la cabecera apropiada según la versión que quieras probar
//#include "Recursos/Cola_Dinamica/TADColaDin.h"    // Para probar la cola dinámica
//#include "Recursos/Cola_Estatica/TADColaEst.h"    // Para probar la cola estática
#include "Recursos/Cola_Estatica/TADColaEstCir.h" // Para probar la cola estática circular

// Definición prototipo de funciones
void printQueue(cola *q);
elemento peekAt(cola *q, int pos);

/*  
int main(void)
Recibe: void (no requiere argumentos de entrada).
Devuelve: int — 0 si finaliza correctamente, distinto de 0 en caso de error.
Observaciones: Gestiona el flujo completo del programa de correos:
  1. Inicializa la cola.
  2. Pide hasta 5 mensajes al usuario (o “FIN” para detenerse).
  3. Muestra la cola actual con printQueue().
  4. Solicita un ID para buscar y usa peekAt+Search para localizarlo.
  5. Invoca Flip() para invertir el orden de envío.
  6. Vuelve a imprimir la cola invertida y libera recursos antes de salir.
*/
int main(void) {
    cola mailQ;
    Initialize(&mailQ);
    char mensaje[100];
    int count = 0;
    elemento e;

    printf(">>> Sistema de gestion de correos salientes <<<\n");
    while (count < 5) {
        printf("Escribe el mensaje #%d (o 'FIN' para terminar): ", count+1);
        if (!fgets(mensaje, sizeof(mensaje), stdin)) break;
        mensaje[strcspn(mensaje, "\n")] = '\0';  // Eliminar el salto de línea
        
        if (strcmp(mensaje, "FIN") == 0) break;
        
        // Copiamos el mensaje al elemento y asignamos la posición como número
        strcpy(e.texto, mensaje);
        e.numero = count + 1;  // Asignar número de posición como ID
        
        Queue(&mailQ, e);
        count++;
    }

    if (Empty(&mailQ)) {
        printf("No hay mensajes en la cola. Saliendo.\n");
        return 0;
    }

    printf("\nCola actual de envios:\n");
    printQueue(&mailQ);

    /* Prueba de Search */
    printf("\nIngresa el ID de mensaje a buscar (1-%d): ", count);
    int idbuscado;
    if (scanf("%d", &idbuscado) == 1) {
        elemento e_buscar = peekAt(&mailQ, idbuscado);
        int pos = Search(&mailQ, e_buscar);

        if (pos > 0) {
            printf("El mensaje con ID %d esta en la posicion %d de la cola.\n", idbuscado, pos);
        } else {
            printf("No se encontro mensaje con ID %d en la cola.\n", idbuscado);
        }
        
    }

    // Prueba de Flip
    printf("\nInvirtiendo orden de envio con Flip()...\n");
    Flip(&mailQ);

    printf("Cola despues de Flip():\n");
    printQueue(&mailQ);

    // Liberar recursos
    while (!Empty(&mailQ)) {
        Dequeue(&mailQ);
    }

    return 0;
}

/*  
void printQueue(cola *q)
Recibe: cola *q — puntero a la cola que se desea imprimir.
Devuelve: void.
Observaciones: Recorre la cola sin alterarla. Para ello desencola cada elemento,
lo imprime con su índice relativo y lo almacena temporalmente en otra cola.
Al terminar, vuelve a encolar todos los elementos desde la cola auxiliar
para restaurar el estado original de la estructura.
*/
void printQueue(cola *q) {
    if (Empty(q)) {
        printf("  La cola esta vacia\n");
        return;
    }
    
    int n = Size(q);
    cola aux;
    Initialize(&aux);
    elemento e;
    
    // Dequeue todos los elementos, imprimirlos y ponerlos en aux
    for (int i = 0; i < n; i++) {
        e = Dequeue(q);
        printf("  [%d] %s\n", i+1, e.texto);
        Queue(&aux, e);
    }
    
    // Devolver todos los elementos a la cola original
    while (!Empty(&aux)) {
        Queue(q, Dequeue(&aux));
    }
}

/*  
elemento peekAt(cola *q, int pos)
Recibe: cola *q — puntero a la cola de la que extraer el elemento,  
int pos — posición 1-based del elemento deseado.
Devuelve: elemento — el struct que ocupa la posición pos en la cola original.
Observaciones: Realiza un recorrido completo desencolando cada elemento hacia
una cola auxiliar; cuando alcanza la posición pos guarda una copia
en 'resultado'. Al finalizar, restaura todos los elementos a la cola
original, garantizando que la estructura quede intacta.
*/
elemento peekAt(cola *q, int pos) {
    int n = Size(q);
    elemento e, resultado;
    cola aux;
    Initialize(&aux);

    // Sacamos todos los elementos a aux, guardando el que nos interesa
    for (int i = 1; i <= n; i++) {
        e = Dequeue(q);
        if (i == pos) resultado = e;
        Queue(&aux, e);
    }
    // Devolvemos todo de aux a la cola original
    while (!Empty(&aux)) {
        Queue(q, Dequeue(&aux));
    }
    return resultado;
}
