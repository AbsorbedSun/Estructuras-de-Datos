#include <stdlib.h>
#include <stdio.h>
#include "pila_est.h"

/*
Inicializar pila (Initialize): recibe<- pila (s); 
Initialize(s);
Efecto: Recibe una pila y la inicializa para su trabajo normal.
*/
void Initialize(pila *s){
	(*s).tope=-1; //s->tope=-1;
	return;
}

/*
Empilar (Push): recibe<- pila (s); recibe<- elemento (e) 
Push(s,e);
Efecto: Recibe la pila y aumenta su tamaño, poniendo el elemento en la cima de la pila.
*/
void Push(pila *s,elemento e){
	if(s->tope<TAM-1){
		s->tope++;
		s->A[s->tope]=e; //(*s).A[(*s).tope]=e;
	}
	else {//Desbordamiento de pila
		printf("\nERROR Push(s,e):\"Desbordamiento de pila\"");
		exit(1);	
	}
	return;
}

/*
Desempilar (Pop): recibe<- pila (s); retorna -> elemento
e=Pop(s);
Efecto: Recibe la pila, remueve el elemento tope y lo retorna.
Excepción: si la pila esta vacía, produce error.
*/
elemento Pop(pila *s){
	elemento r;
	if(s->tope!=-1){
		r=s->A[s->tope];
		s->tope--;
	}
	else{ //Subdesbordamiento de la pila
		printf("\nERROR e=Pop(s):\"Subdesbordamiento de pila\"");
		exit(1);			
	}
	
	return r;
}

/*
Es vacía (Empty): recibe<- pila (s); retorna -> boolean
b=Empty(s);
Efecto: Recibe una pila y devuelve true si esta vacía y false en caso contrario.
*/
boolean Empty(pila *s){
	boolean r;
	if(s->tope!=-1)
		r=FALSE;
	else
		r=TRUE;
	
	return r;
}


/*
Tope pila (Top): recibe<- pila (s); retorna -> elemento
e=Top(s);
Efecto: Devuelve el elemento cima de la pila.
Excepción: si la pila esta vacía produce error
*/
elemento Top(pila *s){
	elemento r;
	if(s->tope!=-1){
		r=s->A[s->tope];
	}
	else{ //Subdesbordamiento de la pila
		printf("\nERROR e=Top(s):\"Subdesbordamiento de pila\"");
		exit(1);			
	}
	
	return r;
}


/*
Tamaño pila (size): recibe<- pila (s); retorna -> tamaño de la pila (entero)
n=size(s);
Efecto: Devuelve el número de elementos que contiene la pila (Altura de la pila).
*/
int Size(pila *s){
	int r;
	r=s->tope+1;
	return r;
}

/*
Eliminar pila (Destroy): recibe<- pila (s)
Destroy(s);
Efecto: Recibe una pila y la libera completamente
*/
void Destroy(pila *s){
	Initialize(s);
	return;
}

/* 
Obtener elemento (Element): recibe<- pila (S); recibe<- índice (n); retorna -> elemento e=Element(S,n);
Efecto: Recibe una pila y un índice, y devuelve el elemento en la posición n de la pila.
Excepción: Si el índice está fuera de rango o la pila está vacía, produce error. 
*/
elemento Element(pila *s, int n){
	int altura = Size(s);
    if (n < 1 || n > altura) {
        printf("\nERROR Element(S,%d): \"Índice fuera de rango\"\n", n);
        exit(1);
    }
    // El fondo está en A[0], el tope en A[tope]
    return s->A[n - 1];
}

/* 
Invertir pila (Flip): recibe<- pila (S)
Flip(S);
Efecto: Recibe una pila y la invierte, de manera que el elemento que estaba en el fondo ahora 
está en el tope y viceversa, manteniendo el orden de todos los elementos de la pila. 
*/
void Flip(pila *s) {
    int i, j;
    elemento temp;
    int alto = s->tope;
    // intercambiamos A[0] con A[t], A[1] con A[t-1], …
    for (i = 0, j = alto; i < j; ++i, --j) {
        temp = s->A[i];
        s->A[i] = s->A[j];
        s->A[j] = temp;
    }
}
