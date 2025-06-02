/*
Autores:	Erendil Aguilar Avendaño
			Iker Itzae Aguilar Souza
Versión 1.6 (03 de Junio 2024)
Grupo: 		2CM2
Materia:	Algoritmos y Estructuras de Datos
Práctica 04: Simulaciones con el TAD Cola

=== Simulaciones con el TAD Cola ===

Descripción: Programa que con el uso de colas simula el comportamiento de un Supermercado de manera animada.

Observaciones: El programa requerira de la libreria "presentacion.h", la cuál tiene las implementaciones
para mover el cursor de la pantalla, esperar un tiempo y borrar pantalla, asi como las librerias "TADColaEst.h"
y "TADCola/TADColaDin.h" para el uso correcto de las colas, la compilación debera incluir las definiciones de 
las funciones según la plataforma que se este utilizando (Windows o Linux).

Compilación: 	gcc -o simulacion_01 simulacion_01.c presentacion/presentacion(Win|Lin).o TADCola/TADCola(Din|Est|EstCirc).o (Win si se correra en Windows | Lin si se ejecutará en Linux)  "Si se tiene el código objeto"
				gcc -o simulacion_01 simulacion_01.c presentacion/presentacion(Win|Lin).c TADCola/TADCola(Din|Est|EstCirc).c (Win si se correra en Windows | Lin si se ejecutará en Linux)  "Si se tiene el código fuente"
Ejecución: Windows simulacion_01.exe  &  Linux ./simulacion_01
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
void PedirRequisitos(char *tienda, int *cajas, int *cajasT, int *clienteT);

void Simulacion(char tienda[], int cajas, int cajasT[], int clienteT);

void AgregarCliente(cola *cajera, int cajas, int clienteN, int filaC);
elemento QuitarCliente(cola *cajera, int cajas, int filaC);

void AbrirTienda(char tienda[], int cajas);
void CerrarTienda(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo);

void DibujaPresentacion();
void DibujaMarco(char tienda[]);
void DibujaCajas(int cajas);
void DibujaEstantes(int cajas);
void DibujaCortina();
void DibujaCartel();
void DibujaCartelDatos(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo);
void DibujaAnuncioAbrir();
void DibujaAnuncioCerrar();

//PROGRAMA PRINCIPAL
int main(void)
{
	char tienda[52];
	int cajas, cajasT[10], clienteT;
	
	setlocale(LC_ALL, "");
	
	/***PEDIR REQUISITOS***/
	PedirRequisitos(tienda, &cajas, cajasT, &clienteT);
	
	/***INCIAR LA TIENDA***/
	Simulacion(tienda, cajas, cajasT, clienteT);
	
	MoverCursor(ANCHO,ALTO);
}

/*
	void PedirRequisitos(char *tienda, int *cajas, int *cajasT, int *clienteT)
	Recibe:	char * Referencia/Dirección a la cadena del nombre de la tienda, int * Referencia/Dirección al numero de cajas, 
					int * Referencia/Dirección al arreglo de los tiempos de cajas, int * Referencia/Dirección al tiempo de llegada del cliente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que pide los datos como el nombre de la tienda, el numero de cajas, tiempo de consulta de cajas y el tiempo
					de llegada de los clientes, poniendo los resultados en las variables dadas.
*/
void PedirRequisitos(char *tienda, int *cajas, int *cajasT, int *clienteT){
	int i;
	int Acajas, AclienteT;
	char ch, Atienda[52], c;
	char correcto = 0;
	
	BorrarPantalla();
	
	/***Pedir el Nombre de la Tienda***/
	printf("¿Como se llamara su tienda?\n");
	do{
		fgets(Atienda, sizeof(Atienda), stdin);
		// Verificar si la longitud de la cadena ingresada excede 50 caracteres (sin contar el '\n' de fgets)
	    if (strlen(Atienda) == 51 && Atienda[50] != '\n') {
	        printf("El nombre de la tienda tiene más de 50 caracteres.\n");
	        while (getchar() != '\n');
	        //exit(1);
	    } else {
	    	//En caso de un salto de linea
	        if (Atienda[strlen(Atienda) - 1] == '\n') {
	            Atienda[strlen(Atienda) - 1] = '\0';
	        }
	        correcto = 1;
	    }
	}while(correcto == 0);
    
	/***Pedir el numero de cajeros***/
	printf("¿Cuantos cajeros quiere que su tienda tenga?\n");
    do{
		scanf("%d", &Acajas);
		if(Acajas<1 || Acajas>10){
			printf("Numero de cajas no valido (0<N<11).\n");
		}
	}while(Acajas<1 || Acajas>10);
    
    /***Pedir los tiempos de consulta de los cajeros***/
    printf("¿Cuanto tiempo se tardara cada cajero en atender a los clientes?\n");
	for(i=0;i<Acajas;i++){
		do{
			scanf("%d", &cajasT[i]);
			if(cajasT[i]<10 || cajasT[i]%10 != 0){
				printf("Tiempo invalido\n");
			}
		}while(cajasT[i]<10 || cajasT[i]%10 != 0);
	}
	
	/***Pedir el tiempo de llegada de los clientes***/
	printf("¿Cual es el tiempo de llegada de los clientes?\n");	
	do{
		scanf("%d", &AclienteT);
		if(AclienteT<10 || AclienteT%10 != 0){
			printf("Tiempo invalido\n");
		}
	}while(AclienteT<10 || AclienteT%10 != 0);
	
	strncpy(tienda, Atienda, 52);
	*cajas = Acajas;
	*clienteT = AclienteT;
}

/*
	void Simulacion(char tienda[], int cajas, int cajasT[], int clienteT)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas, int Arreglo de los tiempos de cajas, int Tiempo de llegada del cliente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que simulara el comportamiento de un supermercado con base en las variables dadas por el
					usuario, permitiendo la llegada de clientes a colas, siendo atendidos en cajas para llegar a 
					la cantidad de 100 clientes para que pueda parar la simulación, solo si no hay mas clientes en
					las colas.
*/
void Simulacion(char tienda[], int cajas, int cajasT[], int clienteT){
	int tiempo = 0, clienteN = 0, atendidos = 0;
	int i, filaC, aux;
	elemento e;
	
	//Inicializar la función Rand
	srand(time(NULL));
	
	//Crear las colas
	cola cajeras[cajas];
	int tiempoC[cajas];
	int atendidosC[cajas];
	
	//Inicializar colas
	for(i=0;i<cajas;i++){
		Initialize(&cajeras[i]);
		tiempoC[i] = 0;
		atendidosC[i] = 0;
	}
	
	//Iniciar Tienda
	AbrirTienda(tienda, cajas);
	
	//Ciclo infinito de la simulación
	while(1){
		EsperarMiliSeg(TIEMPO_BASE); 	//Esperar el tiempo base
		tiempo++;		//Incrementar el contador de tiempo
		
		//Si el tiempo es multiplo del tiempo de atencion de alguna caja
		for(i=0;i<cajas;i++){
			if ((tiempo+tiempoC[i]) % cajasT[i]  == 0){
				if (!Empty(&cajeras[i])){
					//Quitar Cliente de la Caja
					e = QuitarCliente(&cajeras[i], cajas, i);
					atendidos++;
					atendidosC[i]++;
					MoverCursor(30,4);
					printf("%d", atendidos);
					MoverCursor(62,4);
					printf("C%d - %d    ", i+1, e.n);
				}
			}
		}
		
		//Si ya no hay mas clientes que atender
		if(atendidos>=100){
			aux = 0;
			for(i=0;i<cajas;i++){
				if (!Empty(&cajeras[i])){
					aux++;
				}
			}
			if(aux==0){
				CerrarTienda(tienda, cajas, atendidos, atendidosC, tiempo);
				break;
			}
		}
		
		//Si el tiempo es multiplo del de llegada de los clientes
		if(tiempo % clienteT == 0){
			clienteN++;				//Incrementar el numero de clientes
			filaC=rand()%cajas;		//Escoger la fila para formarse aleatoriamente
			
			//Mantener el control del tiempo de cuando una caja empezo a tener clientes
			if(Empty(&cajeras[filaC])){
				tiempoC[filaC] = (cajasT[filaC] - (tiempo % cajasT[filaC])) % cajasT[filaC];
			}
			
			//Agregar Cliente a la Caja
			AgregarCliente(&cajeras[filaC], cajas, clienteN, filaC);
		}
	}
	return;
}

/*
	void AgregarCliente(cola *cajera, int clienteN, int filaC)
	Recibe:	cola * Referencia/Dirección a la cola de un cajero, int Numero de clientes, int Numero de Caja
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que agrega un cliente a la cola de un cajero, mostrando tambien como este es
					ingresado a la cola en pantalla.
*/
void AgregarCliente(cola *cajera, int cajas, int clienteN, int filaC){
	int espacio=5+(7*filaC)+((cajas==10)?0:((cajas%2==0)?0:4)+((10-cajas)/2)*7);
	int fila=10, columna=espacio;
	elemento e;
	int tam;
	
	if(Empty(cajera)){
		MoverCursor(columna,fila);
		if(clienteN<100){
			printf("  %d", clienteN);
		} else if (clienteN<10000){
			printf(" %d", clienteN);
		} else {
			printf("%d", clienteN);
		}
	} else {
		tam = Size(cajera);
		fila++;
		if(tam<10){
			MoverCursor(columna,fila+tam);
			if(clienteN<100){
				printf("  %d", clienteN);
			} else if (clienteN<10000){
				printf(" %d", clienteN);
			} else {
				printf("%d", clienteN);
			}
		} else {
			tam-=9;
			MoverCursor(columna,fila+10);
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
	e.n = clienteN;
	Queue(cajera, e);
}

/*
	elemento QuitarCliente(cola *cajera, int filaC)
	Recibe:	cola * Referencia/Dirección a la cola de un cajero, int Numero de Caja
	Devuelve:	elemento Elemento
	Observaciones:	Función que quita un cliente de cola de un cajero, mostrando tambien como este es
					eliminado de la cola en pantalla, retornando el valor del elemento.
*/
elemento QuitarCliente(cola *cajera, int cajas, int filaC){
	int espacio=5+(7*filaC)+((cajas==10)?0:((cajas%2==0)?0:4)+((10-cajas)/2)*7);
	int fila=10, columna=espacio;
	int tam, i;
	elemento e, aux;
	
	e = Dequeue(cajera);
	
	MoverCursor(columna,fila);
	if(Empty(cajera)){
		printf("      ");
	} else {
		tam = Size(cajera);
		
		for(i=0;i<=tam+1;i++){
			MoverCursor(columna,fila+i);
			printf("      ");
			MoverCursor(columna,fila+i);
			
			if(i==tam+1)break;
			if(i==11)break;
			
			aux = Element(cajera, i==0?1:i);
			if(aux.n<100){
				printf("  %d", aux.n);
			} else if (aux.n<10000){
				printf(" %d", aux.n);
			} else {
				printf("%d", aux.n);
			}
			
			if(i==0)i++;
		}
		if(tam>10){
			tam-=10;
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
	void AbrirTienda(char tienda[], int cajas)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que abre la tienda de manera animada en pantalla.
*/
void AbrirTienda(char tienda[], int cajas){
	DibujaPresentacion();
	DibujaMarco(tienda);
	DibujaCajas(cajas);
	DibujaEstantes(cajas);
	DibujaAnuncioAbrir();
}

/*
	void CerrarTienda(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas, int Numero de clientes atendidos,
				int Arreglo de clientes atendidos por caja, int Tiempo de ejecución
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que cierra la tienda de manera animada en pantalla.
*/
void CerrarTienda(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo){
	DibujaAnuncioCerrar();
	EsperarMiliSeg(600);
	DibujaCortina();
	EsperarMiliSeg(1200);
	DibujaCartel();
	DibujaCartelDatos(tienda, cajas, atendidos, atendidosC, tiempo);
}

/*
	void DibujaPresentacion()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da una presentación a la simulación 01.
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
	printf("SIMULACION 01");
	
	EsperarMiliSeg(5000);
	BorrarPantalla();
}

/*
	void DibujaMarco(char tienda[])
	Recibe:	char Cadena del nombre de la tienda
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea el todo el marco del Supermercado así como sus datos iniciales.
*/
void DibujaMarco(char tienda[]){
	int columna,fila,i;
	int Esperar = 5;
	
	//Crear columnas
	for(columna=1,fila=2;fila<ALTO;fila++){
		//Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
		MoverCursor(columna,fila);
		printf("|");
		MoverCursor(ANCHO-1,fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	
	//Crear filas
	for(columna=2,fila=1;columna<ANCHO-1;columna++){
		//Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
		MoverCursor(columna,fila);
		printf("_");
		MoverCursor(columna,fila+4);
		printf("_");
		MoverCursor(columna,ALTO-1);
		printf("_");
		EsperarMiliSeg(Esperar);
	}
	
	//Crear texto
	MoverCursor(5,3);
	printf("Tienda:\"%s\"", tienda);
	EsperarMiliSeg(200);
	MoverCursor(5,4);
	printf("Num. Clientes Atendidos: 0");
	EsperarMiliSeg(200);
	MoverCursor(42,4);
	printf("Ultimo movimiento:");
	EsperarMiliSeg(200);
	MoverCursor(65,3);
	printf("[[       ]]");
	EsperarMiliSeg(Esperar);
}

/*
	void DibujaCajas(int cajas)
	Recibe:	int Numero de cajas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea las cajas que usara el Supermercado con base en un 
					valor de cajas dado.
*/
void DibujaCajas(int cajas){
	int espacio=4+((cajas==10)?0:((cajas%2==0)?0:4)+((10-cajas)/2)*7);
	int columna=espacio,fila=6,i,j;
	int Esperar = 5;
	
	//Recorrer cada columna
	for(columna=espacio,i=1;i<=cajas;columna+=7,i++){
		//Crear Caja
		for(j=1;j<=6;j++){
			MoverCursor(columna+j,fila);
			printf("_");
			MoverCursor(columna+j,fila+3);
			printf("_");
			EsperarMiliSeg(Esperar);
		}
		
		//Crear Lineas
		for(j=1;j<=15;j++){
			if(j==4){
				MoverCursor(columna,fila+j);
				if(i==1){
					printf(":");
				} else {
					printf("-");
				}
				MoverCursor(columna+7,fila+j);
				if(i==cajas){
					printf(":");
				} else {
					printf("-");
				}
				continue;
			}
			if(j==5){
				MoverCursor(columna,fila+j);
				if(i==1){
					printf(":");
				} else {
					printf(".");
				}
				MoverCursor(columna+7,fila+j);
				if(i==cajas){
					printf(":");
				} else {
					printf(".");
				}
				continue;
			}
			MoverCursor(columna,fila+j);
			printf("|");
			MoverCursor(columna+7,fila+j);
			printf("|");
			EsperarMiliSeg(Esperar);
		}
		MoverCursor(columna+(i==10?2:3),fila+2);
		printf("C%d",i);
	}
}

/*
	void DibujaEstantes(int cajas)
	Recibe:	int Numero de cajas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que dibuja Estantes de decoración para la simulación.
*/
void DibujaEstantes(int cajas){
	int espacio=3+(7*cajas)+((cajas==10)?0:((cajas%2==0)?0:4)+((10-cajas)/2)*7);
	int columna=4,fila=6,i,j,k;
	int Esperar = 5;
	int conteo = 1;
	
	//***Comprobar que hay menos de 10 cajas***//
	//if(cajas==10) return;
	
	//***Crear Estantes Grandes***//
	for(columna=4,fila=6,i=1;i<=2;columna+=espacio,i++){
		for(j=0;j<5;j++){
			if(j==0){
				MoverCursor(columna+1,fila+j);
				for(k=0;k<(10-cajas)*3-2;k++){
					printf("_");
				}
			} else {
				MoverCursor(columna,fila+j);
				if(j%2!=0){
					for(k=0;k<10-cajas;k++){
						if(k%2==0) printf("|/|");
						else printf("|\\|");
					}
				} else {
					for(k=0;k<10-cajas;k++){
						if(k%2==0) printf("|\\|");
						else printf("|/|");
					}
				}
			}
			EsperarMiliSeg(Esperar);
		}
	}
	
	//***Crear Estantes Chicos***//
	for(columna=4,fila=12,i=1;i<=2;columna+=espacio,i++){
		for(j=0;j<5;j++){
			MoverCursor(columna,fila+j*2);
			for(k=0;k<10-cajas;k++){
				printf("[#]");
			}
			EsperarMiliSeg(Esperar);
		}
	}
}

/*
	void DibujaCortina()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea una cortina desendente en la pantalla.
*/
void DibujaCortina(){
	int columna,fila,i,j;
	
	EsperarMiliSeg(800);
	
	for(fila=1,columna=1;fila<ALTO;fila++){
		MoverCursor(columna,fila);
		printf("|||");
		MoverCursor(ANCHO-4,fila);
		printf("||||");
		
		if(fila-1>0){
			MoverCursor(columna,fila-1);
			printf("| |");
			for(i=0;i<14;i++)
				printf(" |__|");
			printf(" |  |");
		}
		
		if(fila-2>0){
			MoverCursor(columna,fila-2);
			printf("| |");
			for(i=0;i<15;i++)
				printf("_|  |");
		}
		
		if(fila-3>0){
			MoverCursor(columna,fila-3);
			printf("| |");
			for(i=0;i<15;i++)
				printf(" |  |");
		}
		
		EsperarMiliSeg(200);
	}
	MoverCursor(4,ALTO-1);
	for(i=0;i<14;i++)
		printf("     ");
	printf(" ");
	
	EsperarMiliSeg(1500);
}

/*
	void DibujaCartel()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea un cartel desendente en la pantalla.
*/
void DibujaCartel(){
	int columna,fila,i,j;
	
	for(fila=1,columna=9;fila<ALTO-4;fila++){
		MoverCursor(columna,fila);
		printf("|");
		for(i=0;i<59;i++)
			printf("_");
		printf("|");
		
		if(fila-1>0){
			MoverCursor(columna,fila-1);
			printf("|");
			for(i=0;i<59;i++)
				printf(" ");
			printf("|");
		}
		
		if(fila-14>0){
			MoverCursor(columna,fila-14);
			printf(" ");
			for(i=0;i<59;i++)
				printf("_");
			printf(" ");
		}
		
		if(fila-15>0){
			MoverCursor(columna,fila-15);
			for(i=0;i<12;i++){
				if(i==2 || i==10)
					printf("||  |");
				else
					printf(" |  |");
			}
		}
		
		EsperarMiliSeg(200);
	}
}

/*
	void DibujaCartelDatos(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas, int Numero de clientes atendidos,
				int Arreglo de clientes atendidos por caja, int Tiempo de ejecución
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que agrega los datos al cartel del finalizamiento de la simulación.
*/
void DibujaCartelDatos(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo){
	int columna,fila,i,j,k;
	int mejorC = 0;
	
	//Calcular mejor Caja
	for(i=0;i<cajas;i++){
		if(mejorC < atendidosC[i]){
			mejorC = atendidosC[i];
		}
	}
	
	//Mostrar datos finales
	for(columna=12,fila=7,i=1;i<=5;fila+=2,i++){
		EsperarMiliSeg(600);
		MoverCursor(columna,fila);
		
		if(i==1){
			printf("\"%s\"", tienda);
			columna--;
		}
		
		if(i==2)
			printf("Tiempo abierta:    %d Milisegundos", tiempo);
		
		if(i==3)
			printf("Clientes Atendidos:    %d Clientes", atendidos);
			
		if(i==4){
			printf("Cajas mas eficientes:    ");
			for(j=0,k=0;j<cajas;j++){
				if(mejorC == atendidosC[j]){
					if(k>0)
						printf(",");
					printf("C%d", j+1);
					k++;
				}
			}
		}
		
		if(i==5){
			printf("Clientes Atendidos por Caja:");
			for(columna+=2,fila++,j=0;j<cajas;j++){
				EsperarMiliSeg(200);
				MoverCursor(columna,fila);
				printf("C%d: %d", j+1, atendidosC[j]);
				if((j+1)%3 != 0){
					fila++;
				} else {
					fila-=2;
					columna+=14;
				}
			}
		}
	}
}

/*
	void DibujaAnuncioAbrir()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da un anunció en pantalla de que la tienda va a abrir.
*/
void DibujaAnuncioAbrir(){
	int columna=67, fila=3, i;
	char anuncio[] = "ABIERTO";
	int Esperar = 800;
	
	//Escribir Conteo
	MoverCursor(columna,fila);
	for(i=3;i>0;i--){
		EsperarMiliSeg(Esperar);
		printf(" %d", i);
	}
	EsperarMiliSeg(Esperar);
	
	//Quitar Conteo
	MoverCursor(columna,fila);
	printf("       ");
	EsperarMiliSeg(Esperar);
	
	//Escribir ABIERTO Lentamente
	MoverCursor(columna,fila);
	for(i=0;i<7;i++){
		EsperarMiliSeg(Esperar-300);
		printf("%c", anuncio[i]);
	}
	
	//Parpadeo de Aununcio
	for(i=0;i<4;i++){
		MoverCursor(columna,fila);
		EsperarMiliSeg(Esperar-200);
		if(i%2 == 0){
			printf("       ");
		} else {
			printf("%s", anuncio);
		}
	}
}

/*
	void DibujaAnuncioCerrar()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da un anunció en pantalla de que la tienda va a cerrar.
*/
void DibujaAnuncioCerrar(){
	int columna=67, fila=3, i;
	char anuncio[] = "CERRADO";
	int Esperar = 800;
	
	//Quitar Cartel de Abierto
	MoverCursor(columna,fila);
	printf("       ");
	EsperarMiliSeg(Esperar);
	
	//Escribir Cerrado Lentamente
	MoverCursor(columna,fila);
	for(i=0;i<7;i++){
		EsperarMiliSeg(Esperar-300);
		printf("%c", anuncio[i]);
	}
	
	//Parpadeo de Aununcio
	for(i=0;i<8;i++){
		MoverCursor(columna,fila);
		EsperarMiliSeg(Esperar-200);
		if(i%2 == 0){
			printf("       ");
		} else {
			printf("%s", anuncio);
		}
	}
}

