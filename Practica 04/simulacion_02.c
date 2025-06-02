/*
Autores:	Garcia Ambrosio Aldo (C) Junio 2025
			Ponce Espino Miguel Angel (C) Junio 2025
			Salinas Aquino Omar Ivan (C) Junio 2025
Grupo: 2CM6
Uso de IA: Claude 3.7 Sonnet
Version: 2.2
Materia:	Algoritmos y Estructuras de Datos

Programa que con el uso de colas simula el comportamiento de las Ejecuciones de Procesos en un Sistema Operativo
de manera animada dentro de la consola de la computadora.

Observaciones: El programa requerira de la libreria "presentacion.h", la cual tiene las implementaciones para mover
el cursor de la pantalla, esperar un tiempo y borrar pantalla, además por utilizar la estructura de datos cola se
necesitarán de las siguientes librerías: "TADColaEst.h" y "TADCola/TADColaDin.h", la compilacion debera incluir las
definiciones de las funciones segun la plataforma que se este utilizando (Windows o Linux).

Compilacion: 	gcc -o simulacion_02 simulacion_02.c presentacion/presentacion(Win|Lin).o TADCola/TADCola(Din|Est|EstCirc).o (Win si se correra en Windows | Lin si se ejecutara en Linux)  "Si se tiene el codigo objeto"
				gcc -o simulacion_02 simulacion_02.c presentacion/presentacion(Win|Lin).c TADCola/TADCola(Din|Est|EstCirc).c (Win si se correra en Windows | Lin si se ejecutara en Linux)  "Si se tiene el codigo fuente"

Ejecucion: Windows simulacion_02.exe  &  Linux ./simulacion_02
*/

// LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include "Recursos/presentacion/presentacion.h"
// #include "TADCola/TADColaEst.h" 	//Si se usa la implemtentacion estatica (TADColaEst.c|TADColaEstCirc.c)
#include "Recursos/TADCola/TADColaDin.h" //Si se usa la implemtentacion dinamica (TADColaDin.c)

// CONSTANTES
#define ALTO 27 // Se piensa en un pantalla de 24 filas x 79 columnas
#define ANCHO 84
#define TIEMPO_BASE 1000 // Tiempo base en milisegundos

// FUNCIONES
void PedirEntrada(cola *Faltantes, int *procesos);

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
void DibujaApagadoTV();
void DibujaCartelDatos(cola *Finalizados);
void DibujaAnuncioAbrir();
void DibujaAnuncioCerrar();

// PROGRAMA PRINCIPAL
int main(void)
{
	cola Faltantes;
	int procesos;

	setlocale(LC_ALL, "");

	// Inicializar colas de los faltantes
	Initialize(&Faltantes);

	// Pedir entrada
	PedirEntrada(&Faltantes, &procesos);

	// Inicializar simulacion
	Simulacion(&Faltantes, procesos);

	MoverCursor(ANCHO, ALTO);
}

/*
void PedirEntrada(cola *Faltantes, int *procesos)
Recibe:	cola * Referencia a la cola de procesos Faltantes, int * Referencia al numero de procesos
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que pide los datos como le numero de procesos a ejecutar, con su respectivo, Nombre, Nombre de
						Actividad, Segundos para completar la Ejecucion y un ID Generado automaticamente.
*/
void PedirEntrada(cola *Faltantes, int *procesos)
{
	char Nombre[45], Actividad[200], ID[45], correcto, ch1, ch2, c;
	int Nprocesos, segundos, num1, num2, i, j, k;
	elemento e, aux;

	BorrarPantalla();

	// Pedir el numero de procesos
	printf("¿Cuantos procesos quiere que se ejecuten en el sistema operativo?\n");
	do
	{
		scanf("%d", &Nprocesos);
		while ((c = getchar()) != '\n' && c != EOF)
			;
		if (Nprocesos < 1 || Nprocesos > 100)
		{
			printf("Numero de procesos no valido (0<N<101).\n");
		}
	} while (Nprocesos < 1 || Nprocesos > 100);

	for (i = 0; i < Nprocesos; i++)
	{
		// Pedir el nombre del proceso
		correcto = 0;
		printf("¿Como se llamara el proceso %d?\n", i + 1);
		do
		{
			fgets(Nombre, sizeof(Nombre), stdin);
			// Verificar si la longitud de la cadena ingresada excede 30 caracteres (sin contar el '\n' de fgets)
			if (strlen(Nombre) > 45 && Nombre[45] != '\n')
			{
				printf("El nombre del proceso supera los 45 caracteres.\n");
				while (getchar() != '\n')
					;
			}
			else
			{
				// En caso de un salto de linea
				if (Nombre[strlen(Nombre) - 1] == '\n')
				{
					Nombre[strlen(Nombre) - 1] = '\0';
				}
				// Quitar espacios innecesarios en el nombre
				for (j = 0; j < strlen(Nombre); j++)
				{
					if (Nombre[j] == ' ')
					{
						while (Nombre[j + 1] == ' ')
						{
							for (k = j + 1; k < strlen(Nombre); k++)
							{
								Nombre[k] = Nombre[k + 1];
							}
						}
						while (Nombre[j] == ' ' && (j == 0 || j == strlen(Nombre) - 1))
						{
							for (k = j; k < strlen(Nombre); k++)
							{
								Nombre[k] = Nombre[k + 1];
							}
						}
					}
				}
				if (strlen(Nombre) != 0)
				{
					correcto = 1;
				}
				else
				{
					printf("El nombre del proceso no es valido.\n");
				}
			}
		} while (correcto == 0);

		// Pedir la actividad del proceso
		correcto = 0;
		printf("¿Cual es la actividad el proceso %d?\n", i + 1);
		do
		{
			fgets(Actividad, sizeof(Actividad), stdin);
			// Verificar si la longitud de la cadena ingresada excede 30 caracteres (sin contar el '\n' de fgets)
			if (strlen(Actividad) == 51 && Actividad[50] != '\n')
			{
				printf("El nombre de la actividad del proceso supera los 50 caracteres.\n");
				while (getchar() != '\n')
					;
			}
			else
			{
				// En caso de un salto de linea
				if (Actividad[strlen(Actividad) - 1] == '\n')
				{
					Actividad[strlen(Actividad) - 1] = '\0';
				}
				// Quitar espacios innecesarios en la Actividad
				for (j = 0; j < strlen(Actividad); j++)
				{
					if (Actividad[j] == ' ')
					{
						while (Actividad[j + 1] == ' ')
						{
							for (k = j + 1; k < strlen(Actividad); k++)
							{
								Actividad[k] = Actividad[k + 1];
							}
						}
						while (Actividad[j] == ' ' && (j == 0 || j == strlen(Actividad) - 1))
						{
							for (k = j; k < strlen(Actividad); k++)
							{
								Actividad[k] = Actividad[k + 1];
							}
						}
					}
				}
				if (strlen(Actividad) != 0)
				{
					correcto = 1;
				}
				else
				{
					printf("El nombre del proceso no es valido.\n");
				}
				correcto = 1;
			}
		} while (correcto == 0);

		// Crear ID del proceso
		num1 = i + 1;
		num2 = 1;
		k = 1;
		if (!Empty(Faltantes))
		{
			for (j = Size(Faltantes); j > 0; j--)
			{
				aux = Element(Faltantes, j);
				if (strcmp(Nombre, aux.Nom) == 0)
				{
					ch1 = aux.ID[3];
					ch2 = aux.ID[4];
					num2 = atoi(&aux.ID[5]) + 1;
					k = 0;
					break;
				}
			}
		}
		if (k)
		{
			ch1 = Nombre[0];
			ch2 = '0';
			for (j = 0; j < strlen(Nombre); j++)
			{
				if (Nombre[j] == ' ' && Nombre[j + 1] != '\0')
				{
					ch2 = Nombre[j + 1];
				}
			}
			if (ch1 >= 'a' && ch1 <= 'z')
				ch1 += 'A' - 'a';
			if (ch2 >= 'a' && ch2 <= 'z')
				ch2 += 'A' - 'a';
		}
		snprintf(ID, sizeof(ID), "%03d%c%c%02d", num1, ch1, ch2, num2);

		// Pedir el tiempo del proceso
		printf("¿Cual es el tiempo de proceso para el proceso %d?\n", i + 1);
		do
		{
			scanf("%d", &segundos);
			while ((c = getchar()) != '\n' && c != EOF)
				;
			if (segundos < 1 || segundos > 3600)
			{
				printf("Numero de procesos no valido (0<N<3601).\n");
			}
		} while (segundos < 1 || segundos > 3600);

		// Agregar proceso a la cola
		strncpy(e.Nom, Nombre, 31);
		strncpy(e.Act, Actividad, 51);
		strncpy(e.ID, ID, 8);
		e.seg = segundos;
		e.temF = segundos;
		e.temT = 0;

		Queue(Faltantes, e);
		printf("\n");
	}

	*procesos = Nprocesos;
}

/*
void Simulacion(cola *Faltantes, int procesos)
Recibe:	cola * Referencia a la cola de procesos Faltantes, int Numero de procesos
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que simulara el comportamiento de ejecucion de un sistema operativo, con base en un
					numero determinado de procesos, ejecutando cada proceso poco a poco por segundo, para ejecutar
					de manera equitativa todos los procesos.
*/
void Simulacion(cola *Faltantes, int procesos)
{
	int tiempo = 0, clienteN = 0, atendidos = 0;
	int i, filaC, aux;
	elemento e;

	// Inicializar la funcion Rand
	srand(time(NULL));

	// Crear las colas
	cola Ejecucion, Finalizados;

	// Inicializar colas
	Initialize(&Ejecucion);
	Initialize(&Finalizados);

	// Iniciar Tienda
	AbrirSistemaOperativo(Faltantes);

	// Ciclo infinito de la simulacion
	while (1)
	{
		tiempo++; // Incrementar el contador de tiempo

		// Checar estado del proceso
		if (!Empty(&Ejecucion))
		{
			e = Element(&Ejecucion, 1);
			if (e.temF == 0)
			{
				// Si el proceso ya acabo
				Queue(&Finalizados, Dequeue(&Ejecucion));
				TerminarProceso(&Finalizados, Faltantes);
			}
			else
			{
				// Si el proceso no a acabado
				Queue(Faltantes, Dequeue(&Ejecucion));
				QuitarProceso(Faltantes);
			}
		}

		// Si hay Procesos por hacer
		if (!Empty(Faltantes))
		{
			e = Dequeue(Faltantes);
			e.temT = tiempo;
			e.temF--;
			// Agregar Ejecucion
			Queue(&Ejecucion, e);
			AgregarProceso(&Ejecucion, Faltantes);
		}

		EsperarMiliSeg(TIEMPO_BASE); // Esperar el tiempo base (1 segundo)

		if (Empty(Faltantes) && Empty(&Ejecucion))
		{
			// Terminar sistema operativo
			CerrarSistemaOperativo(&Finalizados);
			break;
		}
	}
	return;
}

/*
void AgregarProceso(cola *Ejecucion, cola *Faltantes)
Recibe:	cola * Referencia a la cola de procesos en Ejecucion, cola * Referencia a la cola de procesos Faltantes
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que genera de manera animada el cambio de un proceso Faltante, para el apartado de Ejecucion.
*/
void AgregarProceso(cola *Ejecucion, cola *Faltantes)
{
	int columna, fila, i;
	int Esperar = 5;
	elemento e;

	DibujaElementoEliminar(2);
	if (!Empty(Faltantes))
	{
		DibujaElementoAgregar(Element(Faltantes, 1), 2);
	}
	else
	{
		e = Element(Ejecucion, 1);
		if (e.temF != 0)
		{
			DibujaElementoAgregar(Element(Ejecucion, 1), 2);
			EsperarMiliSeg(100);
			DibujaElementoEliminar(2);
		}
	}
	EsperarMiliSeg(Esperar);

	// Eliminar Flecha
	for (columna = 18, fila = 10, i = 0; i < 9; fila++, i++)
	{
		MoverCursor(columna, fila);
		printf(" ");
	}
	for (columna++, fila--, i = 0; i < 12; columna++, i++)
	{
		MoverCursor(columna, fila);
		printf(" ");
	}

	// Animacion de Agregar Flecha
	for (columna = 30, fila = 18, i = 0; i < 12; columna--, i++)
	{
		MoverCursor(columna, fila);
		printf("-");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna, fila);
	printf("\\");
	EsperarMiliSeg(Esperar);
	for (fila--, i = 0; i < 7; fila--, i++)
	{
		MoverCursor(columna, fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna, fila);
	printf("^");
	EsperarMiliSeg(Esperar);

	DibujaElementoAgregar(Element(Ejecucion, 1), 0);
	EsperarMiliSeg(Esperar);
}

/*
void QuitarProceso(cola *Faltantes)
Recibe:	cola * Referencia a la cola de procesos Faltantes.
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que genera de manera animada el cambio de un proceso en Ejecucion, para el apartado de procesos Faltantes
*/
void QuitarProceso(cola *Faltantes)
{
	int columna, fila, i;
	int Esperar = 5;

	DibujaElementoEliminar(0);
	EsperarMiliSeg(Esperar);

	// Eliminar Flecha
	for (columna = 25, fila = 10, i = 0; i < 5; fila++, i++)
	{
		MoverCursor(columna, fila);
		printf(" ");
	}
	for (columna++, fila--, i = 0; i < 5; columna++, i++)
	{
		MoverCursor(columna, fila);
		printf(" ");
	}

	// Animacion de Agregar Flecha
	for (columna = 25, fila = 10, i = 0; i < 4; fila++, i++)
	{
		MoverCursor(columna, fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna, fila);
	printf("\\");
	EsperarMiliSeg(Esperar);
	for (columna++, i = 0; i < 4; columna++, i++)
	{
		MoverCursor(columna, fila);
		printf("-");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna, fila);
	printf(">");
	EsperarMiliSeg(Esperar);

	DibujaElementoEliminar(1);
	DibujaElementoAgregar(Element(Faltantes, Size(Faltantes)), 1);
	EsperarMiliSeg(Esperar);
}

/*
void TerminarProceso(cola *Finalizados, cola *Faltantes)
Recibe:	cola * Referencia a la cola de procesos Finalizados, cola * Referencia a la cola de procesos Faltantes
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que genera de manera animada el cambio de un proceso en Ejecucion, para el apartado de procesos Finalizados.
*/
void TerminarProceso(cola *Finalizados, cola *Faltantes)
{
	int columna, fila, i;
	int Esperar = 5;

	DibujaElementoEliminar(0);
	if (Empty(Faltantes))
	{
		DibujaElementoEliminar(1);
	}
	EsperarMiliSeg(Esperar);

	// Eliminar Flecha
	for (columna = 11, fila = 10, i = 0; i < 13; fila++, i++)
	{
		MoverCursor(columna, fila);
		printf(" ");
	}
	for (columna++, fila--, i = 0; i < 19; columna++, i++)
	{
		MoverCursor(columna, fila);
		printf(" ");
	}

	// Animacion de Agregar Flecha
	for (columna = 11, fila = 10, i = 0; i < 12; fila++, i++)
	{
		MoverCursor(columna, fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna, fila);
	printf("\\");
	EsperarMiliSeg(Esperar);
	for (columna++, i = 0; i < 18; columna++, i++)
	{
		MoverCursor(columna, fila);
		printf("-");
		EsperarMiliSeg(Esperar);
	}
	MoverCursor(columna, fila);
	printf(">");
	EsperarMiliSeg(Esperar);

	DibujaElementoEliminar(3);
	DibujaElementoAgregar(Element(Finalizados, Size(Finalizados)), 3);
	EsperarMiliSeg(Esperar);
}

/*
void AbrirSistemaOperativo(cola *Faltantes)
Recibe:	cola * Referencia a la cola de procesos Faltantes.
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que abre el Sistema Operativo de manera animada en pantalla.
*/
void AbrirSistemaOperativo(cola *Faltantes)
{
	DibujaPresentacion();
	DibujaMarco();
	DibujaCuadros();
	DibujaAnuncioAbrir();
	DibujaElementoAgregar(Element(Faltantes, 1), 2);
}

/*
void CerrarSistemaOperativo(cola *Finalizados)
Recibe:	cola * Referencia/Direccion a la cola de procesos Finalizados
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que cierra el Sistema Operativo de manera animada en pantalla.
*/
void CerrarSistemaOperativo(cola *Finalizados)
{
	DibujaAnuncioCerrar();
	EsperarMiliSeg(600);
	DibujaApagadoTV();
	EsperarMiliSeg(1200);
	DibujaCartelDatos(Finalizados);
}

/*
void DibujaPresentacion()
Recibe:	void (No recibe valor explicito)
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que da una presentacion a la simulacion 02.
*/
void DibujaPresentacion()
{
	int columna, fila, i;

	BorrarPantalla();

	// Crear Particulas Aleatorias
	srand(time(NULL));
	EsperarMiliSeg(2200);
	for (i = 1; i <= 500; i++)
	{
		columna = (rand() % (ANCHO - 1)) + 1;
		fila = (rand() % (ALTO - 1)) + 1;
		MoverCursor(columna, fila);
		printf("G");
		if (i % 100 == 0)
		{
			EsperarMiliSeg(800);
		}
	}

	// Crear Primer Cuadro
	EsperarMiliSeg(2200);
	for (columna = 22; columna <= 57; columna++)
	{
		for (fila = 4; fila <= 13; fila++)
		{
			MoverCursor(columna, fila);
			if ((columna > 22 && columna < 57) && (fila == 4 || fila == 13))
			{
				printf("_");
			}
			else if ((fila > 4 && fila <= 13) && (columna == 22 || columna == 57))
			{
				printf("|");
			}
			else
			{
				printf(" ");
			}
		}
	}

	// Poner datos
	EsperarMiliSeg(1800);
	MoverCursor(33, 7);
	printf("EQUIPO GUIMO");
	MoverCursor(35, 11);
	printf("PRESENTA");

	// Poner datos
	EsperarMiliSeg(1800);
	MoverCursor(33, 7);
	printf("              ");
	MoverCursor(35, 11);
	printf("         ");
	MoverCursor(33, 9);
	printf("SIMULACION 02");

	EsperarMiliSeg(5000);
	BorrarPantalla();
}

/*
void DibujaMarco()
Recibe:	void (No recibe valor explicito)
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que crea el todo el marco del Sistema Operativo asi como un Titulo.
*/
void DibujaMarco()
{
	int columna, fila, i;
	int Esperar = 5;

	// Crear columnas
	for (columna = 1, fila = 2; fila < ALTO; fila++)
	{
		// Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
		MoverCursor(columna, fila);
		printf("|");
		MoverCursor(ANCHO - 1, fila);
		printf("|");
		EsperarMiliSeg(Esperar);
	}

	// Crear filas
	for (columna = 2, fila = 1; columna < ANCHO - 1; columna++)
	{
		// Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
		MoverCursor(columna, fila);
		printf("_");
		MoverCursor(columna, ALTO - 1);
		printf("_");
		EsperarMiliSeg(Esperar);
	}

	// Crear titulo
	MoverCursor(24, 3);
	printf("PROCESOS EN EL SISTEMA OPERATIVO");
	EsperarMiliSeg(200);
}

/*
void DibujaCuadros()
Recibe:	void (No recibe valor explicito)
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que crea los cuadros que usara el Sistema Operativo para mostrar el estado
					de ciertos procesos.
*/
void DibujaCuadros()
{
	int columna = 8, fila = 4, i, j;
	int Esperar = 5;

	// Crea el cuadro principal
	//  Crear lineas
	for (i = 1; i <= 62; i++)
	{
		MoverCursor(columna + i, fila);
		printf("_");
		MoverCursor(columna + i, fila + 5);
		printf("_");
		EsperarMiliSeg(Esperar);
	}

	// Crear columnas
	for (j = 1; j <= 5; j++)
	{
		MoverCursor(columna, fila + j);
		printf("|");
		MoverCursor(columna + 63, fila + j);
		printf("|");
		EsperarMiliSeg(Esperar);
	}

	// Crea el cuadro secundario
	//  Crear cuadros
	for (columna = 31, fila = 12, i = 1; i <= 3; fila += 4, i++)
	{
		// Crear lineas
		for (j = 0; j < 2; j++)
		{
			MoverCursor(columna + 1 + (j * 41), fila); // cambiar
			printf("_");
			MoverCursor(columna + 1 + (j * 41), fila + 2);
			printf("_");
			EsperarMiliSeg(Esperar);
		}

		// Crear lineas
		for (j = 1; j <= 2; j++)
		{
			MoverCursor(columna, fila + j);
			printf("|");
			MoverCursor(columna + 43, fila + j);
			printf("|");
			EsperarMiliSeg(Esperar);
		}
	}

	// Agrega titulos
	MoverCursor(11, 5);
	printf("EJECUCION");
	EsperarMiliSeg(200);
	MoverCursor(30, 12);
	printf("ANTERIOR");
	EsperarMiliSeg(200);
	MoverCursor(30, 16);
	printf("SIGUIENTE");
	EsperarMiliSeg(200);
	MoverCursor(30, 20);
	printf("FINALIZADO");
	EsperarMiliSeg(200);
}

/*
void DibujaElementoAgregar(elemento e, int tipo)
Recibe:	elemento Elemento, int tipo de dibujo
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que con base a la informacion de un elemento, y el tipo de dibujo, este
					agrega la informacion del elemento en el cuadro deseado.
*/
void DibujaElementoAgregar(elemento e, int tipo)
{
	int columna, fila;
	if (tipo == 0)
	{
		columna = 14;
		fila = 6;
		MoverCursor(columna, fila);
		printf("%s", e.Nom);
		MoverCursor(columna, ++fila);
		printf("* %s", e.Act);
		MoverCursor(columna, ++fila);
		printf("ID-%s", e.ID);
		MoverCursor(columna + 21, fila);
		printf("TIEMPO TOTAL DE EJECUCION:%d", e.temT);
	}
	else
	{
		tipo--;
		columna = 37;
		fila = 13 + (4 * tipo);
		MoverCursor(columna, fila);
		printf("-%s-", e.Nom);
		MoverCursor(columna, ++fila);
		printf("ID-%s", e.ID);
		MoverCursor(columna + 12, fila);
		if (tipo != 2)
		{
			printf("TIEMPO FALTANTE:%ds", e.temF);
		}
		else
		{
			printf("TIEMPO TARDADO:%ds", e.temT);
		}
	}
}

/*
void DibujaElementoEliminar(int tipo)
Recibe:	int tipo de dibujo
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que con base al tipo de dibujo, este elimina la informacion contenida en
					el cuadro deseado.
*/
void DibujaElementoEliminar(int tipo)
{
	int columna, fila, i;
	if (tipo == 0)
	{
		columna = 14;
		fila = 6;
		for (i = 0; i < 3; i++)
		{
			MoverCursor(columna, fila + i);
			printf("                                                    ");
		}
	}
	else
	{
		tipo--;
		columna = 37;
		fila = 13 + (4 * tipo);
		for (i = 0; i < 2; i++)
		{
			MoverCursor(columna, fila + i);
			printf("                                 ");
		}
	}
}

/*
void DibujaApagadoTV()
Recibe:
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que simula el apagado de una TV
*/

void DibujaApagadoTV()
{
	int filaSuperior, filaInferior, columnaIzq, columnaDer;
	int i;

	// Paso 1: Colapso vertical hacia el centro
	for (i = 0; i < ALTO / 2; i++)
	{
		filaSuperior = i;
		filaInferior = ALTO - 1 - i;

		MoverCursor(0, filaSuperior);
		for (int j = 0; j < ANCHO; j++)
			printf(" ");

		MoverCursor(0, filaInferior);
		for (int j = 0; j < ANCHO; j++)
			printf(" ");

		EsperarMiliSeg(50);
	}

	// Paso 2: Colapso horizontal del centro
	columnaIzq = ANCHO / 2;
	columnaDer = ANCHO / 2;

	for (i = 0; i < ANCHO / 2; i++)
	{
		MoverCursor(columnaIzq - i, ALTO / 2);
		printf(" ");
		MoverCursor(columnaDer + i, ALTO / 2);
		printf(" ");
		EsperarMiliSeg(10);
	}

	// Apagado final (pantalla negra)
	EsperarMiliSeg(300);
	system("cls"); // Limpia la pantalla
}
/*
void DibujaCartelDatos(cola *Finalizados)
Recibe:	cola * Referencia a la cola de procesos Finalizados
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que agrega los datos al cartel del finalizamiento de la simulacion.
*/
void DibujaCartelDatos(cola *Finalizados)
{
	int columna, fila, i, j, k;
	elemento e;

	// Mostrar datos finales
	for (columna = 8, fila = 6, i = 1; i <= 3; i++)
	{
		EsperarMiliSeg(600);

		if (i == 1)
		{
			MoverCursor(columna + 16, fila);
			printf("PROCESOS EN EL SISTEMA OPERATIVO");
			fila += 2;
		}

		if (i == 2)
		{
			MoverCursor(columna, fila);
			printf("Procesos mas rapidos:");
			MoverCursor(columna + 48, fila);
			printf("Tiempo:");

			for (j = 1; j <= Size(Finalizados) && j <= 3; j++)
			{
				EsperarMiliSeg(200);
				MoverCursor(columna + 3, fila + j);
				e = Element(Finalizados, j);
				printf("* ID-%s - %s ", e.ID, e.Nom);
				for (k = 16 + strlen(e.Nom); k <= 49; k++)
				{
					printf("-");
				}
				printf(" %ds", e.temT);
			}

			fila += 5;
		}

		if (i == 3)
		{
			if (Size(Finalizados) > 3)
			{
				MoverCursor(columna, fila);
				printf("Demas procesos:");
				for (columna += 3, fila++, j = 4; j <= Size(Finalizados); j++)
				{
					EsperarMiliSeg(200);
					MoverCursor(columna, fila);
					e = Element(Finalizados, j);

					if (j == 21)
					{
						if (Size(Finalizados) == 21)
						{
							printf("* %s - %ds", e.ID, e.temT);
						}
						else
						{
							printf("* +%d Procesos", Size(Finalizados) - 20);
						}
						break;
					}

					printf("* %s - %ds", e.ID, e.temT);
					if (j % 3 != 0)
					{
						columna += 21;
					}
					else
					{
						fila++;
						columna = 11;
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
Observaciones:	Funcion que da un anuncio en pantalla de que el Sistema Operativo comenzar
					con su funcionamiento.
*/
void DibujaAnuncioAbrir()
{
	int columna = 34, fila = 7, i;
	int Esperar = 800;

	// Escribir Conteo
	for (i = 3; i > 0; i--)
	{
		MoverCursor(columna, fila);
		EsperarMiliSeg(Esperar);
		printf("[ %d ]", i);
	}
	EsperarMiliSeg(Esperar);

	// Poner GO
	MoverCursor(columna, fila);
	printf("[ GO ]");
	EsperarMiliSeg(Esperar);

	// Quitar GO
	MoverCursor(columna, fila);
	printf("      ");
	EsperarMiliSeg(Esperar);
}

/*
void DibujaAnuncioCerrar()
Recibe:	void (No recibe valor explicito)
Devuelve:	void (No retorna valor explicito)
Observaciones:	Funcion que da un anuncio en pantalla de que el Sistema Operativo termino
					con su funcionamiento.
*/
void DibujaAnuncioCerrar()
{
	int columna = 29, fila = 7, i;
	char anuncio[] = "PROCESOS TERMINADOS";
	int Esperar = 500;

	// Escribir Anuncio Lentamente
	MoverCursor(columna, fila);
	for (i = 0; i < 19; i++)
	{
		EsperarMiliSeg(Esperar - 200);
		printf("%c", anuncio[i]);
	}

	// Parpadeo de Aununcio
	for (i = 0; i < 6; i++)
	{
		MoverCursor(columna, fila);
		EsperarMiliSeg(Esperar + 100);
		if (i % 2 == 0)
		{
			printf("                   ");
		}
		else
		{
			printf("%s", anuncio);
		}
	}
}
