/*
IMPLEMENTACIONES DE LA LIBRERIA DEL TAD LISTA DOBLEMENTE LIGADA (TADListaDL.h)
AUTOR ORIGINAL: Edgardo Adrián Franco Martínez (C)
AUTOR ACTUAL: Garcia Ambrosio Aldo (C) Mayo 2025
VERSIÓN: 2.0 (Diciembre 2024)

DESCRIPCIÓN: TAD lista doblemente ligada
Estructura de datos en la que se cumple:
Los elementos se consultan, añaden y se remueven con base en posiciones 
dentro de una estructura enlazada donde cada nodo tiene referencias
al siguiente y al anterior.

OBSERVACIONES: TADLista por definición es una Estructura de Datos dinámica. 
La implementación del presente código se realiza mediante el principio de "Lista Doblemente Ligada"
i.e. nodos que contienen un elemento y se encuentran ligados hacia el siguiente y anterior.

ESTRUCTURA:
Frente                                                       Final
      ******    ******    ******    ******    ******    ******
   -> *    * -> *    * -> *    * -> *    * -> *    * -> *    *
      * N1 *    * N2 *    * N3 *    * N4 *    * N5 *    * N6 * ->
   <- *    * <- *    * <- *    * <- *    * <- *    * <- *    *
NULL  ******    ******    ******    ******    ******    ******  NULL

COMPILACIÓN PARA GENERAR EL CÓDIGO OBJETO: gcc TADListaDL.c -c 
*/

//LIBRERÍAS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "TADListaDL.h" 

/***************************************************
Operaciones de construcción
***************************************************/

/*
void Initialize(lista *l)
Descripción: Inicializar lista (Iniciar una lista para su uso)
Recibe: lista *l (Referencia a la lista "l" a operar)
Devuelve:
Observaciones: El usuario ha creado una lista y l tiene la referencia a ella, 
si esto no ha pasado se ocasionará un error.
*/
void Initialize(lista *l){
    l->frente = NULL;
    l->final = NULL;
    l->tam = 0;
    l->apellido = l;
    return;
}

/*
void Destroy(lista *l)
Descripción: Destruir una lista (Recibe una lista l y la libera completamente)
Recibe: lista *l (Referencia a la lista "l" a operar)
Devuelve:
Observaciones: El usuario ha creado una lista y l tiene la referencia a ella, 
si esto no ha pasado se ocasionará un error.
*/
void Destroy(lista *l)    {
    posicion aux; 
    aux = l->frente; 
    while(aux != NULL){
        l->frente = aux->siguiente; 
        free(aux); 
        aux = l->frente; 
    }
    l->final = NULL; 
    l->tam = 0; 
    return;
}

/***************************************************
Operaciones de posicionamiento y búsqueda
***************************************************/

/*
posicion Final(lista *l)
Descripción: Recibe una lista l y regresa la posición del final
Recibe: lista *l (Referencia a la lista "l" a operar)
Devuelve: posición del nodo que contiene al elemento final de la lista
*/
posicion Final(lista *l){
    return l->final;
}

/*
posicion First(lista *l)
Descripción: Recibe una lista l y regresa la posición del frente
Recibe: lista *l (Referencia a la lista "l" a operar)
Devuelve: posición del nodo que contiene al elemento del frente de la lista
*/
posicion First(lista *l){
    return l->frente;
}

/*
posicion Following(lista *l, posicion p)
Descripción: Recibe una lista l, una posición p y devuelve la posición del 
elemento siguiente de p
Recibe: lista *l y posicion p (Referencia a la lista "l" a operar y posición válida de la lista)
Devuelve: posición del nodo siguiente a la posición dada
*/
posicion Following(lista *l, posicion p){
    if(ValidatePosition(l, p))
        return p->siguiente;
    printf("ERROR: Following(L,p) la posición es inválida");
    exit(1);
}

/*
posicion Previous(lista *l, posicion p)
Descripción: Recibe una lista L, una posición P y devuelve la posición del 
elemento anterior de P (MEJORADO para lista doblemente ligada)
Recibe: lista *l y posicion p (Referencia a la lista "l" a operar y posición válida de la lista)
Devuelve: posición del nodo anterior a la posición dada
*/
posicion Previous(lista *l, posicion p){
    if(ValidatePosition(l, p))
        return p->anterior;  // MEJORA: Acceso directo al anterior
    printf("ERROR: Previous(L,p) posición inválida");
    exit(1);
}

/*
posicion Search(lista *l, elemento e)
Descripción: Recibe una lista l y un elemento e, regresa la posición que coincida exactamente con el elemento e.
Recibe: lista *l y un elemento e (Referencia a la lista "l" a operar y elemento a buscar en la lista)
Devuelve: posición del elemento en caso de ser encontrado, si no se encuentra se devuelve una posición inválida
*/
posicion Search(lista *l, elemento e){    
    posicion aux = l->frente;
    while(aux != NULL){
        if(memcmp(&aux->e, &e, sizeof(elemento)) == 0){
            return aux;            
        }
        aux = aux->siguiente;
    }
    return NULL;
}

/***************************************************
Operaciones de consulta
/***************************************************/

/*
elemento Position(lista *l, posicion p)
Descripción: Recibe una lista l, una posición p y devuelve el elemento en dicha posición. 
Recibe: lista *l y una posicion p(Referencia a la lista "l" a operar posición válida en la lista)
Devuelve: Elemento en la posición dada, si la posición es inválida se genera error.
*/
elemento Position(lista *l, posicion p){
    if(ValidatePosition(l, p))
        return p->e;
    printf("ERROR: Position(L,p) la posición es inválida");
    exit(1);
}

/*
boolean ValidatePosition(lista *l, posicion p)
Descripción: Recibe una lista l, una posición p y devuelve TRUE si la posición es válida en la lista l y FALSE en caso contrario.
*/
boolean ValidatePosition(lista *l, posicion p){
    boolean b = FALSE;
    if(p != NULL)
        if(p->apellido == l->apellido)
            b = TRUE;
    return b;    
}

/*
elemento Element(lista *l, int n)
Descripción: Recibe una lista y un índice (entre 1 y el tamaño de la lista) y devuelve el elemento que se encuentra en la lista en ese índice.
*/
elemento Element(lista *l, int n){
    int i = 1;
    posicion aux = l->frente;
    if(n > 0 && n <= l->tam){
        while(i < n){
            i++;
            aux = aux->siguiente;
        }
        return aux->e;
    }
    printf("ERROR: Element(L,n) n inválido");    
    exit(1);
}

/*
posicion ElementPosition(lista *l, int n)
Descripción: Recibe una lista y un índice y devuelve la posición del elemento en ese índice.
*/
posicion ElementPosition(lista *l, int n){
    int i = 1;
    posicion aux = l->frente;
    if(n > 0 && n <= l->tam){
        while(i < n){
            i++;
            aux = aux->siguiente;
        }
        return aux;
    }
    printf("ERROR: ElementPosition(L,n) n inválido");    
    exit(1);            
}

/*
int Size(lista * l);
Descripción: Recibe una lista y devuelve el número de elementos que se encuentran en esta.
*/
int Size(lista *l){
    return l->tam;
}

/*
boolean Empty(lista * l);
Descripción: Recibe una lista l y devuelve TRUE en caso de que la lista esté vacía y FALSE en caso contrario.
*/
boolean Empty(lista *l){
    boolean r;
    if(l->tam == 0)
        r = TRUE;
    else
        r = FALSE;
    return r;
}

/***************************************************
Operaciones de modificación
****************************************************/

/*
void Insert(lista * l, posicion p, elemento e, boolean b);
Descripción: Inserta un elemento e en la lista este deberá agregarse al frente de p 
si b es verdadero y atrás de p en caso contrario. MEJORADO para lista doblemente ligada.
*/
void Insert(lista *l, posicion p, elemento e, boolean b){
    posicion aux;
    aux = malloc(sizeof(nodo));
    if(aux == NULL){
        printf("ERROR: Insert(l,p,e,b) Desbordamiento de lista.");
        exit(1);
    }
    aux->e = e;
    aux->apellido = l->apellido;
    aux->anterior = NULL;  // Inicializar enlaces
    aux->siguiente = NULL;
    
    if(ValidatePosition(l, p)){
        if(b){    // Enfrente de p
            aux->siguiente = p;
            aux->anterior = p->anterior;
            
            if(p->anterior != NULL){  // p no es el frente
                p->anterior->siguiente = aux;
            }
            else{  // p es el frente
                l->frente = aux;
            }
            p->anterior = aux;
            l->tam++;
        }
        else{    // Atrás de p
            aux->anterior = p;
            aux->siguiente = p->siguiente;
            
            if(p->siguiente != NULL){  // p no es el final
                p->siguiente->anterior = aux;
            }
            else{  // p es el final
                l->final = aux;
            }
            p->siguiente = aux;
            l->tam++;
        }
    }
    else{ // si p es inválido, se inserta e enfrente
        aux->siguiente = l->frente;
        if(l->frente != NULL){
            l->frente->anterior = aux;
        }
        l->frente = aux;
        if(l->tam == 0)
            l->final = aux;
        l->tam++;
    }
    return;
}

/*
void Add(lista *l,elemento e);
Descripción: Recibe una lista l y un elemento e, se agrega e al final de la lista l.
*/
void Add(lista *l, elemento e){
    posicion aux;
    aux = malloc(sizeof(nodo));
    if(aux == NULL){
        printf("ERROR: Add(l,e) desbordamiento de lista");
        exit(1);
    }
    aux->e = e;
    aux->siguiente = NULL;
    aux->apellido = l->apellido;
    
    if(l->tam > 0){
        aux->anterior = l->final;    // Enlazar al anterior
        l->final->siguiente = aux;
    }
    else{
        aux->anterior = NULL;        // Primer elemento
        l->frente = aux;
    }
    l->final = aux;
    l->tam++;
    return;
}

/*
void Remove(lista *l,posicion p);
Descripción: Recibe una lista l y una posición p, el elemento en la posición p será removido.
*/
void Remove(lista *l, posicion p){
    if(!ValidatePosition(l, p)){
        printf("ERROR: Remove(l,p), p es inválida");
        exit(1);
    }    
    
    // MEJORA: Usar enlaces directos para reconectar
    if(p->anterior != NULL){
        p->anterior->siguiente = p->siguiente;
    }
    else{  // p es el frente
        l->frente = p->siguiente;
    }
    
    if(p->siguiente != NULL){
        p->siguiente->anterior = p->anterior;
    }
    else{  // p es el final
        l->final = p->anterior;
    }
    
    free(p);
    l->tam--;
    return;
}

/*
void Replace(lista *l,posicion p, elemento e);
Descripción: Recibe una lista l, una posición p y un elemento e, el elemento en la posición p será sustituido por e
*/
void Replace(lista *l, posicion p, elemento e){
    if(ValidatePosition(l, p)){
        p->e = e;
    }
    else{
        printf("ERROR: Replace(l,p,e) Posición inválida");
        exit(1);
    }
    return;
}

/********************************************************************************
Función para usarse en depuración, la cual imprime las direcciones de 
memoria de los nodos y sus apuntadores
********************************************************************************/
void VerLigasLista(lista *l){
    posicion aux;    
    aux = l->frente;    
    printf("\n*************************************************************");
    printf("\nRECORRIDO HACIA ADELANTE:");
    while(aux != NULL){
        printf("\nAnterior=%p\tPosicion=%p\tSiguiente=%p", 
               (void*)aux->anterior, (void*)aux, (void*)aux->siguiente);
        aux = aux->siguiente;
    }
    
    printf("\nRECORRIDO HACIA ATRAS:");
    aux = l->final;
    while(aux != NULL){
        printf("\nAnterior=%p\tPosicion=%p\tSiguiente=%p", 
               (void*)aux->anterior, (void*)aux, (void*)aux->siguiente);
        aux = aux->anterior;
    }
    printf("\n*************************************************************");

    return;
}