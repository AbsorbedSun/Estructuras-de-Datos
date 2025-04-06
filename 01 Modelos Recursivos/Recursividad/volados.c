/*
AUTOR: Edgardo Adrián Franco Martínez (C) Septiembre 2024
VERSIÓN: 1.0

DESCRIPCIÓN: Programa que genera y muestra todas las posibles combinaciones de longitud 'n' 
formadas por los caracteres 'S' y 'A'. Este tipo de combinaciones puede interpretarse como el 
resultado de una secuencia de lanzamientos de una moneda donde 'S' representa "sol" y 'A' 
representa "águila". El programa emplea recursión para generar las combinaciones.

OBSERVACIONES: El programa utiliza un enfoque recursivo para construir las cadenas 
agregando los caracteres 'S' y 'A'. La longitud máxima de las cadenas está definida por 
la constante TAM, que es 101 en este caso.

COMPILACIÓN: gcc -o volados volados.c

EJECUCIÓN: ./volados <n> (Donde 'n' es la longitud de las cadenas a generar)

PARÁMETROS:
    <n>: Número entero que indica la longitud de las cadenas a generar. Debe pasarse como argumento 
    al ejecutar el programa.
    
    EJEMPLO: ./volados 3 (Generará las cadenas de longitud 3, como "SSS", "SSA", "SAS", "SAA", "ASS", "ASA", "AAS", "AAA")
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM 101

// Imprime todas las posibles cadenas de longitud n formadas por los caracteres 'S' y 'A'.
void Volados(int n, char* c);

int main (int narg, char *c[])
{
	int n;

	// Verificación de la cantidad de argumentos
	if(narg != 2)
		exit(1);
	
	// Convierte el argumento de la cadena a un entero
	n = atoi(c[1]);
	
	// Llama a la función Volados para generar y mostrar las combinaciones
	Volados(n, "");
	
	return 0;
}

void Volados(int n, char* c)
{
	// Variables auxiliares para evitar modificar la cadena original
	char cadenamia1[TAM];
	char cadenamia2[TAM];
	strcpy(cadenamia1, c);
	strcpy(cadenamia2, c);
	
	// Caso base: Si n es 0, imprime la cadena generada hasta el momento
	if(n == 0)
	{
		printf("\n%s", c);
		return;
	}
	else
	{
		// Se llama a la función recursivamente para n-1, agregando un 'S' al final de la cadena
		Volados(n - 1, strcat(cadenamia1, "S"));
		
		// Se llama a la función recursivamente para n-1, agregando un 'A' al final de la cadena
		Volados(n - 1, strcat(cadenamia2, "A"));
		
		return;
	}
}
