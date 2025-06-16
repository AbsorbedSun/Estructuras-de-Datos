#include <stdio.h>
#include "TADArbol_bin.h"

//                               A
//                           /        \ 
//                        B              C
//                     /    \         /    \
//                   J       K     D          E
//                                /  \      /   \ 
//                              F     G    H      I

// Declaración de las funciones para mostrar el recorrido en PreOrden
void ReccorridoPreOrden(arbol_binario *A);
void ReccorridoRecursivoPreOrden(arbol_binario *A, posicion p);

// Programa principal
int main (void)
{
	arbol_binario a;
	elemento e;
	posicion p;
	
	Initialize(&a);	
	
	// Insertar A como raíz
	e.c = 'A';
	NewRightSon(&a, NULL, e);  // Insertar como raíz si árbol está vacío
	
	// Posicionarse en A
	p = Root(&a); // A
	e.c = 'C';
	NewRightSon(&a, p, e);
	e.c = 'B';
	NewLeftSon(&a, p, e);
	
	// Posicionarse en C
	p = RightSon(&a, p); // C
	e.c = 'D';
	NewLeftSon(&a, p, e);
	e.c = 'E';
	NewRightSon(&a, p, e);
	
	// Posicionarse en D
	p = LeftSon(&a, p); // D
	e.c = 'F';
	NewLeftSon(&a, p, e);
	e.c = 'G';
	NewRightSon(&a, p, e);
	
	// Posicionarse en E
	p = Parent(&a, p); // C
	p = RightSon(&a, p); // E
	e.c = 'H';
	NewLeftSon(&a, p, e);
	e.c = 'I';
	NewRightSon(&a, p, e);
	
	// Posicionarse en B
	p = Parent(&a, p); // C
	p = Parent(&a, p); // A
	p = LeftSon(&a, p); // B
	e.c = 'J';
	NewLeftSon(&a, p, e);
	e.c = 'K';
	NewRightSon(&a, p, e);
	
	// Prueba de lectura
	p = Root(&a);
	e = ReadNode(&a, p);
	printf("\nRaiz = %c", e.c);
	
	p = RightSon(&a, p); // C
	e = ReadNode(&a, p);
	printf("\nHijo derecho de Raiz = %c", e.c);
	
	p = RightSon(&a, p); // E
	e = ReadNode(&a, p);
	printf("\nHijo derecho del hijo derecho de la Raiz = %c", e.c);
	
	// Recorrido
	printf("\n\nRecorrido en PreOrden del árbol A:\n");
	ReccorridoPreOrden(&a);
	
	Destroy(&a);
	
	return 0;	
}

// Función que inicia el recorrido en PreOrden
void ReccorridoPreOrden(arbol_binario *A)
{
	posicion p = Root(A);
	ReccorridoRecursivoPreOrden(A, p);
}

// Función recursiva para recorrido PreOrden
void ReccorridoRecursivoPreOrden(arbol_binario *A, posicion p)
{
	if (p != NULL) {
		elemento e = ReadNode(A, p);
		printf("%c ", e.c); // Visita el nodo
		ReccorridoRecursivoPreOrden(A, LeftSon(A, p));  // Recorre hijo izquierdo
		ReccorridoRecursivoPreOrden(A, RightSon(A, p)); // Recorre hijo derecho
	}
}
