/*
Autores:	Erendil Aguilar Avendaño
			Iker Itzae Aguilar Souza
Versión 1.5 (03 de Junio 2024)
Grupo: 		2CM2
Materia:	Algoritmos y Estructuras de Datos
Práctica 04: Simulaciones con el TAD Cola

=== Simulaciones con el TAD Cola ===

Descripción: Programa que con el uso de colas simula el comportamiento de un Banco de manera animada.

Observaciones: El programa requerira de la libreria "presentacion.h", la cuál tiene las implementaciones
para mover el cursor de la pantalla, esperar un tiempo y borrar pantalla, asi como las librerias "TADColaEst.h"
y "TADCola/TADColaDin.h" para el uso correcto de las colas, la compilación debera incluir las definiciones de 
las funciones según la plataforma que se este utilizando (Windows o Linux).

Compilación: 	gcc -o simulacion_03 simulacion_03.c presentacion/presentacion(Win|Lin).o TADCola/TADCola(Din|Est|EstCirc).o (Win si se correra en Windows | Lin si se ejecutará en Linux)  "Si se tiene el código objeto"
				gcc -o simulacion_03 simulacion_03.c presentacion/presentacion(Win|Lin).c TADCola/TADCola(Din|Est|EstCirc).c (Win si se correra en Windows | Lin si se ejecutará en Linux)  "Si se tiene el código fuente"
Ejecución: Windows simulacion_03.exe  &  Linux ./simulacion_03
*/

//LIBRERIAS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include "presentacion/presentacion.h"
//#include "TADCola/TADColaEst.h" 	//Si se usa la implemtentación estática (TADColaEst.c|TADColaEstCirc.c)
#include "TADCola/TADColaDin.h" 	//Si se usa la implemtentación dinámica (TADColaDin.c)


//CONSTANTES
#define ALTO 24			//Se piensa en un pantalla de 24 filas x 79 columnas
#define ANCHO 79
#define TIEMPO_BASE	10	//Tiempo base en milisegundos

//FUNCIONES
void PedirRequisitos(int *cajeros, int *cajerosT, boolean *cajasB, int *clienteC, int *clienteU, int *clienteP);

void Simulacion(int cajeros, int cajerosT, boolean cajasB[], int clienteC, int clienteU, int clienteP);

void AgregarClienteCaja(boolean cajasB[], cola *cajero, cola *fila, int c, int f);
elemento QuitarClienteCaja(boolean cajasB[], cola *cajero, int c);
void AgregarClienteFila(cola *fila, int clienteN, int tipo);
elemento QuitarClienteFila(cola *fila, int tipo);

void AbrirBanco(int cajeros, boolean cajasB[]);

void DibujaPresentacion();
void DibujaMarco();
void DibujaCajas(int cajeros, boolean cajasB[]);
void DibujaFilas();
void DibujaEstantes();
void DibujaAnuncioAbrir();

//PROGRAMA PRINCIPAL
int main(void)
{
	int cajeros, cajerosT, clienteC, clienteU, clienteP;
	boolean cajasB[10];
	
	setlocale(LC_ALL, "");
	
	/***PEDIR REQUISITOS***/
	PedirRequisitos(&cajeros, &cajerosT, cajasB, &clienteC, &clienteU, &clienteP);
	
	/***INCIAR LA BANCO***/
	Simulacion(cajeros, cajerosT, cajasB, clienteC, clienteU, clienteP);
	
	MoverCursor(ANCHO,ALTO);
}

/*
	PedirRequisitos(int *cajeros, int *cajerosT, boolean *cajasB, int *clienteC, int *clienteU, int *clienteP)
	Recibe:	int * Referencia/Dirección al número de cajeros, int * Referencia/Dirección al tiempo de atención de los cajeros,
					boolean * Referencia/Dirección al arreglo de cajas siendo ocupadas, int * Referencia/Dirección al tiempo de llegada de un Cliente,
					int * Referencia/Dirección al tiempo de llegada de un Usuario, int * Referencia/Dirección al tiempo de llegada de un Preferente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que pide los datos como el número de cajeros, tiempo de consulta de cajas, y el tiempo
					de llegada de los Clientes, Usuarios y Preferentes.
*/
void PedirRequisitos(int *cajeros, int *cajerosT, boolean *cajasB, int *clienteC, int *clienteU, int *clienteP){
	int i;
	int Acajeros, AcajerosT, AclienteC, AclienteU, AclienteP;
	int conteo=0;
	
	BorrarPantalla();
	
	
    
	/***Pedir el numero de cajeros***/
	printf("¿Cuantos cajeros quiere que el Banco tenga?\n");
    do{
		scanf("%d", &Acajeros);
		if(Acajeros<1 || Acajeros>10){
			printf("Numero de cajeros no valido (0<N<11).\n");
		}
	}while(Acajeros<1 || Acajeros>10);
	
	/***Generar cajas abiertas aleatoriamente***/
	for(i=0;i<10;i++){
		cajasB[i]=FALSE;
	}
	
	srand(time(NULL));
	while(conteo!=Acajeros){
		i=rand()%10;
		if(cajasB[i]==FALSE){
			cajasB[i]=TRUE;
			conteo++;
		}
	}
    
    /***Pedir tiempo de consulta de los cajeros***/
    printf("¿Cual es el tiempo de atención de cada cajero?\n");
	do{
		scanf("%d", &AcajerosT);
		if(AcajerosT<10 || AcajerosT%10 != 0){
			printf("Tiempo invalido\n");
		}
	}while(AcajerosT<10 || AcajerosT%10 != 0);
	
	/***Pedir el tiempo de llegada de los clientes***/
	printf("¿Cual es el tiempo de llegada de los clientes del banco?\n");	
	do{
		scanf("%d", &AclienteC);
		if(AclienteC<10 || AclienteC%10 != 0){
			printf("Tiempo invalido\n");
		}
	}while(AclienteC<10 || AclienteC%10 != 0);
	
	/***Pedir el tiempo de llegada de los usuarios***/
	printf("¿Cual es el tiempo de llegada de los usuarios del banco?\n");	
	do{
		scanf("%d", &AclienteU);
		if(AclienteU<10 || AclienteU%10 != 0){
			printf("Tiempo invalido\n");
		}
	}while(AclienteU<10 || AclienteU%10 != 0);
	
	/***Pedir el tiempo de llegada de los preferentes***/
	printf("¿Cual es el tiempo de llegada de los usuarios preferentes?\n");	
	do{
		scanf("%d", &AclienteP);
		if(AclienteP<10 || AclienteP%10 != 0){
			printf("Tiempo invalido\n");
		}
	}while(AclienteP<10 || AclienteP%10 != 0);
	
	*cajeros = Acajeros;
	*cajerosT = AcajerosT;
	*clienteC = AclienteC;
	*clienteU = AclienteU;
	*clienteP = AclienteP;
}

/*
	void Simulacion(int cajeros, int cajerosT, boolean cajasB[], int clienteC, int clienteU, int clienteP)
	Recibe:	int Número de cajeros, int Tiempo de atención de los cajeros,
					boolean Arreglo de cajas siendo ocupadas, int Tiempo de llegada de un Cliente,
					int Tiempo de llegada de un Usuario, int Tiempo de llegada de un Preferente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que simulara el comportamiento de un banco con base en las variables dadas por el
					usuario, permitiendo la llegada de clientes, usuarios y preferentes a su respectiva cola
					siendo atendidos en las cajas respetando las normas de un banco siendo que el banco no
					cerrara.
*/
void Simulacion(int cajeros, int cajerosT, boolean cajasB[], int clienteC, int clienteU, int clienteP){
	int tiempo = 0, clientesN[] = {0,0,0}, atendidos = 0;
	int clientesT[] = {clienteC, clienteU, clienteP};
	int controlC=0, controlU=0;
	int i, cajaC, aux;
	boolean bucle1, bucle2;
	elemento e;
	
	//Inicializar la función Rand
	srand(time(NULL));
	
	//Crear filas
	cola filas[3];
	
	//Inicializar filas
	for(i=0;i<3;i++){
		Initialize(&filas[i]);
	}
	
	//Crear las colas
	cola cajerosC[cajeros];
	int tiempoC[cajeros];
	int atendidosC[cajeros];
	
	//Inicializar colas
	for(i=0;i<cajeros;i++){
		Initialize(&cajerosC[i]);
		tiempoC[i] = 0;
		atendidosC[i] = 0;
	}
	
	//Iniciar Banco
	AbrirBanco(cajeros, cajasB);
	
	//Ciclo infinito de la simulación
	while(1){
		EsperarMiliSeg(TIEMPO_BASE); 	//Esperar el tiempo base
		tiempo++;		//Incrementar el contador de tiempo
		
		//Si el tiempo es multiplo del tiempo de atencion de los cajeros
		for(i=0;i<cajeros;i++){
			if ((tiempo+tiempoC[i]) % cajerosT  == 0){
				if (!Empty(&cajerosC[i])){
					//Quitar Cliente de la Caja
					e = QuitarClienteCaja(cajasB, &cajerosC[i], i);
					atendidos++;
					atendidosC[i]++;
					MoverCursor(30,4);
					printf("%d", atendidos);
					MoverCursor(62,4);
					printf("C%d - %c%d    ", i+1, e.c, e.n);
				}
			}
		}
		
		//Si el tiempo es multiplo del de llegada de los clientes, usuarios y preferentes
		for(i=0;i<3;i++){
			if(tiempo % clientesT[i] == 0){
				clientesN[i]++;			//Incrementar el numero de clientes
				
				//Agregar Cliente a la Fila
				AgregarClienteFila(&filas[i], clientesN[i], i);
			}
	    }
	    
	    //Verificar si hay cajas para nuevos clientes, usuarios o preferentes
	    bucle1=TRUE;
	    while(bucle1){
	    	//Verificando cajas disponibles
			aux=0;
		    for(i=0;i<cajeros;i++){
		    	if (Empty(&cajerosC[i]))
		    		aux++;
			}
			if(aux!=0){
				//Verificando filas
				aux=-1;
				if(!Empty(&filas[2])){
					if(aux==-1 && controlC!=2 && controlU!=5){
						aux=2;
						controlC++;
						controlU++;
					}
				}
				if(!Empty(&filas[0])){
					if(aux==-1 && controlU!=5){
						aux=0;
						controlU++;
						controlC = 0;
					}
				} else {
					controlC = 0;
				}
				if(!Empty(&filas[1])){
					if(aux==-1){
						aux=1;
						controlU=0;
					}
				} else {
					controlU=0;
				}
				
				if(aux!=-1){
					bucle2=TRUE;
					while(bucle2){
						cajaC=rand()%cajeros;
						if(Empty(&cajerosC[cajaC])){
							bucle2=FALSE;
						}
					}
					//Agregar Cliente a la Caja
					AgregarClienteCaja(cajasB, &cajerosC[cajaC], &filas[aux], cajaC, aux);
				} else {
					bucle1=FALSE;
				}
			} else {
				bucle1=FALSE;
			}
		}
	}
	return;
}

/*
	void AgregarClienteCaja(boolean cajasB[], cola *cajeroC, cola *filaC, int c, int f)
	Recibe:	boolean Arreglo de cajas siendo ocupadas, cola * Referencia/Dirección a la cola de un Cajero,
					cola * Referencia/Dirección a la cola de una Fila, int Número de Cajero, int Número de Fila
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que Mostrara de manera animada como uno de los tipos de usuario dependiendo del número
					de fila, se pone delante de una caja para ser atendido.
*/
void AgregarClienteCaja(boolean cajasB[], cola *cajeroC, cola *filaC, int c, int f){
	int fila=9, columna=5;
	int i, conteo=0;
	elemento e, aux;
	
	aux = QuitarClienteFila(filaC, f);
	
	for(columna=5,i=0;i<10;columna+=7,i++){
		if(cajasB[i]==TRUE){
			if(conteo==c){
				break;
			}
			conteo++;
		}
	}
	
	MoverCursor(columna,fila);
	if(aux.n<10){
		printf("  %c%d", aux.c, aux.n);
	} else if (aux.n<1000){
		printf(" %c%d", aux.c, aux.n);
	} else {
		printf("%c%d", aux.c, aux.n);
	}
	
	EsperarMiliSeg(20);
	e.n = aux.n;
	e.c = aux.c;
	Queue(cajeroC, e);
}

/*
	elemento QuitarClienteCaja(boolean cajasB[], cola *cajeroC, int c)
	Recibe:	boolean Arreglo de cajas siendo ocupadas, cola * Referencia/Dirección a la cola de un Cajero, int Número de Cajero
	Devuelve:	elmento Elemento
	Observaciones:	Función que Mostrara de manera animada como con un número de caja dada, el cliente actual se borrara de está y
					se retornara como un elemento.
*/
elemento QuitarClienteCaja(boolean cajasB[], cola *cajeroC, int c){
	int fila=9, columna=5;
	int i, conteo=0;
	elemento e;
	
	for(columna=5,i=0;i<10;columna+=7,i++){
		if(cajasB[i]==TRUE){
			if(conteo==c){
				break;
			}
			conteo++;
		}
	}
	
	MoverCursor(columna,fila);
	printf("      ");
	
	EsperarMiliSeg(20);
	e = Dequeue(cajeroC);
	return e;
}

/*
	void AgregarClienteFila(cola *filaC, int clienteN, int tipo)
	Recibe:	cola * Referencia/Dirección a la cola de una Fila, int Número de Cliente, int Tipo de cliente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que Mostrara de manera animada como una de las filas se le añade un nuevo cliente,
					con base en que tipo de cliente es.
*/
void AgregarClienteFila(cola *filaC, int clienteN, int tipo){
	int fila=12, columna=28+(9*tipo);
	elemento e;
	int tam;
	
	e.n = clienteN;
	if(tipo==0)
		e.c='C';
	if(tipo==1)
		e.c='U';
	if(tipo==2)
		e.c='P';
	
	if(Empty(filaC)){
		MoverCursor(columna,fila);
		if(clienteN<10){
			printf("  %c%d", e.c, e.n);
		} else if (clienteN<1000){
			printf(" %c%d", e.c, e.n);
		} else {
			printf("%c%d", e.c, e.n);
		}
	} else {
		tam = Size(filaC);
		if(tam<9){
			MoverCursor(columna,fila+tam);
			if(clienteN<10){
				printf("  %c%d", e.c, e.n);
			} else if (clienteN<1000){
				printf(" %c%d", e.c, e.n);
			} else {
				printf("%c%d", e.c, e.n);
			}
		} else {
			tam-=8;
			MoverCursor(columna,fila+9);
			if(tam<10){
				printf("  +%d", tam);
			} else if (tam<1000){
				printf(" +%d", tam);
			} else {
				printf("+%d", tam);
			}
		}
	}
	
	EsperarMiliSeg(20);
	Queue(filaC, e);
}

/*
	elemento QuitarClienteFila(cola *filaC, int tipo)
	Recibe:	cola * Referencia/Dirección a la cola de una Fila, int Tipo de cliente
	Devuelve:	elmento Elemento
	Observaciones:	Función que Mostrara de manera animada como con un tipo de cliente, se quitara ese cliente 
					de esa Fila y se retornara como un elemento.
*/
elemento QuitarClienteFila(cola *filaC, int tipo){
	int fila=12, columna=28+(9*tipo);
	int tam, i;
	elemento e, aux;
	
	e = Dequeue(filaC);
	
	MoverCursor(columna,fila);
	if(Empty(filaC)){
		printf("      ");
	} else {
		tam = Size(filaC);
		
		for(i=0;i<=tam;i++){
			MoverCursor(columna,fila+i);
			printf("      ");
			MoverCursor(columna,fila+i);
			
			if(i==tam)break;
			if(i==9)break;
			
			aux = Element(filaC, i+1);
			if(aux.n<10){
				printf("  %c%d", aux.c, aux.n);
			} else if (aux.n<1000){
				printf(" %c%d", aux.c, aux.n);
			} else {
				printf("%c%d", aux.c, aux.n);
			}
		}
		if(tam>9){
			tam-=9;
			if(tam<10){
				printf("  +%d", tam);
			} else if (tam<1000){
				printf(" +%d", tam);
			} else {
				printf("+%d", tam);
			}
		}
	}
	
	EsperarMiliSeg(10);
	return e;
}

/*
	void AbrirBanco(int cajeros, boolean cajasB[])
	Recibe:	int Número de Cajeros, boolean Arreglo de cajas siendo ocupadas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que abre el banco de manera animada en pantalla.
*/
void AbrirBanco(int cajeros, boolean cajasB[]){
	DibujaPresentacion();
	BorrarPantalla();
	DibujaMarco();
	DibujaCajas(cajeros, cajasB);
	DibujaFilas();
	DibujaEstantes();
	DibujaAnuncioAbrir();
}

/*
	void DibujaPresentacion()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da una presentación a la simulación 03.
*/
void DibujaPresentacion(){
	int columna, fila, i;
	
	BorrarPantalla();
	
	//Crear Particulas Aleatorias
	srand(time(NULL));
	EsperarMiliSeg(2200);
    for (i=1;i<=500;i++) {
    	columna=(rand()%(ANCHO-1))+1;
        fila=(rand()%(ALTO-1))+1;
        MoverCursor(columna,fila);
        printf("*");
        if(i%100 == 0){
        	EsperarMiliSeg(800);
		}
    }
    
    //Crear Primer Cuadro
    EsperarMiliSeg(2200);
    for(columna=22;columna<=57;columna++){
    	for(fila=4;fila<=13;fila++){
    		MoverCursor(columna,fila);
    		if((columna>22 && columna<57) && (fila==4 || fila==13)){
    			printf("_");
			} else if ((fila>4 && fila<=13) && (columna==22 || columna==57)){
				printf("|");
			} else {
				printf(" ");
			}
		}
	}
	
	//Poner datos
	EsperarMiliSeg(1800);
	MoverCursor(28,6);
	printf("Erendil Aguilar Avendaño");
	MoverCursor(28,9);
	printf("Iker Itzae Aguilar Souza");
	MoverCursor(35,12);
	printf("PRESENTAN");
	
	//Crear Segundo Cuadro
    EsperarMiliSeg(2200);
    for(columna=22;columna<=57;columna++){
    	for(fila=17;fila<=20;fila++){
    		MoverCursor(columna,fila);
    		if((columna>22 && columna<57) && (fila==17 || fila==20)){
    			printf("_");
			} else if ((fila>17 && fila<=20) && (columna==22 || columna==57)){
				printf("|");
			} else {
				printf(" ");
			}
		}
	}
	
	//Poner datos
	EsperarMiliSeg(1800);
	MoverCursor(34,19);
	printf("SIMULACION 03");
	
	EsperarMiliSeg(5000);
	BorrarPantalla();
}

/*
	void DibujaMarco()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea el todo el marco del Banco así como sus datos iniciales.
*/
void DibujaMarco(){
	int columna,fila,i;
	int Esperar = 5;
	
	//Crear columnas
	for(columna=1,fila=2;fila<ALTO;fila++){
		//Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
		if(fila<6){
			MoverCursor(columna,fila);
			printf("|");
			MoverCursor(ANCHO-1,fila);
			printf("|");
		} else {
			MoverCursor(columna+1,fila);
			printf("||");
			MoverCursor(ANCHO-3,fila);
			printf("||");
		}
		EsperarMiliSeg(Esperar);
	}
	
	//Crear filas
	for(columna=2,fila=1;columna<ANCHO-1;columna++){
		//Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
		MoverCursor(columna,fila);
		printf("_");
		MoverCursor(columna,fila+4);
		printf("_");
		if(columna>3 && columna<ANCHO-3){
			MoverCursor(columna,ALTO-1);
			printf("_");
		}
		EsperarMiliSeg(Esperar);
	}
	
	//Crear texto
	MoverCursor(27,3);
	printf("\"Banco Nacional de México\"");
	EsperarMiliSeg(200);
	MoverCursor(5,4);
	printf("Num. Clientes Atendidos: 0");
	EsperarMiliSeg(200);
	MoverCursor(42,4);
	printf("Ultimo movimiento:");
	EsperarMiliSeg(200);
}

/*
	void DibujaCajas(int cajeros, boolean cajasB[])
	Recibe:	int Número de Cajeros, boolean Arreglo de cajas siendo ocupadas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea las cajas que usara el Banco con base a un número de cajas
					dado y con un arreglo que cajas usar.
*/
void DibujaCajas(int cajeros, boolean cajasB[]){
	int columna=4,fila=6,i,j;
	int Esperar = 5;
	int conteo = 1;
	
	//Crear Cajas
	for(columna=4,i=1,fila=6;i<=10;columna+=7,i++){
		//***Crear Cajas***//
		//Lineas
		for(j=1;j<=6;j++){
			MoverCursor(columna+j,fila+2);
			printf("_");
			if(i==10){
				printf("_");
			}
			EsperarMiliSeg(Esperar);
		}
		for(j=0;j<5;j++){
			if(j==3||j==4){
				MoverCursor(columna,fila+j);
				if(i==1){
					printf(":");
				} else {
					printf("-");
				}
				if(i==10){
					MoverCursor(columna+8,fila+j);
					printf(":");
				} else {
					MoverCursor(columna+7,fila+j);
					printf("-");
				}
				continue;
			}
			MoverCursor(columna,fila+j);
			printf("|");
			MoverCursor(columna+(i==10?8:7),fila+j);
			printf("|");
			EsperarMiliSeg(Esperar);
		}
		if(cajasB[i-1]){
			MoverCursor(columna+3,fila+1);
			printf("C%d", conteo);
			conteo++;
		} else {
			for(j=0;j<2;j++){
				MoverCursor(columna+1,fila+j);
				printf("//////");
				if(i==10){
					printf("/");
				}
			}
		}
	}
}

/*
	void DibujaFilas()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea las Filas que usaran los 3 tipos de clientes.
*/
void DibujaFilas(){
	int columna=25,fila=11,i,j;
	int Esperar = 5;
	
	//Crear Cajas
	for(columna=25,i=1,fila=11;i<=4;columna+=9,i++){
		//***Crear Lineas***//
		for(j=0;j<11;j++){
			if(j==0){
				MoverCursor(columna,fila+j);
				printf(" _ ");
				EsperarMiliSeg(Esperar);
				continue;
			}
			if(j==10){
				MoverCursor(columna,fila+j);
				printf("|_|");
				EsperarMiliSeg(Esperar);
				continue;
			}
			MoverCursor(columna,fila+j);
			printf("|||");
			EsperarMiliSeg(Esperar);
		}
	}
}

/*
	void DibujaEstantes()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que dibuja Estantes de decoración para la simulación.
*/
void DibujaEstantes(){
	int columna=6,fila=12,i,j;
	int Esperar = 5;
	int conteo = 1;
	
	//Crear Cajas
	for(columna=6,i=1,fila=12;i<=5;fila+=2,i++){
		//***Crear Estantes***//
		if(i%2==0){
			MoverCursor(columna,fila);
			printf("[$$$$$$$$$$$$$]");
			MoverCursor(ANCHO-20,fila);
			printf("[$$$$$$$$$$$$$]");
		} else {
			MoverCursor(columna,fila);
			printf("[#############]");
			MoverCursor(ANCHO-20,fila);
			printf("[#############]");
		}
		EsperarMiliSeg(Esperar);
	}
}

/*
	void DibujaAnuncioAbrir()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da un anunció en pantalla de que el banco va a abrir.
*/
void DibujaAnuncioAbrir(){
	int columna=39, fila=16, i;
	int Esperar = 800;
	
	//Escribir Conteo
	for(i=3;i>0;i--){
		MoverCursor(columna,fila);
		EsperarMiliSeg(Esperar);
		printf("%d", i);
	}
	EsperarMiliSeg(Esperar);
	
	//Poner GO
	MoverCursor(columna,fila);
	printf("GO");
	EsperarMiliSeg(Esperar);
	
	//Quitar GO
	MoverCursor(columna,fila);
	printf("  ");
	EsperarMiliSeg(Esperar);
}

