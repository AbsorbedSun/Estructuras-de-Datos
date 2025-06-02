/*
LIBRERIA: Cabecera de el TAD COLA DINÁMICA
AUTOR: Aldo Garcia Ambrosio (C) Mayo 2025
AUTOR ORIGINAL: Edgardo Adrián Franco Martínez (C) Noviembre 2022
Uso de IA: Claude 3.7 Sonnet, ChatGPT 4.0
VERSIÓN: 2.4

DESCRIPCIÓN: TAD cola o Queue.
Estructura de datos en la que se cumple:
Los elementos se insertan en un extremo (el posterior) y 
la supresiones tienen lugar en el otro extremo (frente).

OBSERVACIONES: Hablamos de una Estructura de datos dinámica 
cuando se le asigna memoria a medida que es necesitada, 
durante la ejecución del programa. 
 
*/
#include <string.h>
//DEFINICIONES DE CONSTANTES
#define TRUE	1
#define FALSE	0

//DEFINICIONES DE TIPOS DE DATO

//Definir un boolean (Se modela con un "char")
typedef unsigned char boolean;

//Definir un elemento (Se modela con una estructura "elemento")
typedef struct elemento
{
	//Variables de la estructura "elemento" (El usuario puede modificar)
	int numero;	//Número de elemento
	char texto[100];
	//***
	//***
	//***
} elemento;

//Definir la estructura de un nodo dinámico
typedef struct nodo
{
	elemento e;
	struct nodo *siguiente;
}nodo;

//Definir una cola 
typedef struct cola
{
	int num_elem;
	nodo *frente;
	nodo *final;
} cola;

//DECLARACIÓN DE FUNCIONES
void Initialize(cola * c);			//Inicializar cola (Initialize): Recibe una cola y la inicializa para su trabajo normal.
void Queue(cola * c, elemento e);	//Encolar (Queue): Recibe una cola y agrega un elemento al final de ella. 
elemento Dequeue(cola * c);			//Desencolar (Dequeue): Recibe una cola y remueve el elemento del frente retornándolo.
boolean Empty(cola * c);			//Es vacía (Empty): Recibe la cola y devuelve verdadero si esta esta vacía.
elemento Front(cola * c);			//Frente (Front): Recibe una cola y retorna el elemento del frente.	
elemento Final(cola * c);			//Final (Final): Recibe una cola y retorna el elemento del final.
elemento Element(cola *c, int i); 	// Recibe una cola y un número de elemento de 1 al tamaño de la cola y retorna el elemento de esa posición.
int Size(cola *c);					//Tamaño (Size): Retorna el tamaño de la cola 	
void Destroy(cola * c);				//Eliminar cola (Destroy): Recibe una cola y la libera completamente.
int Search(cola *c, elemento e);	//Buscar (Search): Busca un elemento en la cola y retorna su posición
void Flip(cola *c);				//Invertir (Flip): Invierte el orden de los elementos en la cola
