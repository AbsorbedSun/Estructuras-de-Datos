/*
Simulaciones con el TAD Cola

Descripcion: Programa que con el uso de colas simula el comportamiento de un Supermercado de manera animada.

Autor: guimo
Integrantes:
				- Garcia Ambrosio Aldo
				- Ponce Espino Miguel Angel
				- Salinas Aquino Omar Ivan
Version 1.3 (2 de Junio 2025)

Observaciones: El programa necesita la biblioteca "presentacion.h", que contiene funciones para mover el cursor
en pantalla, pausar la ejecución y limpiar la pantalla. Tambien requiere las bibliotecas "TADColaEst.h"
y "TADCola/TADColaDin.h" para utilizar adecuadamente las estructuras de tipo cola. La compilación debe incluir
las definiciones correspondientes a las funciones, segun el sistema operativo en el que se trabaje (Windows o Linux).

Compilacion: 	gcc simulacion_01.c Recursos/presentacion/presentacion(Win|Lin).o Recursos/TADCola/TADCola(Din|Est|EstCirc).o -o simulacion_01 (Win si se correra en Windows | Lin si se ejecutara en Linux)  "Si se tiene el codigo objeto"
				gcc simulacion_01.c Recursos/presentacion/presentacion(Win|Lin).c Recursos/TADCola/TADCola(Din|Est|EstCirc).c -o simulacion_01 (Win si se correra en Windows | Lin si se ejecutara en Linux) "Si se tiene el codigo fuente"

Ejecucion: Windows simulacion_01.exe  &  Linux ./simulacion_01
*/

// LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <locale.h>
#include "Recursos/presentacion/presentacion.h"
// #include "TADCola/TADColaEst.h" 	//Si se usa la implemtentacion estatica (TADColaEst.c|TADColaEstCirc.c)
#include "Recursos/TADCola/TADColaDin.h" //Si se usa la implemtentacion dinamica (TADColaDin.c)

// CONSTANTES
#define ALTO 24 // Se piensa en un pantalla de 24 filas x 79 columnas
#define ANCHO 79
#define TIEMPO_BASE 10 // Tiempo base en milisegundos

// FUNCIONES
void PedirRequisitos(char *tienda, int *cajas, int *cajasT, int *clienteT);

void Simulacion(char tienda[], int cajas, int cajasT[], int clienteT);

void AgregarCliente(cola *cajera, int cajas, int clienteN, int filaC);
int AsignarCajaOptima(cola cajeras[], int cajas, int cajasT[], int atendidosC[]);
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

// PROGRAMA PRINCIPAL
int main(void)
{
	char tienda[52];
	int cajas, cajasT[10], clienteT;

	setlocale(LC_ALL, "");

	// Pedir parámetros de simulación
	PedirRequisitos(tienda, &cajas, cajasT, &clienteT);

	// Iniciar simulación
	Simulacion(tienda, cajas, cajasT, clienteT);

	MoverCursor(ANCHO, ALTO);
}

/*
	void PedirRequisitos(char *tienda, int *cajas, int *cajasT, int *clienteT);
	Recibe:	char * Referencia/Direccion a la cadena del nombre de la tienda, int * Referencia/Direccion al numero de cajas,
					int * Referencia/Direccion al arreglo de los tiempos de cajas, int * Referencia/Direccion al tiempo de llegada del cliente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que pide los datos como el nombre de la tienda, el numero de cajas, tiempo de consulta de cajas y el tiempo
					de llegada de los clientes, poniendo los resultados en las variables dadas.
*/
void PedirRequisitos(char *tienda, int *cajas, int *cajasT, int *clienteT)
{
	// Declaracion de variables
	int i;
	int Acajas, AclienteT;
	char Atienda[52];
	char correcto = 0;

	BorrarPantalla();

	// Pedir datos
	printf("Nombre de la tienda: \n");
	do
	{
		fgets(Atienda, sizeof(Atienda), stdin);
		// Verificar si la longitud de la cadena ingresada excede 50 caracteres (sin contar el '\n' de fgets)
		if (strlen(Atienda) == 51 && Atienda[50] != '\n')
		{
			printf("El nombre de la tienda tiene mas de 50 caracteres.\n");
			while (getchar() != '\n')
				;
		}
		else
		{
			// En caso de un salto de linea
			if (Atienda[strlen(Atienda) - 1] == '\n')
			{
				Atienda[strlen(Atienda) - 1] = '\0';
			}
			correcto = 1;
		}
	} while (correcto == 0);

	printf("Numero de cajeros: \n");
	do
	{
		scanf("%d", &Acajas);
		if (Acajas < 1 || Acajas > 10)
		{
			printf("Numero de cajas no valido (0< n < 11).\n");
		}
	} while (Acajas < 1 || Acajas > 10);

	printf("Tiempo de consulta por cajero: \n");
	for (i = 0; i < Acajas; i++)
	{
		do
		{
			scanf("%d", &cajasT[i]);
			if (cajasT[i] < 10 || cajasT[i] % 10 != 0)
			{
				printf("Tiempo invalido\n");
			}
		} while (cajasT[i] < 10 || cajasT[i] % 10 != 0);
	}

	printf("Tiempo de llegada de los clientes: \n");
	do
	{
		scanf("%d", &AclienteT);
		if (AclienteT < 10 || AclienteT % 10 != 0)
		{
			printf("Tiempo invalido\n");
		}
	} while (AclienteT < 10 || AclienteT % 10 != 0);

	strncpy(tienda, Atienda, 52);
	*cajas = Acajas;
	*clienteT = AclienteT;
}

/*
	void Simulacion(char tienda[], int cajas, int cajasT[], int clienteT)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas, int Arreglo de los tiempos de cajas, int Tiempo de llegada del cliente
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que simulara el comportamiento de un supermercado con base en las variables dadas por el
					usuario, permitiendo la llegada de clientes a colas, siendo atendidos en cajas para llegar a
					la cantidad de 100 clientes para que pueda parar la simulacion, solo si no hay mas clientes en
					las colas.
*/
void Simulacion(char tienda[], int cajas, int cajasT[], int clienteT)
{
	// Declaracion de variables
	int tiempo = 0, clienteN = 0, atendidos = 0;
	int i, cajas_ocupadas;
	elemento cliente_actual;

	// Crear las colas
	cola cajeras[cajas];
	int tiempoC[cajas];
	int atendidosC[cajas];

	// Inicializar colas
	for (i = 0; i < cajas; i++)
	{
		Initialize(&cajeras[i]);
		tiempoC[i] = 0;
		atendidosC[i] = 0;
	}

	// Iniciar Tienda
	AbrirTienda(tienda, cajas);

	// Ciclo infinito de la simulacion
	while (1)
	{
		EsperarMiliSeg(TIEMPO_BASE); // Esperar el tiempo base
		tiempo++;					 // Incrementar el contador de tiempo

		// --- FASE 1: Procesar atención en cajas ---
		for (i = 0; i < cajas; i++)
		{
			if ((tiempo + tiempoC[i]) % cajasT[i] == 0 && !Empty(&cajeras[i]))
			{
				if (!Empty(&cajeras[i]))
				{
					// Quitar Cliente de la Caja
					cliente_actual = QuitarCliente(&cajeras[i], cajas, i);
					atendidos++;
					atendidosC[i]++;

					// Actualizar visualización
					MoverCursor(30, 4);
					printf("%d", atendidos);
					MoverCursor(62, 4);
					printf("C%d - %d    ", i + 1, cliente_actual.n);
				}
			}
		}

		// --- FASE 2: Verificar condición de término ---
		if (atendidos >= 100)
		{
			cajas_ocupadas = 0;
			for (i = 0; i < cajas; i++)
			{
				if (!Empty(&cajeras[i]))
				{
					cajas_ocupadas++;
				}
			}
			if (cajas_ocupadas == 0)
			{
				CerrarTienda(tienda, cajas, atendidos, atendidosC, tiempo);
				break;
			}
		}
		// --- FASE 3: Manejar llegada de nuevos clientes ---
		if (tiempo % clienteT == 0)
		{
			clienteN++; // Incrementar el numero de clientes
			int caja_optima = AsignarCajaOptima(cajeras, cajas, cajasT, atendidosC);

			// Mantener el control del tiempo de cuando una caja empezo a tener clientes
			if (Empty(&cajeras[caja_optima]))
			{
				tiempoC[caja_optima] = (cajasT[caja_optima] - (tiempo % cajasT[caja_optima])) % cajasT[caja_optima];
			}

			// Agregar Cliente a la Caja
			AgregarCliente(&cajeras[caja_optima], cajas, clienteN, caja_optima);
		}
	}
	return;
}

/*
	int AsignarCajaOptima(cola cajeras[], int cajas, int cajasT[], int atendidosC[])
	Recibe:		Arreglo de colas que representan las cajas, int con el número total de cajas,
				Arreglo de enteros con los tiempos de atención por caja (en milisegundos),
				Arreglo de enteros con el número de clientes atendidos por cada caja.
	Devuelve:	int (Índice de la caja óptima para asignar al siguiente cliente, entre 0 y cajas-1)
	Observaciones:	Función que determina la mejor caja para enviar un nuevo cliente de forma equilibrada,
					usando una estrategia híbrida que considera la longitud de la cola actual (60%),
					la velocidad de atención de la caja (30%) y la distribución histórica de clientes atendidos (10%),
					con el objetivo de mantener un flujo eficiente y equitativo de atención en el supermercado.
*/
int AsignarCajaOptima(cola cajeras[], int cajas, int cajasT[], int atendidosC[])
{
	// Declaracion de variables
	int mejor_caja = 0;			   // Variable para guardar el índice de la mejor caja encontrada
	float mejor_puntaje = FLT_MAX; // Valor máximo inicial para comparación
	int total_clientes = 0;		   // Suma total de clientes atendidos por todas las cajas
	int i;

	// Calcular el total de clientes atendidos por todas las cajas
	for (i = 0; i < cajas; i++)
	{
		total_clientes += atendidosC[i];
	}

	// Evaluar cada caja para determinar su puntaje
	for (i = 0; i < cajas; i++)
	{
		int en_cola = Size(&cajeras[i]);	 // Obtener la cantidad actual de clientes en la cola
		float velocidad = 10.0f / cajasT[i]; // Calcular la "velocidad" de la caja (mayor valor = más rápida)

		// Calcular el puntaje de esta caja:
		// - 60% por la longitud de la cola (menos clientes es mejor)
		// - 30% por la lentitud de la caja (más rápida es mejor)
		// - 10% por equilibrio en la cantidad de clientes atendidos
		float puntaje = (0.5f * en_cola) +											 // Peso de la cola
						(0.3f * (1.0f / velocidad)) +								 // Peso de la velocidad (inverso porque queremos rápido)
						(total_clientes > 0 ?										 // Evitar división entre cero
							 0.2f * (1.0f - (atendidosC[i] / (float)total_clientes)) // Peso del equilibrio histórico
											: 0);

		// Si el puntaje de esta caja es mejor (menor), actualizar la mejor caja
		if (puntaje < mejor_puntaje)
		{
			mejor_puntaje = puntaje;
			mejor_caja = i;
		}
	}

	// Devolver el índice de la caja seleccionada
	return mejor_caja;
}

/*
	void AgregarCliente(cola *cajera, int clienteN, int filaC)
	Recibe:	cola * Referencia/Direccion a la cola de un cajero, int Numero de clientes, int Numero de Caja
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que agrega un cliente a la cola de un cajero, mostrando tambien como este es
					ingresado a la cola en pantalla.
*/
void AgregarCliente(cola *cajera, int cajas, int clienteN, int filaC)
{
	// Declaracion de variables
	int espacio = 5 + (7 * filaC) + ((cajas == 10) ? 0 : ((cajas % 2 == 0) ? 0 : 4) + ((10 - cajas) / 2) * 7);
	int fila = 10, columna = espacio;
	elemento e;
	int tam;

	// Si la cola está vacía, imprimir directamente el número del cliente en la posicion base
	if (Empty(cajera))
	{
		MoverCursor(columna, fila);
		// Imprime el numero del cliente con formato para alinear visualmente
		if (clienteN < 100)
		{
			printf("  %d", clienteN);
		}
		else if (clienteN < 10000)
		{
			printf(" %d", clienteN);
		}
		else
		{
			printf("%d", clienteN);
		}
	}
	else
	{
		tam = Size(cajera); // Obtener el tamaño actual de la cola
		fila++;				// Mover la impresión una línea hacia abajo

		// Si hay menos de 10 elementos, imprimir el cliente en la siguiente línea disponible
		if (tam < 10)
		{
			MoverCursor(columna, fila + tam);
			// Formato del número del cliente
			if (clienteN < 100)
			{
				printf("  %d", clienteN);
			}
			else if (clienteN < 10000)
			{
				printf(" %d", clienteN);
			}
			else
			{
				printf("%d", clienteN);
			}
		}
		else
		{
			// Si hay más de 9 elementos en la cola, mostrar una línea resumen con "+N"
			tam -= 9;
			MoverCursor(columna, fila + 10);
			if (tam < 10)
			{
				printf("  +%d", tam);
			}
			else if (tam < 1000)
			{
				printf(" +%d", tam);
			}
			else
			{
				printf("+%d", tam);
			}
		}
	}
	// Pausa breve para simular animación
	EsperarMiliSeg(20);
	// Asignar número al nuevo elemento y agregarlo a la cola
	e.n = clienteN;
	Queue(cajera, e);
}

/*
	elemento QuitarCliente(cola *cajera, int filaC)
	Recibe:	cola * Referencia/Direccion a la cola de un cajero, int Numero de Caja
	Devuelve:	elemento Elemento
	Observaciones:	Funcion que quita un cliente de cola de un cajero, mostrando tambien como este es
					eliminado de la cola en pantalla, retornando el valor del elemento.
*/
elemento QuitarCliente(cola *cajera, int cajas, int filaC)
{
	// Declaracion de variables
	int espacioHorizontal = 5 + (7 * filaC) + ((cajas == 10) ? 0 : ((cajas % 2 == 0) ? 0 : 4) + ((10 - cajas) / 2) * 7);
	int filaBase = 10, columna = espacioHorizontal;
	int cantidadClientes, i;
	elemento clienteQuitado, clienteEnCola;

	// Quitar el primer cliente de la cola
	clienteQuitado = Dequeue(cajera);

	// Borrar visualmente la posición anterior del primer cliente
	MoverCursor(columna, filaBase);

	// Si la cola quedó vacía, se borra la celda visual
	if (Empty(cajera))
	{
		printf("      "); // Borrado visual
	}
	else
	{
		// Obtener el número de clientes restantes
		cantidadClientes = Size(cajera);

		// Reimprimir los clientes restantes en la pantalla
		for (i = 0; i <= cantidadClientes + 1; i++)
		{
			MoverCursor(columna, filaBase + i);
			printf("      "); // Limpiar línea anterior

			MoverCursor(columna, filaBase + i);

			// Detener si ya imprimimos todos o llegamos al límite visual
			if (i == cantidadClientes + 1 || i == 11)
				break;

			// Obtener el cliente en turno para mostrar
			clienteEnCola = Element(cajera, (i == 0 ? 1 : i));

			// Mostrar número de cliente con alineación
			if (clienteEnCola.n < 100)
			{
				printf("  %d", clienteEnCola.n);
			}
			else if (clienteEnCola.n < 10000)
			{
				printf(" %d", clienteEnCola.n);
			}
			else
			{
				printf("%d", clienteEnCola.n);
			}

			// Evitar repetición del primer elemento
			if (i == 0)
				i++;
		}

		// Si hay más de 10 clientes, mostrar el exceso con formato "+N"
		if (cantidadClientes > 10)
		{
			int excedente = cantidadClientes - 10;

			if (excedente < 10)
				printf("  +%d", excedente);
			else if (excedente < 1000)
				printf(" +%d", excedente);
			else
				printf("+%d", excedente);
		}
	}

	// Pausa breve para simular animación
	EsperarMiliSeg(10);

	// Devolver el cliente que fue retirado
	return clienteQuitado;
}

/*
	void AbrirTienda(char tienda[], int cajas)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que abre la tienda de manera animada en pantalla.
*/
void AbrirTienda(char tienda[], int cajas)
{
	DibujaPresentacion();
	DibujaMarco(tienda);
	DibujaCajas(cajas);
	DibujaEstantes(cajas);
	DibujaAnuncioAbrir();
}

/*
	void CerrarTienda(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo)
	Recibe:	char Cadena del nombre de la tienda, int Numero de cajas, int Numero de clientes atendidos,
				int Arreglo de clientes atendidos por caja, int Tiempo de ejecucion
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que cierra la tienda de manera animada en pantalla.
*/
void CerrarTienda(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo)
{
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
	Observaciones:	Funcion que da una presentacion a la simulacion 01.
*/
void DibujaPresentacion()
{
	int columna, fila, i;

	BorrarPantalla();

	// TELÓN QUE SE ABRE DE IZQUIERDA A DERECHA
	srand(time(NULL));

	// Telón cerrado inicial (toda la pantalla llena de asteriscos)
	for (columna = 0; columna < ANCHO; columna++)
	{
		for (fila = 0; fila < ALTO; fila++)
		{
			MoverCursor(columna, fila);
			printf("*");
		}
	}
	EsperarMiliSeg(1000);

	// Animación de apertura (izquierda a derecha)
	for (columna = 0; columna < ANCHO; columna += 3)
	{ // Paso de 3 columnas
		// Dibuja una línea vertical que "abre" el telón
		for (fila = 0; fila < ALTO; fila++)
		{
			MoverCursor(columna, fila);
			printf(" ");
			MoverCursor(columna + 1, fila);
			printf(" ");
			MoverCursor(columna + 2, fila);
			printf(" ");
		}
		EsperarMiliSeg(50); // Velocidad de apertura
	}

	// FIN DEL TELÓN (el resto del código original sigue igual)

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
	MoverCursor(28, 9);
	printf("\tGUIMO Company");
	MoverCursor(28, 15);
	printf("    Ig: g.u.i.m.o");

	// Crear Segundo Cuadro
	EsperarMiliSeg(2200);
	for (columna = 22; columna <= 57; columna++)
	{
		for (fila = 17; fila <= 20; fila++)
		{
			MoverCursor(columna, fila);
			if ((columna > 22 && columna < 57) && (fila == 17 || fila == 20))
			{
				printf("_");
			}
			else if ((fila > 17 && fila <= 20) && (columna == 22 || columna == 57))
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
	MoverCursor(34, 19);
	printf("SUPERMERCADO OCSO");

	EsperarMiliSeg(5000);
	BorrarPantalla();
}

/*
	void DibujaMarco(char tienda[])
	Recibe:	char Cadena del nombre de la tienda
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que crea el todo el marco del Supermercado asi como sus datos iniciales.
*/
void DibujaMarco(char tienda[])
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
		MoverCursor(columna, fila + 4);
		printf("_");
		MoverCursor(columna, ALTO - 1);
		printf("_");
		EsperarMiliSeg(Esperar);
	}

	// Crear texto
	MoverCursor(5, 3);
	printf("\t\"%s\"", tienda);
	EsperarMiliSeg(200);
	MoverCursor(5, 4);
	printf("Ultimo movimiento:");
	EsperarMiliSeg(200);
	MoverCursor(42, 4);
	printf("Num. Clientes Atendidos: 0");
	EsperarMiliSeg(200);
	MoverCursor(65, 3);
	printf("-/       |-");
	EsperarMiliSeg(Esperar);
}

/*
	void DibujaCajas(int cajas)
	Recibe:	int Numero de cajas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que crea las cajas que usara el Supermercado con base en un
					valor de cajas dado.
*/
void DibujaCajas(int cajas)
{
	// Cálculo del espacio inicial en pantalla para centrar las cajas horizontalmente
	int espacio = 4 + ((cajas == 10) ? 0 : ((cajas % 2 == 0) ? 0 : 4) + ((10 - cajas) / 2) * 7);
	int columna = espacio, fila = 6, i, j;
	int Esperar = 5; // Tiempo de espera entre impresiones para simular animación

	// Bucle para dibujar cada una de las cajas
	for (columna = espacio, i = 1; i <= cajas; columna += 7, i++)
	{
		// Dibuja la parte superior e inferior de la caja (techo y piso)
		for (j = 1; j <= 6; j++)
		{
			MoverCursor(columna + j, fila); // Mover cursor a la parte superior
			printf("_");					// Dibujar techo

			MoverCursor(columna + j, fila + 3); // Mover cursor a la parte inferior
			printf("_");						// Dibujar piso

			EsperarMiliSeg(Esperar); // Pequeña pausa para animación
		}

		// Dibuja las paredes y los separadores entre cajas
		for (j = 1; j <= 15; j++)
		{
			// Línea especial (línea 4): separador visual central
			if (j == 4)
			{
				MoverCursor(columna, fila + j);
				if (i == 1)
				{
					printf(":"); // Límite izquierdo de la primera caja
				}
				else
				{
					printf("-"); // Separador entre cajas
				}

				MoverCursor(columna + 7, fila + j);
				if (i == cajas)
				{
					printf(":"); // Límite derecho de la última caja
				}
				else
				{
					printf("-"); // Separador entre cajas
				}
				continue;
			}

			// Línea especial (línea 5): separador inferior
			if (j == 5)
			{
				MoverCursor(columna, fila + j);
				if (i == 1)
				{
					printf(":"); // Límite izquierdo
				}
				else
				{
					printf("."); // Punto para separación visual
				}

				MoverCursor(columna + 7, fila + j);
				if (i == cajas)
				{
					printf(":"); // Límite derecho
				}
				else
				{
					printf("."); // Punto de separación
				}
				continue;
			}

			// Dibuja las paredes laterales de la caja
			MoverCursor(columna, fila + j);
			printf("|");

			MoverCursor(columna + 7, fila + j);
			printf("|");

			EsperarMiliSeg(Esperar); // Animación
		}

		// Imprimir etiqueta con el número de la caja (centrado)
		MoverCursor(columna + (i == 10 ? 2 : 3), fila + 2);
		printf("C%d", i);
	}
}

/*
	void DibujaEstantes(int cajas)
	Recibe:	int Numero de cajas
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que dibuja Estantes de decoracion para la simulacion.
*/
void DibujaEstantes(int cajas)
{
	int espacio = 3 + (7 * cajas) + ((cajas == 10) ? 0 : ((cajas % 2 == 0) ? 0 : 4) + ((10 - cajas) / 2) * 7);
	int columna = 4, fila = 6, i, j, k;
	int Esperar = 5;
	int conteo = 1;

	// Comprobar que hay menos de 10 cajas
	//  if(cajas==10) return;

	// Crear Estantes Grandes
	for (columna = 4, fila = 6, i = 1; i <= 2; columna += espacio, i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (j == 0)
			{
				MoverCursor(columna + 1, fila + j);
				for (k = 0; k < (10 - cajas) * 3 - 2; k++)
				{
					printf("_");
				}
			}
			else
			{
				MoverCursor(columna, fila + j);
				if (j % 2 != 0)
				{
					for (k = 0; k < 10 - cajas; k++)
					{
						if (k % 2 == 0)
							printf("( )");
						else
							printf("[ ]");
					}
				}
				else
				{
					for (k = 0; k < 10 - cajas; k++)
					{
						if (k % 2 == 0)
							printf("[ ]");
						else
							printf("( )");
					}
				}
			}
			EsperarMiliSeg(Esperar);
		}
	}

	// Crear Estantes Chicos
	for (columna = 4, fila = 12, i = 1; i <= 2; columna += espacio, i++)
	{
		for (j = 0; j < 5; j++)
		{
			MoverCursor(columna, fila + j * 2);
			for (k = 0; k < 10 - cajas; k++)
			{
				printf(">|<");
			}
			EsperarMiliSeg(Esperar);
		}
	}
}

/*
	void DibujaCortina()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que crea una cortina desendente en la pantalla.
*/
void DibujaCortina()
{
	int columna, fila, i, j;

	EsperarMiliSeg(800);

	for (fila = 1, columna = 1; fila < ALTO; fila++)
	{
		MoverCursor(columna, fila);
		printf("|||");
		MoverCursor(ANCHO - 4, fila);
		printf("||||");

		if (fila - 1 > 0)
		{
			MoverCursor(columna, fila - 1);
			printf("| |");
			for (i = 0; i < 14; i++)
				printf(" |__|");
			printf(" |  |");
		}

		if (fila - 2 > 0)
		{
			MoverCursor(columna, fila - 2);
			printf("| |");
			for (i = 0; i < 15; i++)
				printf("_|  |");
		}

		if (fila - 3 > 0)
		{
			MoverCursor(columna, fila - 3);
			printf("| |");
			for (i = 0; i < 15; i++)
				printf(" |  |");
		}

		EsperarMiliSeg(200);
	}
	MoverCursor(4, ALTO - 1);
	for (i = 0; i < 14; i++)
		printf("     ");
	printf(" ");

	EsperarMiliSeg(1500);
}

/*
	void DibujaCartel()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que crea un cartel desendente en la pantalla.
*/
void DibujaCartel()
{
	int columna, fila, i, j;

	for (fila = 1, columna = 9; fila < ALTO - 4; fila++)
	{
		MoverCursor(columna, fila);
		printf("|");
		for (i = 0; i < 59; i++)
			printf("_");
		printf("|");

		if (fila - 1 > 0)
		{
			MoverCursor(columna, fila - 1);
			printf("|");
			for (i = 0; i < 59; i++)
				printf(" ");
			printf("|");
		}

		if (fila - 14 > 0)
		{
			MoverCursor(columna, fila - 14);
			printf(" ");
			for (i = 0; i < 59; i++)
				printf("_");
			printf(" ");
		}

		if (fila - 15 > 0)
		{
			MoverCursor(columna, fila - 15);
			for (i = 0; i < 12; i++)
			{
				if (i == 2 || i == 10)
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
				int Arreglo de clientes atendidos por caja, int Tiempo de ejecucion
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que agrega los datos al cartel del finalizamiento de la simulacion.
*/
void DibujaCartelDatos(char tienda[], int cajas, int atendidos, int atendidosC[], int tiempo)
{
	int columna, fila, i, j, k;
	int mejorC = 0;

	// Calcular mejor Caja
	for (i = 0; i < cajas; i++)
	{
		if (mejorC < atendidosC[i])
		{
			mejorC = atendidosC[i];
		}
	}

	// Mostrar datos finales
	for (columna = 12, fila = 7, i = 1; i <= 5; fila += 2, i++)
	{
		EsperarMiliSeg(600);
		MoverCursor(columna, fila);

		if (i == 1)
		{
			printf("\"%s\"", tienda);
			columna--;
		}

		if (i == 2)
			printf("Tiempo abierta:    %d Milisegundos", tiempo);

		if (i == 3)
			printf("Clientes Atendidos:    %d Clientes", atendidos);

		if (i == 4)
		{
			printf("Cajas mas eficientes:    ");
			for (j = 0, k = 0; j < cajas; j++)
			{
				if (mejorC == atendidosC[j])
				{
					if (k > 0)
						printf(",");
					printf("C%d", j + 1);
					k++;
				}
			}
		}

		if (i == 5)
		{
			printf("Clientes Atendidos por Caja:");
			for (columna += 2, fila++, j = 0; j < cajas; j++)
			{
				EsperarMiliSeg(200);
				MoverCursor(columna, fila);
				printf("C%d: %d", j + 1, atendidosC[j]);
				if ((j + 1) % 3 != 0)
				{
					fila++;
				}
				else
				{
					fila -= 2;
					columna += 14;
				}
			}
		}
	}
}

/*
	void DibujaAnuncioAbrir()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que da un anuncio en pantalla de que la tienda va a abrir.
*/
void DibujaAnuncioAbrir()
{
	int columna = 67, fila = 3, i;
	char anuncio[] = "ABIERTO";
	int Esperar = 800;

	// Escribir Conteo
	MoverCursor(columna, fila);
	for (i = 3; i > 0; i--)
	{
		EsperarMiliSeg(Esperar);
		printf(" %d", i);
	}
	EsperarMiliSeg(Esperar);

	// Quitar Conteo
	MoverCursor(columna, fila);
	printf("       ");
	EsperarMiliSeg(Esperar);

	// Escribir ABIERTO Lentamente
	MoverCursor(columna, fila);
	for (i = 0; i < 7; i++)
	{
		EsperarMiliSeg(Esperar - 300);
		printf("%c", anuncio[i]);
	}

	// Parpadeo de Aununcio
	for (i = 0; i < 4; i++)
	{
		MoverCursor(columna, fila);
		EsperarMiliSeg(Esperar - 200);
		if (i % 2 == 0)
		{
			printf("       ");
		}
		else
		{
			printf("%s", anuncio);
		}
	}
}

/*
	void DibujaAnuncioCerrar()
	Recibe:	void (No recibe valor explicito)
	Devuelve:	void (No retorna valor explicito)
	Observaciones:	Funcion que da un anuncio en pantalla de que la tienda va a cerrar.
*/
void DibujaAnuncioCerrar()
{
	int columna = 67, fila = 3, i;
	char anuncio[] = "CERRADO";
	int Esperar = 800;

	// Quitar Cartel de Abierto
	MoverCursor(columna, fila);
	printf("       ");
	EsperarMiliSeg(Esperar);

	// Escribir Cerrado Lentamente
	MoverCursor(columna, fila);
	for (i = 0; i < 7; i++)
	{
		EsperarMiliSeg(Esperar - 300);
		printf("%c", anuncio[i]);
	}

	// Parpadeo de Aununcio
	for (i = 0; i < 8; i++)
	{
		MoverCursor(columna, fila);
		EsperarMiliSeg(Esperar - 200);
		if (i % 2 == 0)
		{
			printf("       ");
		}
		else
		{
			printf("%s", anuncio);
		}
	}
}
