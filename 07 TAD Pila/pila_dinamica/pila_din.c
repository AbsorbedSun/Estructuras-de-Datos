#include <stdlib.h>
#include <stdio.h>
#include "pila_din.h"

/*
Inicializar pila (Initialize): recibe<- pila (s); 
Initialize(s);
Efecto: Recibe una pila y la inicializa para su trabajo normal.
*/
void Initialize(pila *s){
	s->size=0;
	(*s).tope=NULL;
	return;
}

/*
Empilar (Push): recibe<- pila (s); recibe<- elemento (e) 
Push(s,e);
Efecto: Recibe la pila y aumenta su tamaño, poniendo el elemento en la cima de la pila.
*/
void Push(pila *s,elemento e){
	nodo *aux;
	
	aux=malloc(sizeof(nodo));
	
	if(aux!=NULL){
		s->size++;
		aux->e=e;
		aux->abajo=s->tope;
		s->tope=aux;
	}
	else{ //Desbordamiento de pila
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
	nodo* aux;
	if(s->tope!=NULL){
		r=s->tope->e;
		aux=s->tope;
		s->tope=s->tope->abajo; //s->tope=aux->abajo
		free(aux);
		s->size--;
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
	if(s->tope!=NULL)
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
	if(s->tope!=NULL){
		r=s->tope->e;
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
	int r=s->size;
	/*int r=0;
	nodo *aux;
	aux=s->tope;
	while(aux!=NULL){
		r++;
		aux=aux->abajo;
	}
*/
	return r;
}

/*
Eliminar pila (Destroy): recibe<- pila (s)
Destroy(s);
Efecto: Recibe una pila y la libera completamente
*/
void Destroy(pila *s){
	nodo *aux;
	if(s->tope!=NULL){
		do{
			aux=s->tope->abajo;
			free(s->tope);
			s->tope=aux;	
		}while(aux!=NULL);
	}
	s->size=0;
	return;
}

/* 
Obtener elemento (Element): recibe<- pila (S); recibe<- índice (n); retorna -> elemento e=Element(S,n);
Efecto: Recibe una pila y un índice, y devuelve el elemento en la posición n de la pila.
Excepción: Si el índice está fuera de rango o la pila está vacía, produce error. 
*/
elemento Element(pila *s, int n) {
    // Obtiene el tamaño actual de la pila
	int altura = Size(s);   
    if (n < 1 || n > altura) {
        printf("\nERROR Element(S,%d): \"Índice fuera de rango\"\n", n);
        exit(1); // Termina si el índice es inválido
    }

    // Número de pasos desde el tope
	int pasos = altura - n;              
	// Inicia desde el tope de la pila
	nodo *aux = s->tope;                 
    for (int i = 0; i < pasos; i++) {
        aux = aux->abajo; // Avanza hacia abajo en la estructura
    }
    return aux->e; // Retorna el elemento encontrado
}

/* 
Invertir pila (Flip): recibe<- pila (S)
Flip(S);
Efecto: Recibe una pila y la invierte, de manera que el elemento que estaba en el fondo ahora 
está en el tope y viceversa, manteniendo el orden de todos los elementos de la pila. 
*/
void Flip(pila *s) {
    // Obtiene el número de elementos en la pila
	int n = Size(s);                     
    // Reserva memoria para un arreglo temporal
	elemento *temp = malloc(n * sizeof(*temp));  
    for (int i = 0; i < n; ++i) {
        temp[i] = Pop(s); // Extrae elementos y los guarda en orden inverso
    }
    for (int i = 0; i < n; ++i) {
        Push(s, temp[i]); // Reinserta los elementos en el mismo orden de extracción
    }
    free(temp); // Libera la memoria del arreglo temporal
}
