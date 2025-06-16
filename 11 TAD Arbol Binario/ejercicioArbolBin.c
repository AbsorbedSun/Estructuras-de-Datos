/*
PROGRAMA PRINCIPAL - ÁRBOL BINARIO CON RECORRIDOS
AUTOR: Aldo Garcia Ambrosio (C) Junio 2025
USO DE IA: Claude Sonnet 4
VERSION: 1.0

DESCRIPCIÓN: Programa que implementa un árbol binario específico y demuestra 
los tres tipos de recorridos fundamentales: PreOrden, InOrden y PostOrden.
El programa crea el árbol mostrado en la imagen proporcionada y ejecuta
cada tipo de recorrido mostrando el orden de visita de los nodos.

ESTRUCTURA DEL ÁRBOL CREADO:
        A
       / \
      B   C
     / \ / \
    D  E F  G
      /     \
     H       K

RECORRIDOS IMPLEMENTADOS:
- PreOrden (Raiz → Izquierda → Derecha): A B D E H F C G K
- InOrden (Izquierda → Raiz → Derecha): D B H E A F C G K  
- PostOrden (Izquierda → Derecha → Raiz): D H E B F K G C A

OBSERVACIONES: El programa utiliza la implementación del TAD Árbol Binario
proporcionada en TADArbol_bin.c y su archivo de cabecera correspondiente.
Cada nodo del árbol almacena un carácter como elemento.

COMPILACIÓN: gcc ejercicioArbolBin.c TADArbol_bin.c -o ejercicio_arbol
EJECUCIÓN: ./ejercicio_arbol
*/

//LIBRERÍAS
#include "TADArbol_bin.h"
#include <stdio.h>
#include <stdlib.h>

//PROTOTIPOS DE FUNCIONES
void PreOrden(arbol_binario *a, posicion p);
void InOrden(arbol_binario *a, posicion p);
void PostOrden(arbol_binario *a, posicion p);

//DEFINICIÓN DE FUNCIONES

/*
int main()
Descripción: Función principal que coordina la creación del árbol binario específico
y la ejecución de los tres tipos de recorridos (PreOrden, InOrden, PostOrden)
Recibe: void (No recibe parámetros)
Devuelve: int (0 si termina correctamente, 1 si hay algún error)
Observaciones: La función crea el árbol nodo por nodo siguiendo la estructura
mostrada en la imagen, luego ejecuta y muestra cada tipo de recorrido,
finalmente libera la memoria utilizada.
*/
int main() {
    // Declaración de variables
    arbol_binario arbol;
	elemento e;
	posicion p;
	
	Initialize(&arbol);	
	
	// Insertar A como raíz
	e.caracter = 'A';
	NewRightSon(&arbol, NULL, e);  // Insertar como raíz si árbol está vacío
	
	// Posicionarse en A
	p = Root(&arbol); // A
	e.caracter = 'C';
	NewRightSon(&arbol, p, e);
	e.caracter = 'B';
	NewLeftSon(&arbol, p, e);
	
	// Posicionarse en C
	p = RightSon(&arbol, p); // C
	e.caracter = 'F';
	NewLeftSon(&arbol, p, e);
	e.caracter = 'G';
	NewRightSon(&arbol, p, e);
	
	// Posicionarse en G
	p = RightSon(&arbol, p); // G
	e.caracter = 'K';
	NewRightSon(&arbol, p, e);
	
	// Posicionarse en B
	p = Parent(&arbol, p); // C
	p = Parent(&arbol, p); // A
	p = LeftSon(&arbol, p); // B
	e.caracter = 'D';
	NewLeftSon(&arbol, p, e);
	e.caracter = 'E';
	NewRightSon(&arbol, p, e);

	// Posicionarse en E
	p = RightSon(&arbol, p); // E
	e.caracter = 'H';
    NewLeftSon(&arbol, p, e);
    
    // Confirmar que el árbol se creó correctamente
    printf("Arbol creado\n");
    printf("Estructura:\n");
    printf("        A\n");
    printf("       / \\\n");
    printf("      B   C\n");
    printf("     / \\ / \\\n");
    printf("    D  E F  G\n");
    printf("      /     \\\n");
    printf("     H       K\n\n");
    
    // Ejecutar y mostrar los tres tipos de recorridos
    printf("RECORRIDOS\n\n");
    
    // Recorrido PreOrden (Raiz → Izquierda → Derecha)
    printf("PREORDEN (Raiz -> Izquierda -> Derecha): ");
    PreOrden(&arbol, Root(&arbol));
    printf("\n\n");
    
    // Recorrido InOrden (Izquierda → Raiz → Derecha)
    printf("INORDEN (Izquierda -> Raiz -> Derecha): ");
    InOrden(&arbol, Root(&arbol));
    printf("\n\n");
    
    // Recorrido PostOrden (Izquierda → Derecha → Raiz)
    printf("POSTORDEN (Izquierda -> Derecha -> Raiz): ");
    PostOrden(&arbol, Root(&arbol));
    printf("\n\n");
    
    // Liberar la memoria 
    Destroy(&arbol);   
    return 0;
}

/*
void PreOrden(arbol_binario *a, posicion p)
Descripción: Función recursiva que realiza el recorrido PreOrden del árbol binario
(visita primero el nodo actual, luego el subárbol izquierdo, finalmente el subárbol derecho)
Recibe: arbol_binario *a (referencia al árbol binario), posicion p (posición actual a procesar)
Devuelve: void (No retorna valor explícito)
Observaciones: El recorrido PreOrden es útil para crear copias del árbol o para
operaciones que requieren procesar un nodo antes que sus descendientes.
Orden de visita: Raiz → Subárbol Izquierdo → Subárbol Derecho
*/
void PreOrden(arbol_binario *a, posicion p){
	if (p != NULL) {
		elemento e = ReadNode(a, p);
		printf("%c ", e.caracter); // Visita el nodo
		PreOrden(a, LeftSon(a, p));  // Recorre hijo izquierdo
		PreOrden(a, RightSon(a, p)); // Recorre hijo derecho
	}
}

/*
void InOrden(arbol_binario *a, posicion p)
Descripción: Función recursiva que realiza el recorrido InOrden del árbol binario
(visita primero el subárbol izquierdo, luego el nodo actual, finalmente el subárbol derecho)
Recibe: arbol_binario *a (referencia al árbol binario), posicion p (posición actual a procesar)
Devuelve: void (No retorna valor explícito)
Observaciones: El recorrido InOrden en árboles binarios de búsqueda produce los elementos
en orden ascendente. Es útil para obtener los datos ordenados del árbol.
Orden de visita: Subárbol Izquierdo → Raiz → Subárbol Derecho
*/
void InOrden(arbol_binario *a, posicion p) {
    // Verificar que la posición no sea nula (caso base de la recursión)
    if (p != NULL) {
        // Paso 1: Recorrer recursivamente el subárbol izquierdo
        InOrden(a, LeftSon(a, p));
        
        // Paso 2: Visitar la Raiz (procesar el nodo actual)
        elemento e = ReadNode(a, p);
        printf("%c ", e.caracter);
        
        // Paso 3: Recorrer recursivamente el subárbol derecho
        InOrden(a, RightSon(a, p));
    }
}

/*
void PostOrden(arbol_binario *a, posicion p)
Descripción: Función recursiva que realiza el recorrido PostOrden del árbol binario
(visita primero el subárbol izquierdo, luego el subárbol derecho, finalmente el nodo actual)
Recibe: arbol_binario *a (referencia al árbol binario), posicion p (posición actual a procesar)
Devuelve: void (No retorna valor explícito)
Observaciones: El recorrido PostOrden es útil para operaciones de limpieza o destrucción
del árbol, ya que procesa los hijos antes que el padre. También se usa en evaluación de expresiones.
Orden de visita: Subárbol Izquierdo → Subárbol Derecho → Raiz
*/
void PostOrden(arbol_binario *a, posicion p) {
    // Verificar que la posición no sea nula (caso base de la recursión)
    if (p != NULL) {
        // Paso 1: Recorrer recursivamente el subárbol izquierdo
        PostOrden(a, LeftSon(a, p));
        
        // Paso 2: Recorrer recursivamente el subárbol derecho
        PostOrden(a, RightSon(a, p));
        
        // Paso 3: Visitar la Raiz (procesar el nodo actual)
        elemento e = ReadNode(a, p);
        printf("%c ", e.caracter);
    }
}