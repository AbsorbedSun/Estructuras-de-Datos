#ifndef __TADArbol_bin_H
#define __TADArbol_bin_H
/*
IMPLEMENTACIONES DEL TAD ÁRBOL BINARIO (TADArbol_bin.h)
AUTOR ORIGINAL: Edgardo Adrián Franco Martínez (C) Mayo 2021
AUTOR ACTUAL: Garcia Ambrosio Aldo (C) Junio 2025
VERSIÓN: 1.4

DESCRIPCIÓN: TAD árbol binario (binary tree)
Estructura de datos jerárquica en la que se cumple:
- Cada nodo tiene como máximo dos hijos (izquierdo y derecho)
- Existe un nodo raíz desde el cual se puede acceder a todos los demás nodos
- Los nodos sin hijos se llaman hojas
- Permite operaciones de inserción, eliminación, búsqueda y recorrido

OBSERVACIONES: Este archivo solo incluye las estructuras, tipos de datos y
declaración de las operaciones del TAD Árbol Binario.

EL CÓDIGO QUE IMPLEMENTA LAS FUNCIONES ES EL ARCHIVO: TADArbol_bin.c

Estructura visual del árbol:
                    Raíz
                   /    \
              Hijo Izq  Hijo Der
             /    \        /    \
           ...   ...    ...   ...
*/

#define TRUE 1
#define FALSE 0

typedef unsigned char boolean;

typedef struct elemento{
    //Lo que gustes
    int number;
    char caracter;
}elemento;

//Estructura de un NODO de árbol binario
typedef struct nodo{
    elemento e;
    //Relación con hijo izquierdo
    struct nodo* izq;
    //Relación con hijo derecho
    struct nodo* der;
}nodo;

//Arbol binario como un apuntador a nodo (raíz)
typedef nodo* arbol_binario;

//Posición como un apuntador a nodo
typedef nodo* posicion;

/***************************************************
Operaciones de construcción
***************************************************/

//Efecto: Recibe un árbol binario a y lo inicializa para su trabajo normal.
void Initialize(arbol_binario *a);

//Efecto: Recibe un árbol binario a y lo libera completamente de memoria.
void Destroy(arbol_binario *a);

/***************************************************
Operaciones de posicionamiento y búsqueda
***************************************************/

//Efecto: Recibe un árbol binario a y retorna la posición del nodo raíz.
posicion Root(arbol_binario *a);

//Efecto: Recibe un árbol binario a y una posición p, devuelve la posición del nodo padre de p.
//Requerimientos: La posición p es válida y no es la raíz.
posicion Parent(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, devuelve la posición del hijo derecho de p.
//Requerimientos: La posición p es válida.
posicion RightSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, devuelve la posición del hijo izquierdo de p.
//Requerimientos: La posición p es válida.
posicion LeftSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y un elemento e, devuelve la posición del primer nodo que coincida exactamente con e.
posicion Search(arbol_binario *a, elemento e);

/***************************************************
Operaciones de consulta
***************************************************/

//Efecto: Recibe un árbol binario a y devuelve TRUE si el árbol está vacío, FALSE en caso contrario.
boolean Empty(arbol_binario *a);

//Efecto: Recibe un árbol binario a y una posición p, devuelve TRUE si p es una posición válida en el árbol, FALSE en caso contrario.
boolean NullNode(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, devuelve el elemento almacenado en la posición p.
//Requerimientos: La posición p es válida.
elemento ReadNode(arbol_binario *a, posicion p);

/***************************************************
Operaciones de modificación
***************************************************/

//Efecto: Recibe un árbol binario a, una posición p y un elemento e; crea un nuevo hijo derecho de p con el elemento e.
//Requerimientos: Si el árbol está vacío, se crea la raíz. Si p ya tiene hijo derecho, se produce error.
void NewRightSon(arbol_binario *a, posicion p, elemento e);

//Efecto: Recibe un árbol binario a, una posición p y un elemento e; crea un nuevo hijo izquierdo de p con el elemento e.
//Requerimientos: Si el árbol está vacío, se crea la raíz. Si p ya tiene hijo izquierdo, se produce error.
void NewLeftSon(arbol_binario *a, posicion p, elemento e);

//Efecto: Recibe un árbol binario a y una posición p, elimina completamente el subárbol derecho de p.
//Requerimientos: La posición p es válida.
void DeleteRightSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, elimina completamente el subárbol izquierdo de p.
//Requerimientos: La posición p es válida.
void DeleteLeftSon(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a y una posición p, elimina el nodo en la posición p y todos sus descendientes.
//Requerimientos: La posición p es válida.
void DeleteNode(arbol_binario *a, posicion p);

//Efecto: Recibe un árbol binario a, una posición p y un elemento e, sustituye el elemento en la posición p por e.
//Requerimientos: La posición p es válida.
void ReplaceNode(arbol_binario *a, posicion p, elemento e);

#endif    // __TADArbol_bin_H