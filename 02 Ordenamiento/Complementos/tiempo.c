/*
tiempo.c
V 1.0 Agosto 2022
Autor: Edgardo Adrián Franco Martínez

Programa que lee n números de la entrada estandar y los coloca en un arreglo A, 
después ejecuta un algoritmo que invierte el arreglo y posteriormente pierde el 
tiempo en orden O(n^2).

Compilación 
Windows: gcc tiempo.c -o tiempo.exe
Linux: gcc tiempo.c -o tiempo
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TRUE 1
#define FALSE 0

void Algoritmo(int *A,int n);
	
int main(int argc, char* argv[])
{	
    //Variables para la medición de tiempos    
    clock_t t_inicio, t_final;
    double t_intervalo;
	
	//Variables para el algoritmo
	int i,j,n,*A;
	
	//Recibir por argumento el tamaño de n y el valor a buscar
	if (argc!=2) 
	{
		printf("\nIndique el tamanio de n y el valor a buscar - Ejemplo: [user@equipo]$ %s 100\n",argv[0]);
		exit(1);
	} 
	
	//Tomar el argumento del main
	n=atoi(argv[1]); //n
	
	//Apartar memoria para n números  enteros
	A=malloc(n*sizeof(int));
	if(A==NULL)
	{
		printf("\nError al intentar reservar memoria para %d elementos\n",n);		
		exit(1);
	}	
	
	//Leer de la entrada estandar los n valores y colocarlos en el arreglo de números
	for(i=0;i<n;i++)
		scanf("%d",&A[i]);
	
	//*****************************************  
	//Algoritmo a medir el tiempo
	//*****************************************
	//Inicia medición del tiempo
	t_inicio = clock();
	
	//Llamar al algoritmo
	Algoritmo(A,n);
	
	//Termina medición del tiempo	
	t_final = clock();
	
	//Cálculo del tiempo y enviar mensaje a salida estandar con la medición  
    t_intervalo = (double)(t_final - t_inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo medido: %.10f segundos.", t_intervalo);
	
	//Enviar a la salida estandar el arreglo final
	for(i=0;i<n;i++)
		printf("\nA[%d]=%d",i,A[i]);
	
	return 0;
}


/*
void Algoritmo(int *arreglo,int size)
Recibe: int * Referencia/Dirección al arreglo A, int tamaño del arreglo
Devuelve: void (No retorna valor explicito)
Observaciones: Función que intercambia los valores de A (voltea el arreglo) y
pierde tiempo en razón de O(n^2) 
*/
void Algoritmo(int *A,int n)
{
	int i,j,aux;	
	//Voltear el arreglo
	for(i=0;i<n;i++)
	{
		aux=A[(n-1)-i];
		A[(n-1)-i]=A[i];
		A[i]=aux;
	}
	
	//Perder tiempo en razón cuadrática
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			A[i]=A[i];
	
	return;
}