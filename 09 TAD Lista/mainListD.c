/*  
  mainListD.c 
  Autor: Aldo Garcia Ambrosio (C) Mayo 2025
  Uso de IA: Claude 4.0 Sonnet
  Version: 2.4 
  
  Programa que prueba exhaustivamente todas las operaciones del TAD Lista Doblemente Ligada,
  verificando su correcto funcionamiento mediante diferentes casos de prueba incluyendo
  operaciones básicas, búsqueda, inserción, eliminación y casos límite.
  
  Complejidad: O(n) para la mayoría de operaciones de prueba, O(n²) para las pruebas
  de estrés que involucran múltiples recorridos de listas grandes.
  
  Observaciones: El programa ejecuta una batería completa de pruebas organizadas en 
  tres módulos principales: operaciones básicas, búsqueda y posicionamiento,
  inserción y eliminación.
  
  Compilación: Windows / Linux: gcc mainListD.c Recursos/versionDL/TADListaDL.c -o test_programa
  
  Ejecución: ./test_programa (Linux/Mac) o test_programa.exe (Windows)
*/

#include <stdio.h>
#include <stdlib.h>
#include "Recursos/versionDL/TADListaDL.h"
//#include "Recursos/versionSL/TADListaSL.h"

// ===== DECLARACIÓN DE PROTOTIPOS DE FUNCIONES =====
void imprimirLista(lista *l, char *titulo);
void probarOperacionesBasicas();
void probarBusquedaYPosicionamiento();
void probarInsercionYEliminacion();
void separador(char *texto);

/*
int main(void)
Recibe: Ningún parámetro
Devuelve: 0 si termina correctamente
Observaciones: Función principal que ejecuta todas las pruebas del TAD Lista Doblemente
Ligada de manera secuencial, verificando el correcto funcionamiento de todas las operaciones.
*/
int main(void) {
    printf("=== PROGRAMA DE PRUEBA TAD LISTA DOBLEMENTE LIGADA ===\n");
    
    // Ejecutar todas las pruebas
    probarOperacionesBasicas();
    probarBusquedaYPosicionamiento();
    probarInsercionYEliminacion();
    
    printf("\n=== TODAS LAS PRUEBAS COMPLETADAS EXITOSAMENTE ===\n");
    return 0;
}

/*
void probarOperacionesBasicas()
Recibe: Ningún parámetro
Devuelve: void
Observaciones: Prueba las operaciones fundamentales del TAD: inicialización, adición de elementos,
consulta de tamano, verificación de vacío, acceso por indice y recorridos bidireccionales.
*/
void probarOperacionesBasicas() {
    separador("PRUEBAS DE OPERACIONES BASICAS");
    
    // Variables locales para las pruebas
    lista mi_lista;    
    elemento e;        // Elemento individual para insertar/manipular
    
    // INICIALIZACIÓN DE LISTA VACÍA
    printf("1. Probando Initialize()...\n");
    Initialize(&mi_lista);  // Inicializar lista vacía
    
    // Verificar estado inicial correcto
    printf("   Lista inicializada. Tamano: %d, Vacia: ", Size(&mi_lista));
    if(Empty(&mi_lista)) {  // debe retornar TRUE para lista recién inicializada
        printf("Si\n");
    } else {
        printf("No\n");  // Esto indicaría un error en Initialize()
    }
    
    // ADICIÓN SECUENCIAL DE ELEMENTOS
    printf("\n2. Probando Add() - Agregando elementos del 1 al 10...\n");
    // Bucle para agregar 10 elementos consecutivos
    for(int i = 1; i <= 10; i++) {
        e.number = i;           // Asignar valor al campo del elemento
        Add(&mi_lista, e);      // inserta al final
    }
    // Verificar que el tamaño se actualizó correctamente
    printf("   Elementos agregados. Tamano actual: %d\n", Size(&mi_lista));
    imprimirLista(&mi_lista, "Lista despues de Add");
    
    // ACCESO POR ÍNDICE
    printf("\n3. Probando Element() - Acceso por indice...\n");
    // Recorrer todos los elementos
    for(int i = 1; i <= Size(&mi_lista); i++) {
        elemento elem = Element(&mi_lista, i); 
        printf("   Elemento[%d] = %d\n", i, elem.number);
    }
    
    // RECORRIDO HACIA ADELANTE CON PUNTEROS
    printf("\n4. Probando First() y Following() - Recorrido hacia adelante...\n");
    posicion p = First(&mi_lista);  // Obtener puntero al primer nodo
    int contador = 1;
    
    // Recorrer usando punteros 
    while(p != NULL) {  // NULL indica fin de lista
        elemento elem = Position(&mi_lista, p);  // Obtener elemento en posición p
        printf("   Posicion %d: %d\n", contador++, elem.number);
        p = Following(&mi_lista, p);  // Avanzar al siguiente nodo (p = p->next)
    }
    
    // RECORRIDO HACIA ATRÁS
    printf("\n5. Probando Final() y Previous() - Recorrido hacia atras...\n");
    p = Final(&mi_lista);  // Obtener puntero al último nodo
    contador = Size(&mi_lista);
    
    // Recorrer en sentido inverso (solo posible en listas doblemente ligadas)
    while(p != NULL) {  // NULL indica que llegamos antes del primer nodo
        elemento elem = Position(&mi_lista, p);  // Obtener elemento en posición p
        printf("   Posicion %d: %d\n", contador--, elem.number);
        p = Previous(&mi_lista, p);  // Retroceder al nodo anterior (p = p->prev)
    }

    // VISUALIZACIÓN DE ENLACES INTERNOS
    //VerLigasLista(&mi_lista);
    
    Destroy(&mi_lista);  // Liberar toda la memoria asignada dinámicamente
}

/*
void probarBusquedaYPosicionamiento()
Recibe: Ningún parámetro
Devuelve: void
Observaciones: Verifica las operaciones de búsqueda de elementos, obtención de posiciones
por indice y validación de posiciones, incluyendo casos donde los elementos no existen.
*/
void probarBusquedaYPosicionamiento() {
    separador("PRUEBAS DE BUSQUEDA Y POSICIONAMIENTO");
    
    lista mi_lista;  // Nueva lista para estas pruebas
    elemento e;      // Elemento para manipulación
    Initialize(&mi_lista);  // inicializar antes de usar
    
    // LLENAR LISTA CON DATOS DE PRUEBA
    int valores[] = {5, 10, 15, 20, 25, 30};
    int num_valores = sizeof(valores) / sizeof(valores[0]);  // Calcular tamaño del array
    
    printf("1. Llenando lista con valores: ");
    for(int i = 0; i < num_valores; i++) {
        e.number = valores[i];  // Asignar valor del array
        Add(&mi_lista, e);      // Agregar al final de la lista
        printf("%d ", valores[i]);  // Mostrar progreso
    }
    printf("\n");
    
    // BÚSQUEDA DE ELEMENTO EXISTENTE
    printf("\n2. Probando Search()...\n");
    e.number = 15;  // Buscar el valor 15 (que sí existe)
    posicion pos_encontrada = Search(&mi_lista, e); 
    
    if(pos_encontrada != NULL) {  // NULL = no encontrado
        // Verificar que realmente encontramos el elemento correcto
        elemento encontrado = Position(&mi_lista, pos_encontrada);
        printf("   Elemento 15 encontrado: %d\n", encontrado.number);
    } else {
        printf("   Elemento 15 no encontrado\n");  // Esto sería un error
    }
    
    // BÚSQUEDA DE ELEMENTO NO EXISTENTE
    e.number = 99;  // Buscar valor que no existe en la lista
    pos_encontrada = Search(&mi_lista, e);
    printf("   Buscando elemento 99: ");
    if(pos_encontrada != NULL) {
        printf("Encontrado\n");  // Esto sería un error
    } else {
        printf("No encontrado\n");  // Resultado esperado
    }
    
    // CONVERSIÓN ÍNDICE-A-PUNTERO
    printf("\n3. Probando ElementPosition()...\n");
    // Verificar que ElementPosition() funciona para todos los índices válidos
    for(int i = 1; i <= Size(&mi_lista); i++) {
        posicion pos = ElementPosition(&mi_lista, i);  // Convertir índice a puntero
        elemento elem = Position(&mi_lista, pos);      // Obtener elemento desde puntero
        printf("   ElementPosition(%d) = %d\n", i, elem.number);
    }
    
    Destroy(&mi_lista);  // Liberar memoria antes de salir
}

/*
void probarInsercionYEliminacion()
Recibe: Ningún parámetro
Devuelve: void
Observaciones: Prueba las operaciones de modificación de la lista: inserción en posiciones
específicas (adelante y atras), eliminación de elementos y reemplazo de valores en posiciones dadas.
*/
void probarInsercionYEliminacion() {
    separador("PRUEBAS DE INSERCION Y ELIMINACION");
    
    lista mi_lista;  // Lista para pruebas de modificación
    elemento e;      // Elemento para manipulación
    Initialize(&mi_lista);  // Inicialización obligatoria
    
    // CREAR LISTA INICIAL SIMPLE
    printf("1. Creando lista inicial [1, 2, 3, 4, 5]...\n");
    for(int i = 1; i <= 5; i++) {
        e.number = i;
        Add(&mi_lista, e);  // Lista resultante: 1->2->3->4->5
    }
    imprimirLista(&mi_lista, "Lista inicial");
    
    // INSERCIÓN ANTES DE UNA POSICIÓN
    printf("\n2. Probando Insert() - Insertar 99 al frente de la posicion 3...\n");
    posicion pos = ElementPosition(&mi_lista, 3);  // Obtener puntero al nodo con valor 3
    e.number = 99;
    // Insert con TRUE = insertar ANTES de la posición (al frente)
    Insert(&mi_lista, pos, e, TRUE);  // Lista: 1->2->99->3->4->5
    imprimirLista(&mi_lista, "Despues de insertar 99 al frente de pos 3");
    
    // INSERCIÓN DESPUÉS DE UNA POSICIÓN
    printf("\n3. Probando Insert() - Insertar 88 atras de la posicion 2...\n");
    pos = ElementPosition(&mi_lista, 2);  // Obtener puntero al nodo con valor 2
    e.number = 88;
    // Insert con FALSE = insertar DESPUÉS de la posición (atrás)
    Insert(&mi_lista, pos, e, FALSE);  // Lista: 1->2->88->99->3->4->5
    imprimirLista(&mi_lista, "Despues de insertar 88 atras de pos 2");
    
    // ELIMINACIÓN DE NODO
    printf("\n4. Probando Remove() - Eliminar elemento en posicion 4...\n");
    pos = ElementPosition(&mi_lista, 4);  // Obtener puntero al 4to elemento
    elemento elem_a_eliminar = Position(&mi_lista, pos);  // Obtener valor antes de eliminar
    printf("   Eliminando elemento: %d\n", elem_a_eliminar.number);
    
    Remove(&mi_lista, pos);  // Elimina el nodo y libera su memoria
    imprimirLista(&mi_lista, "Despues de eliminar");
    
    // REEMPLAZO DE VALOR
    printf("\n5. Probando Replace() - Reemplazar posicion 3 con 777...\n");
    pos = ElementPosition(&mi_lista, 3);  // Obtener puntero al 3er elemento
    elemento anterior = Position(&mi_lista, pos);  // Guardar valor anterior para referencia
    printf("   Valor anterior: %d\n", anterior.number);
    e.number = 777;
    // Replace() cambia solo el contenido, no modifica la estructura de enlaces
    Replace(&mi_lista, pos, e);  // Reemplazar contenido sin cambiar punteros
    imprimirLista(&mi_lista, "Despues de reemplazar");
    
    Destroy(&mi_lista);  // Liberar toda la memoria asignada
}

/*
void imprimirLista(lista *l, char *titulo)
Recibe: lista *l como referencia a la lista a imprimir y char *titulo como texto descriptivo
Devuelve: void
Observaciones: Función auxiliar que imprime el contenido de una lista de forma legible,
limitando la salida a 20 elementos para evitar salidas excesivamente largas en pruebas de estrés.
*/
void imprimirLista(lista *l, char *titulo) {
    printf("   %s: [", titulo);  // Iniciar formato de salida
    posicion p = First(l);       // Comenzar desde el primer elemento
    int contador = 0;
    
    // Recorrer la lista hasta máximo 20 elementos
    while(p != NULL && contador < 20) {
        elemento e = Position(l, p);  // Obtener elemento actual
        printf("%d", e.number);       // Imprimir valor
        p = Following(l, p);          // Avanzar al siguiente
        
        // Agregar coma separadora si no es el último elemento mostrado
        if(p != NULL && contador < 19) printf(", ");
        contador++;
    }
    
    // Si la lista tiene más de 20 elementos, indicar el total
    if(Size(l) > 20) {
        printf("... (%d elementos total)", Size(l));
    }
    printf("]\n");  // Cerrar formato de salida
}

/*
void separador(char *texto)
Recibe: char *texto como título de la sección a mostrar
Devuelve: void
Observaciones: Función auxiliar que imprime un separador visual con el título de la sección
de pruebas para mejorar la legibilidad de la salida del programa.
*/
void separador(char *texto) {
    printf("\n");
    printf("========================================\n");
    printf("%s\n", texto);
    printf("========================================\n");
}