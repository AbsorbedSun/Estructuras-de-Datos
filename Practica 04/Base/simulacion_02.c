/*
Autores:	Erendil Aguilar Avendaño
			Iker Itzae Aguilar Souza
Versión 1.4 (06 de Junio 2024)
Grupo: 		2CM2
Materia:	Algoritmos y Estructuras de Datos
Práctica 04: Simulaciones con el TAD Cola

=== Simulaciones con el TAD Cola ===

Descripción: Programa que con el uso de colas simula el comportamiento de las Ejecuciones de Procesos en un Sistema Operativo de manera animada.

Observaciones: El programa requerira de la libreria "presentacion.h", la cuál tiene las implementaciones
para mover el cursor de la pantalla, esperar un tiempo y borrar pantalla, asi como las librerias "TADColaEst.h"
y "TADCola/TADColaDin.h" para el uso correcto de las colas, la compilación debera incluir las definiciones de 
las funciones según la plataforma que se este utilizando (Windows o Linux).

Compilación: 	gcc -o simulacion_02 simulacion_02.c presentacion/presentacion(Win|Lin).o TADCola/TADCola(Din|Est|EstCirc).o (Win si se correra en Windows | Lin si se ejecutará en Linux)  "Si se tiene el código objeto"
				gcc -o simulacion_02 simulacion_02.c presentacion/presentacion(Win|Lin).c TADCola/TADCola(Din|Est|EstCirc).c (Win si se correra en Windows | Lin si se ejecutará en Linux)  "Si se tiene el código fuente"
Ejecución: Windows simulacion_02.exe  &  Linux ./simulacion_02
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
#define TIEMPO_BASE	1000	//Tiempo base en milisegundos

//FUNCIONES
void PedirRequisitos(cola *Faltantes, int *procesos);

void Simulacion(cola *Faltantes, int procesos);

void AgregarProceso(cola *Ejecucion, cola *Faltantes);
void QuitarProceso(cola *Faltantes);
void TerminarProceso(cola *Finalizados, cola *Faltantes);

void AbrirSistemaOperativo(cola *Faltantes);
void CerrarSistemaOperativo(cola *Finalizados);

void DibujaPresentacion();
void DibujaMarco();
void DibujaCuadros();
void DibujaElementoAgregar(elemento e, int tipo);
void DibujaElementoEliminar(int tipo);
void DibujaCortina();
void DibujaCartel();
void DibujaCartelDatos(cola *Finalizados);
void DibujaAnuncioAbrir();
void DibujaAnuncioCerrar();

//PROGRAMA PRINCIPAL
int main(void)
{
	cola Faltantes;
	int procesos;
	
	setlocale(LC_ALL, "");
	
	//INICIALIZAR COLA FALTANTES
	Initialize(&Faltantes);
	
	/***PEDIR REQUISITOS***/
	PedirRequisitos(&Faltantes, &procesos);
	
	/***INCIAR LA TIENDA***/
	Simulacion(&Faltantes, procesos);
	
	MoverCursor(ANCHO,ALTO);
}

/*
	void PedirRequisitos(cola *Faltantes, int *procesos)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Faltantes, int * Referencia/Dirección al número de procesos
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que pide los datos como le numero de procesos a ejecútar, con su respectivo, Nombre, Nombre de
						Actividad, Segundos para completar la Ejecución y un ID Generado automaticamente.
*/
void PedirRequisitos(cola *Faltantes, int *procesos){
	char Nombre[32], Actividad[52], ID[8], correcto, ch1, ch2, c;
	int Aprocesos, segundos, num1, num2, i, j, k;
	elemento e, aux;
	
	BorrarPantalla();
	
	/***Pedir el numero de procesos***/
	printf("¿Cuantos procesos quiere que se ejecuten en el sistema operativo?\n");
    do{
		scanf("%d", &Aprocesos);
		while ((c = getchar()) != '\n' && c != EOF);
		if(Aprocesos<1 || Aprocesos>100){
			printf("Numero de procesos no valido (0<N<101).\n");
		}
	}while(Aprocesos<1 || Aprocesos>100);
	
	for(i=0;i<Aprocesos;i++){
		/***Pedir el Nombre del Proceso***/
		correcto=0;
		printf("¿Como se llamara el proceso %d?\n", i+1);
		do{
			fgets(Nombre, sizeof(Nombre), stdin);
			// Verificar si la longitud de la cadena ingresada excede 30 caracteres (sin contar el '\n' de fgets)
		    if (strlen(Nombre) == 31 && Nombre[30] != '\n') {
		        printf("El nombre del proceso supera los 30 caracteres.\n");
		        while (getchar() != '\n');
		        //exit(1);
		    } else {
		    	//En caso de un salto de linea
		        if (Nombre[strlen(Nombre) - 1] == '\n') {
		            Nombre[strlen(Nombre) - 1] = '\0';
		        }
		        //Quitar espacios innecesarios en el nombre
			    for(j=0;j<strlen(Nombre);j++){
			    	if(Nombre[j] == ' '){
			    		while(Nombre[j+1] == ' '){
				    		for(k=j+1;k<strlen(Nombre);k++){
								Nombre[k] = Nombre[k+1];
							}
						}
						while(Nombre[j] == ' ' && (j==0 || j==strlen(Nombre)-1)){
							for(k=j;k<strlen(Nombre);k++){
								Nombre[k] = Nombre[k+1];
							}
						}
					}
				}
				if(strlen(Nombre)!=0){
					correcto = 1;
				} else {
					printf("El nombre del proceso no es valido.\n");
				}
		    }
		}while(correcto == 0);
		
		/***Pedir la Actividad del Proceso***/
		correcto=0;
		printf("¿Cual es la actividad el proceso %d?\n", i+1);
		do{
			fgets(Actividad, sizeof(Actividad), stdin);
			// Verificar si la longitud de la cadena ingresada excede 30 caracteres (sin contar el '\n' de fgets)
		    if (strlen(Actividad) == 51 && Actividad[50] != '\n') {
		        printf("El nombre de la actividad del proceso supera los 50 caracteres.\n");
		        while (getchar() != '\n');
		        //exit(1);
		    } else {
		    	//En caso de un salto de linea
		        if (Actividad[strlen(Actividad) - 1] == '\n') {
		            Actividad[strlen(Actividad) - 1] = '\0';
		        }
		        //Quitar espacios innecesarios en la Actividad
				for(j=0;j<strlen(Actividad);j++){
			    	if(Actividad[j] == ' '){
			    		while(Actividad[j+1] == ' '){
				    		for(k=j+1;k<strlen(Actividad);k++){
								Actividad[k] = Actividad[k+1];
							}
						}
						while(Actividad[j] == ' ' && (j==0 || j==strlen(Actividad)-1)){
							for(k=j;k<strlen(Actividad);k++){
								Actividad[k] = Actividad[k+1];
							}
						}
					}
				}
				if(strlen(Actividad)!=0){
					correcto = 1;
				} else {
					printf("El nombre del proceso no es valido.\n");
				}
		        correcto = 1;
		    }
		}while(correcto == 0);
		
		
		
		/***Crear ID del proceso***/
		num1=i+1;
		num2=1;
		k=1;
		if(!Empty(Faltantes)){
			for(j=Size(Faltantes);j>0;j--){
				aux = Element(Faltantes, j);
				if(strcmp(Nombre, aux.Nom) == 0){
					ch1 = aux.ID[3];
					ch2 = aux.ID[4];
					num2 = atoi(&aux.ID[5])+1;
					k=0;
					break;
				}
			}
		}
		if(k){
			ch1=Nombre[0];
			ch2='0';
			for(j=0;j<strlen(Nombre);j++){
		    	if(Nombre[j] == ' ' && Nombre[j+1] != '\0'){
		    		ch2=Nombre[j+1];
				}
			}
			if(ch1>='a' && ch1<='z') ch1+='A'-'a';
			if(ch2>='a' && ch2<='z') ch2+='A'-'a';
		}
		snprintf(ID, sizeof(ID), "%03d%c%c%02d", num1, ch1, ch2, num2);
		
		/***Pedir el tiempo de Proceso***/
		printf("¿Cual es el tiempo de proceso para el proceso %d?\n", i+1);
	    do{
			scanf("%d", &segundos);
			while ((c = getchar()) != '\n' && c != EOF);
			if(segundos<1 || segundos>3600){
				printf("Numero de procesos no valido (0<N<3601).\n");
			}
		}while(segundos<1 || segundos>3600);
		
		/***Agregar Proceso a la Cola***/
		strncpy(e.Nom, Nombre, 31);
		strncpy(e.Act, Actividad, 51);
		strncpy(e.ID, ID, 8);
		e.seg = segundos;
		e.temF = segundos;
		e.temT = 0;
		
		Queue(Faltantes, e);
		printf("\n");
	}
	
	*procesos = Aprocesos;
}

/*
	void Simulacion(cola *Faltantes, int procesos)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Faltantes, int Número de procesos
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que simulara el comportamiento de ejecución de un sistema operativo, con base en un
					número determinado de procesos, ejecutando cada proceso poco a poco por segundo, para ejecutar
					de manera equitativa todos los procesos.
*/
void Simulacion(cola *Faltantes, int procesos){
	int tiempo = 0, clienteN = 0, atendidos = 0;
	int i, filaC, aux;
	elemento e;
	
	//Inicializar la función Rand
	srand(time(NULL));
	
	//Crear las colas
	cola Ejecucion, Finalizados;
	
	//Inicializar colas
	Initialize(&Ejecucion);
	Initialize(&Finalizados);
	
	//Iniciar Tienda
	AbrirSistemaOperativo(Faltantes);
	
	//Ciclo infinito de la simulación
	while(1){
		tiempo++;		//Incrementar el contador de tiempo
		
		//Checar estado del proceso
		if(!Empty(&Ejecucion)){
			e = Element(&Ejecucion, 1);
			if(e.temF == 0){
				//Si el proceso ya acabo
				Queue(&Finalizados, Dequeue(&Ejecucion));
				TerminarProceso(&Finalizados, Faltantes);
			} else {
				//Si el proceso no a acabado
				Queue(Faltantes, Dequeue(&Ejecucion));
				QuitarProceso(Faltantes);
			}
		}
		
		//Si hay Procesos por hacer
		if (!Empty(Faltantes)){
			e = Dequeue(Faltantes);
			e.temT=tiempo;
			e.temF--;
			//Agregar Ejecución
			Queue(&Ejecucion, e);
			AgregarProceso(&Ejecucion, Faltantes);
		}
		
		EsperarMiliSeg(TIEMPO_BASE); 	//Esperar el tiempo base (1 segundo)
		
		if(Empty(Faltantes) && Empty(&Ejecucion)){
			//Terminar sistema operativo
			CerrarSistemaOperativo(&Finalizados);
			break;
		}
	}
	return;
}

/*
	void AgregarProceso(cola *Ejecucion, cola *Faltantes)
	Recibe:	cola * Referencia/Dirección a la cola de procesos en Ejecución, cola * Referencia/Dirección a la cola de procesos Faltantes
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que genera de manera animada el cambio de un proceso Faltante, para el apartado de Ejecución.
*/
void AgregarProceso(cola *Ejecucion, cola *Faltantes){
	int columna, fila, i;
	int Esperar = 5;
	elemento e;
	
	
	DibujaElementoEliminar(2);
	if(!Empty(Faltantes)){
		DibujaElementoAgregar(Element(Faltantes, 1), 2);
	} else {
		e = Element(Ejecucion, 1);
		if(e.temF != 0){
			DibujaElementoAgregar(Element(Ejecucion, 1), 2);
			EsperarMiliSeg(100);
			DibujaElementoEliminar(2);
		}
	}
	EsperarMiliSeg(Esperar);
	
	
	//Eliminar Flecha
	for(columna=18,fila=10,i=0;i<9;fila++,i++){
		MoverCursor(columna,fila);
		printf(" ");
	}
	for(columna++,fila--,i=0;i<12;columna++,i++){
		MoverCursor(columna,fila);
		printf(" ");
	}
	
	//Animación de Agregar Flecha
	for(columna=30,fila=18,i=0;i<12;columna--,i++){
		MoverCursor(columna,fila);
		printf("-");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna,fila);
	printf("\\");
	EsperarMiliSeg(Esperar);
	for(fila--,i=0;i<7;fila--,i++){
		MoverCursor(columna,fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna,fila);
	printf("^");
	EsperarMiliSeg(Esperar);
	
	DibujaElementoAgregar(Element(Ejecucion, 1), 0);
	EsperarMiliSeg(Esperar);
}

/*
	void QuitarProceso(cola *Faltantes)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Faltantes. 
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que genera de manera animada el cambio de un proceso en Ejecución, para el apartado de procesos Faltantes
*/
void QuitarProceso(cola *Faltantes){
	int columna, fila, i;
	int Esperar = 5;
	
	
	DibujaElementoEliminar(0);
	EsperarMiliSeg(Esperar);
	
	//Eliminar Flecha
	for(columna=25,fila=10,i=0;i<5;fila++,i++){
		MoverCursor(columna,fila);
		printf(" ");
	}
	for(columna++,fila--,i=0;i<5;columna++,i++){
		MoverCursor(columna,fila);
		printf(" ");
	}
	
	//Animación de Agregar Flecha
	for(columna=25,fila=10,i=0;i<4;fila++,i++){
		MoverCursor(columna,fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna,fila);
	printf("\\");
	EsperarMiliSeg(Esperar);
	for(columna++,i=0;i<4;columna++,i++){
		MoverCursor(columna,fila);
		printf("-");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna,fila);
	printf(">");
	EsperarMiliSeg(Esperar);
	
	DibujaElementoEliminar(1);
	DibujaElementoAgregar(Element(Faltantes, Size(Faltantes)), 1);
	EsperarMiliSeg(Esperar);
}

/*
	void TerminarProceso(cola *Finalizados, cola *Faltantes)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Finalizados, cola * Referencia/Dirección a la cola de procesos Faltantes
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que genera de manera animada el cambio de un proceso en Ejecución, para el apartado de procesos Finalizados.
*/
void TerminarProceso(cola *Finalizados, cola *Faltantes){
	int columna, fila, i;
	int Esperar = 5;
	
	
	DibujaElementoEliminar(0);
	if(Empty(Faltantes)){
		DibujaElementoEliminar(1);
	}
	EsperarMiliSeg(Esperar);
	
	//Eliminar Flecha
	for(columna=11,fila=10,i=0;i<13;fila++,i++){
		MoverCursor(columna,fila);
		printf(" ");
	}
	for(columna++,fila--,i=0;i<19;columna++,i++){
		MoverCursor(columna,fila);
		printf(" ");
	}
	
	//Animación de Agregar Flecha
	for(columna=11,fila=10,i=0;i<12;fila++,i++){
		MoverCursor(columna,fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna,fila);
	printf("\\");
	EsperarMiliSeg(Esperar);
	for(columna++,i=0;i<18;columna++,i++){
		MoverCursor(columna,fila);
		printf("-");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna,fila);
	printf(">");
	EsperarMiliSeg(Esperar);
	
	DibujaElementoEliminar(3);
	DibujaElementoAgregar(Element(Finalizados, Size(Finalizados)), 3);
	EsperarMiliSeg(Esperar);
}

/*
	void AbrirSistemaOperativo(cola *Faltantes)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Faltantes.
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que abre el Sistema Operativo de manera animada en pantalla.
*/
void AbrirSistemaOperativo(cola *Faltantes){
	DibujaPresentacion();
	DibujaMarco();
	DibujaCuadros();
	DibujaAnuncioAbrir();
	DibujaElementoAgregar(Element(Faltantes, 1), 2);
}

/*
	void CerrarSistemaOperativo(cola *Finalizados)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Finalizados
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que cierra el Sistema Operativo de manera animada en pantalla.
*/
void CerrarSistemaOperativo(cola *Finalizados){
	DibujaAnuncioCerrar();
	EsperarMiliSeg(600);
	DibujaCortina();
	EsperarMiliSeg(1200);
	DibujaCartel();
	DibujaCartelDatos(Finalizados);
}

/*
	void DibujaPresentacion()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da una presentación a la simulación 02.
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
	printf("SIMULACION 02");
	
	EsperarMiliSeg(5000);
	BorrarPantalla();
}

/*
	void DibujaMarco()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea el todo el marco del Sistema Operativo así como un Titulo.
*/
void DibujaMarco(){
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
		MoverCursor(columna,ALTO-1);
		printf("_");
		EsperarMiliSeg(Esperar);
	}
	
	//Crear titulo
	MoverCursor(24,3);
	printf("PROCESOS EN EL SISTEMA OPERATIVO");
	EsperarMiliSeg(200);
}

/*
	void DibujaCuadros()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que crea los cuadros que usara el Sistema Operativo para mostrar el estado
					de ciertos procesos.
*/
void DibujaCuadros(){
	int columna=8,fila=4,i,j;
	int Esperar = 5;
	
	/***Crear cuadro Principal***/
	//Crear lineas
	for(i=1;i<=62;i++){
		MoverCursor(columna+i,fila);
		printf("_");
		MoverCursor(columna+i,fila+5);
		printf("_");
		EsperarMiliSeg(Esperar);
	}
	
	//Crear columnas
	for(j=1;j<=5;j++){
		MoverCursor(columna,fila+j);
		printf("|");
		MoverCursor(columna+63,fila+j);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	
	/***Crear cuadros Secundarios***/
	//Crear cuadros
	for(columna=31,fila=12,i=1;i<=3;fila+=4,i++){
		//Crear lineas
		for(j=0;j<2;j++){
			MoverCursor(columna+1+(j*41),fila);
			printf("_");
			MoverCursor(columna+1+(j*41),fila+2);
			printf("_");
			EsperarMiliSeg(Esperar);
		}
		
		//Crear lineas
		for(j=1;j<=2;j++){
			MoverCursor(columna,fila+j);
			printf("|");
			MoverCursor(columna+43,fila+j);
			printf("|");
			EsperarMiliSeg(Esperar);
		}
	}
	
	/***Agregar Titulos***/
	MoverCursor(11,5);
	printf("EJECUCIÓN");
	EsperarMiliSeg(200);
	MoverCursor(35,12);
	printf("ANTERIOR");
	EsperarMiliSeg(200);
	MoverCursor(35,16);
	printf("SIGUIENTE");
	EsperarMiliSeg(200);
	MoverCursor(35,20);
	printf("FINALIZADO");
	EsperarMiliSeg(200);
}

/*
	void DibujaElementoAgregar(elemento e, int tipo)
	Recibe:	elemento Elemento, int tipo de dibujo
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que con base a la información de un elemento, y el tipo de dibujo, este 
					agrega la informacíon del elemento en el cuadro deseado.
*/
void DibujaElementoAgregar(elemento e, int tipo){
	int columna, fila;
	if(tipo==0){
		columna=14;fila=6;
		MoverCursor(columna,fila);
		printf("%s", e.Nom);
		MoverCursor(columna,++fila);
		printf("* %s", e.Act);
		MoverCursor(columna,++fila);
		printf("ID-%s", e.ID);
		MoverCursor(columna+21,fila);
		printf("TIEMPO TOTAL DE EJECUCION:%d", e.temT);
	} else {
		tipo--;
		columna=37;fila=13+(4*tipo);
		MoverCursor(columna,fila);
		printf("-%s-", e.Nom);
		MoverCursor(columna,++fila);
		printf("ID-%s", e.ID);
		MoverCursor(columna+12,fila);
		if(tipo!=2){
			printf("TIEMPO FALTANTE:%ds", e.temF);
		} else {
			printf("TIEMPO TARDADO:%ds", e.temT);
		}
	}
}

/*
	void DibujaElementoEliminar(int tipo)
	Recibe:	int tipo de dibujo
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que con base al tipo de dibujo, este elimina la informacíon contenida en
					el cuadro deseado.
*/
void DibujaElementoEliminar(int tipo){
	int columna, fila, i;
	if(tipo==0){
		columna=14;fila=6;
		for(i=0;i<3;i++){
			MoverCursor(columna,fila+i);
			printf("                                                    ");
		}
	} else {
		tipo--;
		columna=37;fila=13+(4*tipo);
		for(i=0;i<2;i++){
			MoverCursor(columna,fila+i);
			printf("                                 ");
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
	
	for(fila=1,columna=4;fila<ALTO-3;fila++){
		MoverCursor(columna,fila);
		printf("|");
		for(i=0;i<69;i++)
			printf("_");
		printf("|");
		
		if(fila-1>0){
			MoverCursor(columna,fila-1);
			printf("|");
			for(i=0;i<69;i++)
				printf(" ");
			printf("|");
		}
		
		if(fila-16>0){
			MoverCursor(columna,fila-16);
			printf(" ");
			for(i=0;i<69;i++)
				printf("_");
			printf(" ");
		}
		
		if(fila-17>0){
			MoverCursor(columna,fila-17);
			for(i=0;i<14;i++){
				if(i==3 || i==11)
					printf("||  |");
				else
					printf(" |  |");
			}
		}
		
		EsperarMiliSeg(200);
	}
}

/*
	void DibujaCartelDatos(cola *Finalizados)
	Recibe:	cola * Referencia/Dirección a la cola de procesos Finalizados
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que agrega los datos al cartel del finalizamiento de la simulación.
*/
void DibujaCartelDatos(cola *Finalizados){
	int columna,fila,i,j,k;
	elemento e;
	
	//Mostrar datos finales
	for(columna=8,fila=6,i=1;i<=3;i++){
		EsperarMiliSeg(600);
		
		if(i==1){
			MoverCursor(columna+16,fila);
			printf("PROCESOS EN EL SISTEMA OPERATIVO");
			fila+=2;
		}
		
		if(i==2){
			MoverCursor(columna,fila);
			printf("Procesos mas rapidos:");
			MoverCursor(columna+48,fila);
			printf("Tiempo:");
			
			for(j=1;j<=Size(Finalizados)&&j<=3;j++){
				EsperarMiliSeg(200);
				MoverCursor(columna+3,fila+j);
				e = Element(Finalizados,j);
				printf("* ID-%s - %s ", e.ID, e.Nom);
				for(k=16+strlen(e.Nom);k<=49;k++){
					printf("-");
				}
				printf(" %ds", e.temT);
			}
			
			fila+=5;
		}
		
		if(i==3){
			if(Size(Finalizados)>3){
				MoverCursor(columna,fila);
				printf("Demas procesos:");
				for(columna+=3,fila++,j=4;j<=Size(Finalizados);j++){
					EsperarMiliSeg(200);
					MoverCursor(columna,fila);
					e = Element(Finalizados,j);
					
					if(j==21){
						if(Size(Finalizados) == 21){
							printf("* %s - %ds", e.ID, e.temT);
						} else {
							printf("* +%d Procesos", Size(Finalizados) - 20);
						}
						break;
					}
					
					printf("* %s - %ds", e.ID, e.temT);
					if(j%3 != 0){
						columna+=21;
					} else {
						fila++;
						columna=11;
					}
				}
			}
		}
	}
}

/*
	void DibujaAnuncioAbrir()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da un anunció en pantalla de que el Sistema Operativo comenzará
					con su funciónamiento.
*/
void DibujaAnuncioAbrir(){
	int columna=34, fila=7, i;
	int Esperar = 800;
	
	//Escribir Conteo
	for(i=3;i>0;i--){
		MoverCursor(columna,fila);
		EsperarMiliSeg(Esperar);
		printf("[ %d ]", i);
	}
	EsperarMiliSeg(Esperar);
	
	//Poner GO
	MoverCursor(columna,fila);
	printf("[ GO ]");
	EsperarMiliSeg(Esperar);
	
	//Quitar GO
	MoverCursor(columna,fila);
	printf("      ");
	EsperarMiliSeg(Esperar);
}

/*
	void DibujaAnuncioCerrar()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Función que da un anunció en pantalla de que el Sistema Operativo termino
					con su funciónamiento.
*/
void DibujaAnuncioCerrar(){
	int columna=29, fila=7, i;
	char anuncio[] = "PROCESOS TERMINADOS";
	int Esperar = 500;
	
	//Escribir Anuncio Lentamente
	MoverCursor(columna,fila);
	for(i=0;i<19;i++){
		EsperarMiliSeg(Esperar-200);
		printf("%c", anuncio[i]);
	}
	
	//Parpadeo de Aununcio
	for(i=0;i<6;i++){
		MoverCursor(columna,fila);
		EsperarMiliSeg(Esperar+100);
		if(i%2 == 0){
			printf("                   ");
		} else {
			printf("%s", anuncio);
		}
	}
}
