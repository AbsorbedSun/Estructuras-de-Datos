/*
Simulacion_Banco.c
Simulaciones con el TAD Cola

Descripcion: Programa que con el uso de colas simula el comportamiento de un Banco de manera animada,
siguiendo la estructura y estilo de Simulacion_01, pero adaptado a la lógica de un banco.

Autor: Adaptado por ChatGPT
Integrantes:
				- Garcia Ambrosio Aldo
				- Ponce Espino Miguel Angel
				- Salinas Aquino Omar Ivan
Version 1.6 (02 de Junio 2025)

Observaciones: El programa necesita la biblioteca "presentacion.h" (funciones para mover el cursor,
pausar y limpiar pantalla) y la biblioteca "TADCola/TADColaDin.h" para el uso de colas dinámicas.
La compilación debe incluir las definiciones de las funciones según el sistema operativo:
    gcc simulacion_03.c Recursos/presentacion/presentacion(Win|Lin).c Recursos/TADCola/TADColaDin.c -o Simulacion_BancoD
    gcc simulacion_03.c Recursos/presentacion/presentacion(Win|Lin).c Recursos/TADCola/TADColaEst.c -o Simulacion_BancoE
    gcc simulacion_03.c Recursos/presentacion/presentacion(Win|Lin).c Recursos/TADCola/TADColaEstCir.c -o Simulacion_BancoEC

Ejecucion: Windows: Simulacion_Banco.exe
           Linux:   ./Simulacion_Banco
*/

// LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include "Base/presentacion/presentacion.h"
// #include "Recursos/TADCola/TADColaEst.h"  // Si se usa la implementacion estatica
// #include "Recursos/TADCola/TADColaEstCir.h"  // Si se usa la implementacion estatica circular
#include "Recursos/TADCola/TADColaDin.h"   // Implementacion dinamica de colas

// CONSTANTES
#define ALTO        24   // Pantalla de 24 filas x 79 columnas
#define ANCHO       95
#define TIEMPO_BASE 10   // Tiempo base en milisegundos

// PROTOTIPOS DE FUNCIONES
void PedirRequisitos(int *cajeros, int *tiempoAtencion, boolean cajasAbiertas[],
                     int *tLlegadaCliente, int *tLlegadaUsuario, int *tLlegadaPreferente);

void Simulacion(int cajeros, int tiempoAtencion, boolean cajasAbiertas[],
                int tCliente, int tUsuario, int tPreferente);

void AgregarClienteALaCaja(boolean cajasAbiertas[], cola *caja, cola *fila, int idxCaja, int tipoFila);
elemento QuitarClienteDeCaja(boolean cajasAbiertas[], cola *caja, int idxCaja);
void AgregarClienteAFila(cola *fila, int numeroCliente, int tipo);
elemento QuitarClienteDeFila(cola *fila, int tipo);

void AbrirBanco(int cajeros, boolean cajasAbiertas[]);
void DibujaPresentacion();
void DibujaMarco();
void DibujaCajas(int cajeros, boolean cajasAbiertas[]);
void DibujaFilas();
void DibujaDecoracion();
void DibujaAnuncioAbrir();

// PROGRAMA PRINCIPAL
int main(void)
{
    int cajeros;
    int tiempoAtencion;
    boolean cajasAbiertas[10];
    int tLlegadaCliente, tLlegadaUsuario, tLlegadaPreferente;

    setlocale(LC_ALL, "");

    // Pedir parametros de simulacion al usuario
    PedirRequisitos(&cajeros, &tiempoAtencion, cajasAbiertas,
                    &tLlegadaCliente, &tLlegadaUsuario, &tLlegadaPreferente);

    // Iniciar simulacion del banco
    Simulacion(cajeros, tiempoAtencion, cajasAbiertas,
               tLlegadaCliente, tLlegadaUsuario, tLlegadaPreferente);

    // Colocar cursor al final de la pantalla
    MoverCursor(ANCHO, ALTO);
    return 0;
}

/*
    void PedirRequisitos(int *cajeros, int *tiempoAtencion, boolean cajasAbiertas[],
                         int *tLlegadaCliente, int *tLlegadaUsuario, int *tLlegadaPreferente);
    Recibe: int * Referencia al numero de cajeros,
            int * Referencia al tiempo de atencion de los cajeros,
            boolean [] Arreglo indicando que cajeros (indices 0..9) estan abiertos,
            int * Referencia al tiempo de llegada de clientes generales,
            int * Referencia al tiempo de llegada de usuarios,
            int * Referencia al tiempo de llegada de clientes preferentes.
    Devuelve: void
    Observaciones: Funcion que pide al usuario el numero de cajeros, asigna aleatoriamente
                   que cajeros estan abiertos, solicita el tiempo de atencion (igual para todos),
                   y los tiempos de llegada de los tres tipos de clientes.
*/
void PedirRequisitos(int *cajeros, int *tiempoAtencion, boolean cajasAbiertas[],
                     int *tLlegadaCliente, int *tLlegadaUsuario, int *tLlegadaPreferente)
{
    int i, numCajeros;
    int tAtencion, tC, tU, tP;
    int abiertaCont = 0;

    BorrarPantalla();
    // Solicitar numero de cajeros
    printf("Cuantos cajeros quiere que el Banco tenga? (1 - 10):\n");
    do {
        scanf("%d", &numCajeros);
        if (numCajeros < 1 || numCajeros > 10) {
            printf("Numero de cajeros no valido (1 <= n <= 10).\n");
        }
    } while (numCajeros < 1 || numCajeros > 10);

    // Inicializar arreglo de cajeros cerrados
    for (i = 0; i < 10; i++) {
        cajasAbiertas[i] = FALSE;
    }

    // Aleatorizar que cajeros estan abiertos
    srand((unsigned)time(NULL));
    while (abiertaCont < numCajeros) {
        i = rand() % 10;
        if (cajasAbiertas[i] == FALSE) {
            cajasAbiertas[i] = TRUE;
            abiertaCont++;
        }
    }

    // Solicitar tiempo de atencion (igual para cada cajero)
    printf("\nCual es el tiempo de atencion de cada cajero? (multiplo de 10, >=10):\n");
    do {
        scanf("%d", &tAtencion);
        if (tAtencion < 10 || tAtencion % 10 != 0) {
            printf("Tiempo invalido.\n");
        }
    } while (tAtencion < 10 || tAtencion % 10 != 0);

    // Solicitar tiempo de llegada de clientes generales
    printf("\nCual es el tiempo de llegada de los clientes generales? (multiplo de 10, >=10):\n");
    do {
        scanf("%d", &tC);
        if (tC < 10 || tC % 10 != 0) {
            printf("Tiempo invalido.\n");
        }
    } while (tC < 10 || tC % 10 != 0);

    // Solicitar tiempo de llegada de usuarios
    printf("\nCual es el tiempo de llegada de los usuarios? (multiplo de 10, >=10):\n");
    do {
        scanf("%d", &tU);
        if (tU < 10 || tU % 10 != 0) {
            printf("Tiempo invalido.\n");
        }
    } while (tU < 10 || tU % 10 != 0);

    // Solicitar tiempo de llegada de clientes preferentes
    printf("\nCual es el tiempo de llegada de los clientes preferentes? (multiplo de 10, >=10):\n");
    do {
        scanf("%d", &tP);
        if (tP < 10 || tP % 10 != 0) {
            printf("Tiempo invalido.\n");
        }
    } while (tP < 10 || tP % 10 != 0);

    // Asignar valores de salida
    *cajeros = numCajeros;
    *tiempoAtencion = tAtencion;
    *tLlegadaCliente = tC;
    *tLlegadaUsuario = tU;
    *tLlegadaPreferente = tP;
}

/*
    void Simulacion(int cajeros, int tiempoAtencion, boolean cajasAbiertas[],
                    int tCliente, int tUsuario, int tPreferente);
    Recibe: int Numero de cajeros,
            int Tiempo de atencion uniforme para cada cajero,
            boolean [] Arreglo indicando que cajeros estan abiertos,
            int Tiempo de llegada de clientes generales,
            int Tiempo de llegada de usuarios,
            int Tiempo de llegada de clientes preferentes.
    Devuelve: void
    Observaciones: Funcion que simula el comportamiento del banco. Se crean tres filas (colas):
                   clientes generales, usuarios y preferentes. En cada iteracion de tiempo:
                   1) Procesa atencion en cajeros abiertos (si corresponde).
                   2) Agrega a las filas los nuevos clientes que llegan.
                   3) Asigna de forma aleatoria clientes de fila a cajeros vacios,
                      priorizando a los preferentes, luego usuarios y finalmente generales,
                      respetando limites de servicio continuo.
*/
void Simulacion(int cajeros, int tiempoAtencion, boolean cajasAbiertas[],
    int tCliente, int tUsuario, int tPreferente)
{
    int tiempo = 0;
    int clientesCont[3] = { 0, 0, 0 }; // 0: generales, 1: usuarios, 2: preferentes
    int atendidosTotal = 0;
    int tiemposLlegada[3] = { tCliente, tUsuario, tPreferente };
    int i, cajeroAleatorio;
    boolean hayCajaLibre;
    elemento e;

    // Crear e inicializar las tres filas de espera
    cola filas[3];
    for (i = 0; i < 3; i++) {
        Initialize(&filas[i]);
    }

    // Crear e inicializar colas para cada cajero
    cola colasCajeros[cajeros];
    int tiempoInicioAtencion[cajeros];  // Tiempo cuando el cajero empezó a atender al cliente actual
    int atendidosPorCajero[cajeros];
    for (i = 0; i < cajeros; i++) {
        Initialize(&colasCajeros[i]);
        tiempoInicioAtencion[i] = -1;  // -1 indica que no está atendiendo a nadie
        atendidosPorCajero[i] = 0;
    }

    // Mostrar animacion de apertura del banco
    AbrirBanco(cajeros, cajasAbiertas);

    // Bucle infinito de la simulacion
    while (1) {
    EsperarMiliSeg(TIEMPO_BASE);
    tiempo++;  // Incrementar contador de tiempo

    // --- FASE 1: Procesar atencion en cada cajero abierto ---
    for (i = 0; i < cajeros; i++) {
    // Solo procesar si la caja i esta abierta
    int conteoAbiertas = 0, idxC = -1;
        for (int j = 0; j < 10; j++) {
            if (cajasAbiertas[j]) {
                if (conteoAbiertas == i) {
                    idxC = j;
                    break;
                }
                conteoAbiertas++;
            }
        }

        // Si el cajero tiene un cliente y no está atendiendo, iniciar atención
        if (idxC != -1 && !Empty(&colasCajeros[i]) && tiempoInicioAtencion[i] == -1) {
            tiempoInicioAtencion[i] = tiempo;
        }

        // Si el cajero está atendiendo y ha pasado el tiempo necesario
        if (idxC != -1 && !Empty(&colasCajeros[i]) && 
            tiempoInicioAtencion[i] != -1 && 
            (tiempo - tiempoInicioAtencion[i]) >= tiempoAtencion) {
            
            // Quitar cliente de la caja
            e = QuitarClienteDeCaja(cajasAbiertas, &colasCajeros[i], i);
            atendidosTotal++;
            atendidosPorCajero[i]++;
            tiempoInicioAtencion[i] = -1;  // Resetear tiempo de atención
            
            // Actualizar visualizacion
            MoverCursor(28, 3);
            printf("[ BANCO PLUS - Simulacion ]");
            MoverCursor(3, 5);
            printf("Clientes Ati. : %3d", atendidosTotal);
            MoverCursor(50, 5);
            printf("Ultimo Mov. : %3c%d", e.c, e.n);
            MoverCursor(60, 3);
            printf("V%2d: %c%d  ", i + 1, e.c, e.n);
        }
    }

    // --- FASE 2: Llegada de nuevos clientes a sus filas ---
    for (i = 0; i < 3; i++) {
        if (tiempo % tiemposLlegada[i] == 0) {
            clientesCont[i]++;
            AgregarClienteAFila(&filas[i], clientesCont[i], i);
        }
    }

    // --- FASE 3: Asignar clientes de filas a cajeros libres ---
    do {
    // Verificar si hay al menos un cajero vacio
    hayCajaLibre = FALSE;
    for (i = 0; i < cajeros; i++) {
        if (Empty(&colasCajeros[i])) {
            hayCajaLibre = TRUE;
            break;
        }
    }
    if (!hayCajaLibre) {
        break;
    }

    // Priorizar preferentes (2), luego usuarios (1), luego generales (0)
    int tipoAsignar = -1;
    if (!Empty(&filas[2])) {
        tipoAsignar = 2;
    } else if (!Empty(&filas[1])) {
        tipoAsignar = 1;
    } else if (!Empty(&filas[0])) {
        tipoAsignar = 0;
    }

    if (tipoAsignar == -1) {
        // No hay clientes en ninguna fila
        break;
    }

    // Escoger cajero aleatorio que este libre
    do {
        cajeroAleatorio = rand() % cajeros;
    } while (!Empty(&colasCajeros[cajeroAleatorio]));

    // Agregar cliente de la fila correspondiente a ese cajero
    AgregarClienteALaCaja(cajasAbiertas, &colasCajeros[cajeroAleatorio],
                        &filas[tipoAsignar], cajeroAleatorio, tipoAsignar);
    } while (hayCajaLibre);

    // La simulacion del banco es continua y no tiene condicion de cierre explicita
    }
}

/*
    void AgregarClienteALaCaja(boolean cajasAbiertas[], cola *caja, cola *fila,
                               int idxCaja, int tipoFila);
    Recibe: boolean [] Arreglo indicando que cajeros estan abiertos,
            cola * Referencia a la cola del cajero seleccionado,
            cola * Referencia a la cola de la fila correspondiente al tipo de cliente (0,1,2),
            int Indice interno del cajero (0..cajeros-1),
            int Tipo de fila (0: generales, 1: usuarios, 2: preferentes).
    Devuelve: void
    Observaciones: Funcion que quita animadamente el primer cliente de la fila indicada
                   y lo pone en pantalla frente al cajero libre. Luego lo encola en la cola
                   interna del cajero para simular su atencion.
*/
void AgregarClienteALaCaja(boolean cajasAbiertas[], cola *caja, cola *fila,
                           int idxCaja, int tipoFila)
{
    int filaPantalla =  8;  // Ajuste vertical: un poco arriba de los recuadros de cajeros
    int columnaBase =  4;   // Punto de partida horizontal
    int conteoAbiertas = 0;
    int j;

    // Quitar cliente de la fila de espera
    elemento aux = QuitarClienteDeFila(fila, tipoFila);

    // Calcular columna en que se mostrara el cliente segun el orden de cajas abiertas
    for (j = 0; j < 10; j++) {
        if (cajasAbiertas[j]) {
            if (conteoAbiertas == idxCaja) {
                break;
            }
            conteoAbiertas++;
        }
    }
    int columna = columnaBase + (j * 9);  // Ahora cada cajero ocupa 9 columnas

    // Mostrar numero y tipo de cliente frente a la caja, centrado en 7 espacios
    MoverCursor(columna + 1, filaPantalla);
    if (aux.n < 10) {
        printf("[ %c%d  ]", aux.c, aux.n);
    } else if (aux.n < 100) {
        printf("[ %c%d ]", aux.c, aux.n);
    } else {
        printf("[%c%d ]", aux.c, aux.n);
    }
    EsperarMiliSeg(20);

    // Encolar en la cola interna del cajero
    elemento e;
    e.n = aux.n;
    e.c = aux.c;
    Queue(caja, e);
}

/*
    elemento QuitarClienteDeCaja(boolean cajasAbiertas[], cola *caja, int idxCaja);
    Recibe: boolean [] Arreglo indicando que cajeros estan abiertos,
            cola * Referencia a la cola del cajero,
            int Indice interno del cajero (0..cajeros-1).
    Devuelve: elemento El cliente que se retira de la caja para ser atendido.
    Observaciones: Funcion que borra animadamente el cliente que acaba de terminar
                   de ser atendido en esa caja y devuelve su informacion.
*/
elemento QuitarClienteDeCaja(boolean cajasAbiertas[], cola *caja, int idxCaja)
{
    int filaPantalla =  8;  // Ajuste vertical: un poco arriba de los recuadros de cajeros
    int columnaBase =  4;
    int conteoAbiertas = 0;
    int j;

    // Calcular columna en que estaba el cliente segun el orden de cajas abiertas
    for (j = 0; j < 10; j++) {
        if (cajasAbiertas[j]) {
            if (conteoAbiertas == idxCaja) {
                break;
            }
            conteoAbiertas++;
        }
    }
    int columna = columnaBase + (j * 9);

    // Borrar visualmente el espacio frente a la caja (7 caracteres + espacios de borde)
    MoverCursor(columna + 1, filaPantalla);
    printf("       ");
    EsperarMiliSeg(20);

    // Dequeue del cliente y regresarlo
    elemento e = Dequeue(caja);
    return e;
}

/*
    void AgregarClienteAFila(cola *fila, int numeroCliente, int tipo);
    Recibe: cola * Referencia a la cola de la fila correspondiente,
            int Numero del nuevo cliente,
            int Tipo de cliente (0: generales, 1: usuarios, 2: preferentes).
    Devuelve: void
    Observaciones: Funcion corregida para usar las nuevas posiciones centradas y 
                   limpiar correctamente los caracteres residuales.
*/
void AgregarClienteAFila(cola *fila, int numeroCliente, int tipo)
{
    int filaPantalla = 15;  // Ajuste vertical: justo dentro del recuadro de fila
    int baseX = 12;         // Nueva base horizontal ajustada
    int espacio = 28;       // Nueva separación entre filas
    int columnaBase = baseX + (espacio * tipo);  // Base horizontal para cada fila
    int centramiento = 3;  // Offset para centrar dentro del recuadro de 9 columnas
    elemento e;
    int tamActual;

    e.n = numeroCliente;
    if (tipo == 0) {
        e.c = 'C';  // Cliente general
    } else if (tipo == 1) {
        e.c = 'U';  // Usuario
    } else {
        e.c = 'P';  // Preferente
    }

    // Si la fila esta vacia, imprimir en la posicion base centrada
    if (Empty(fila)) {
        MoverCursor(columnaBase + centramiento, filaPantalla);
        if (numeroCliente < 10) {
            printf("< %c%d >", e.c, e.n);
        } else {
            printf("<%c%d >", e.c, e.n);
        }
    } else {
        tamActual = Size(fila);
        // Si hay menos de 3 en fila (caben 3 filas dentro del recuadro)
        if (tamActual < 3) {
            MoverCursor(columnaBase + centramiento, filaPantalla + tamActual);
            if (numeroCliente < 10) {
                printf("< %c%d >", e.c, e.n);
            } else {
                printf("<%c%d >", e.c, e.n);
            }
        } else {
            // Si 3 o mas, mostrar conteo excedente centrado
            tamActual -= 2;
            MoverCursor(columnaBase + centramiento, filaPantalla + 3);
            if (tamActual < 10) {
                printf("[+%d]", tamActual);
            } else {
                printf("[%2d]", tamActual);
            }
        }
    }
    EsperarMiliSeg(20);
    Queue(fila, e);
}


/*
    elemento QuitarClienteDeFila(cola *fila, int tipo);
    Recibe: cola * Referencia a la cola de la fila,
            int Tipo de cliente (0: generales, 1: usuarios, 2: preferentes).
    Devuelve: elemento El cliente que se retira de la fila.
    Observaciones: Funcion corregida para usar las nuevas posiciones y limpiar 
                   completamente los caracteres residuales incluyendo el ">".
*/
elemento QuitarClienteDeFila(cola *fila, int tipo)
{
    int filaPantalla = 15;  // Ajuste vertical: justo dentro del recuadro de fila
    int baseX = 12;         // Nueva base horizontal ajustada
    int espacio = 28;       // Nueva separación entre filas
    int columnaBase = baseX + (espacio * tipo);  // Base horizontal para cada fila
    int centramiento = 3;  // Offset para centrar dentro del recuadro de 9 columnas
    int tam, i;
    elemento e, aux;

    // Dequeue del primer cliente
    e = Dequeue(fila);

    // Limpiar completamente el área (incluyendo caracteres residuales como ">")
    for (i = 0; i < 4; i++) {
        MoverCursor(columnaBase, filaPantalla + i);
        printf("         ");  // Limpiar 9 espacios completos dentro del recuadro
    }

    // Borrar y reimprimir fila restante (hasta 3 lineas dentro del recuadro)
    if (!Empty(fila)) {
        tam = Size(fila);
        for (i = 0; i < tam && i < 3; i++) {
            aux = Element(fila, i + 1);
            MoverCursor(columnaBase + centramiento, filaPantalla + i);
            if (aux.n < 10) {
                printf("< %c%d >", aux.c, aux.n);
            } else {
                printf("<%c%d >", aux.c, aux.n);
            }
        }
        if (tam > 3) {
            int excedente = tam - 3;
            MoverCursor(columnaBase + centramiento, filaPantalla + 3);
            if (excedente < 10) {
                printf("[+%d]", excedente);
            } else {
                printf("[%2d]", excedente);
            }
        }
    }
    EsperarMiliSeg(10);
    return e;
}

/*
    void AbrirBanco(int cajeros, boolean cajasAbiertas[]);
    Recibe: int Numero de cajeros,
            boolean [] Arreglo indicando que cajeros estan abiertos.
    Devuelve: void
    Observaciones: Funcion que dibuja la animacion de apertura del banco:
                   presentacion, marco, cajeros, filas, decoracion y anuncio de apertura.
*/
void AbrirBanco(int cajeros, boolean cajasAbiertas[])
{
    DibujaPresentacion();
    BorrarPantalla();
    DibujaMarco();
    DibujaCajas(cajeros, cajasAbiertas);
    DibujaFilas();
    DibujaDecoracion();
    DibujaAnuncioAbrir();
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
	printf("Banco Plus");

	EsperarMiliSeg(5000);
	BorrarPantalla();
}

/*
    void DibujaMarco();
    Recibe: void
    Devuelve: void
    Observaciones: Funcion que dibuja el marco principal del banco con un estilo ASCII limpio,
                   incluyendo bordes simples y cabeceras de informacion. Marco ajustado para
                   acomodar todas las ventanillas dentro del area visible.
*/
void DibujaMarco()
{
    int columna, fila;
    int Esperar = 5;

    // Bordes horizontales superior e inferior
    for (columna = 0; columna < ANCHO; columna++) {
        MoverCursor(columna, 0);
        printf("-");
        MoverCursor(columna, ALTO - 1);
        printf("-");
        EsperarMiliSeg(Esperar);
    }
    // Bordes verticales laterales
    for (fila = 1; fila < ALTO - 1; fila++) {
        MoverCursor(0, fila);
        printf("|");
        MoverCursor(ANCHO - 1, fila);
        printf("|");
        EsperarMiliSeg(Esperar);
    }

    // Cabecera del banco
    MoverCursor(3, 1);
    printf("[BANCO PLUS - Simulacion ]");
    // Contadores iniciales
    //MoverCursor(3, 2);
    //printf("Clientes Ati. :   0");
    //MoverCursor(35, 2);
    //printf("Ultimo Mov. :  ---");
}

/*
    void DibujaCajas(int cajeros, boolean cajasAbiertas[]);
    Recibe: int Numero de cajeros (<=10),
            boolean [] Arreglo indicando que cajeros estan abiertos.
    Devuelve: void
    Observaciones: Funcion que dibuja las cajas (ventanillas) del banco en disposicion
                   horizontal en la parte media de la pantalla, con un estilo simetrico.
*/
void DibujaCajas(int cajeros, boolean cajasAbiertas[])
{
    int filaY =  6;       // Fila donde empiezan las ventanillas
    int anchoCajero =  9; // Cada cajero ocupa 9 columnas
    int baseX =  4;       // Punto de partida horizontal
    int contador =  1;
    int Esperar = 5;
    int dibujadas = 0;

    for (int idx = 0; idx < 10; idx++) {
        int posX = baseX + idx * anchoCajero;
        // Si la ventanilla esta abierta y aun no hemos colocado todas
        if (cajasAbiertas[idx] && dibujadas < cajeros) {
            // Dibujar recuadro simetrico de 5 filas x 9 columnas
            MoverCursor(posX,     filaY);
            printf("+-------+");  // Techo (7 guiones)
            MoverCursor(posX,     filaY + 1);
            printf("|  V%2d |", contador); // Identificador Vxx centrado
            MoverCursor(posX,     filaY + 2);
            printf("|  ===  |");  // Ventana abierta
            MoverCursor(posX,     filaY + 3);
            printf("|  ---  |");  // Base decorativa
            MoverCursor(posX,     filaY + 4);
            printf("+-------+");  // Suelo
            contador++;
            dibujadas++;
        } else {
            // Ventanilla cerrada (indicador claro)
            MoverCursor(posX,     filaY);
            printf("+-------+");
            MoverCursor(posX,     filaY + 1);
            printf("| CERR  |");
            MoverCursor(posX,     filaY + 2);
            printf("|  XXX  |");
            MoverCursor(posX,     filaY + 3);
            // Terminación de la función DibujaCajas (continuación desde donde se cortó)
            printf("|  XXX  |");
            MoverCursor(posX,     filaY + 4);
            printf("+-------+");
        }
        EsperarMiliSeg(Esperar);
    }
}

/*
    void DibujaFilas();
    Recibe: void
    Devuelve: void
    Observaciones: Funcion que dibuja las tres filas de espera centradas dentro de sus recuadros,
                   ubicadas en la parte inferior de la pantalla. Ajustada para el nuevo ancho (95).
*/
void DibujaFilas()
{
    int alturaCaja =  6 + 5;   // 6 (empieza filaY) + 5 (alto del recuadro ventanilla) = 11
    int inicioY    =  alturaCaja + 1; // Primera fila de espera
    int anchoFila  =  11;      // Cada recuadro mide 11 columnas
    int espacio    = 28;       // Separacion horizontal entre recuadros (ajustada para centrar)
    int baseX      = 12;       // Posición inicial ajustada para centrar en el nuevo ancho
    int Esperar    = 5;

    // Label de ColA: General (por encima del recuadro)
    MoverCursor(baseX + 2, inicioY - 1);
    printf("ColA: General");

    // Dibujo del recuadro (sin borde inferior) para ColA
    for (int col = baseX; col < baseX + anchoFila; col++) {
        MoverCursor(col, inicioY);
        printf("-");
    }
    for (int fil = inicioY + 1; fil < inicioY + 1 + 3; fil++) {
        MoverCursor(baseX, fil);
        printf("|");
        MoverCursor(baseX + anchoFila - 1, fil);
        printf("|");
        EsperarMiliSeg(Esperar);
    }

    // Label de ColB: Usuario
    MoverCursor(baseX + espacio + 2, inicioY - 1);
    printf("ColB: Usuario");

    // Dibujo del recuadro para ColB
    for (int col = baseX + espacio; col < baseX + espacio + anchoFila; col++) {
        MoverCursor(col, inicioY);
        printf("-");
    }
    for (int fil = inicioY + 1; fil < inicioY + 1 + 3; fil++) {
        MoverCursor(baseX + espacio, fil);
        printf("|");
        MoverCursor(baseX + espacio + anchoFila - 1, fil);
        printf("|");
        EsperarMiliSeg(Esperar);
    }

    // Label de ColC: Preferente
    MoverCursor(baseX + 2 * espacio + 2, inicioY - 1);
    printf("ColC: Preferente");

    // Dibujo del recuadro para ColC
    for (int col = baseX + 2 * espacio; col < baseX + 2 * espacio + anchoFila; col++) {
        MoverCursor(col, inicioY);
        printf("-");
    }
    for (int fil = inicioY + 1; fil < inicioY + 1 + 3; fil++) {
        MoverCursor(baseX + 2 * espacio, fil);
        printf("|");
        MoverCursor(baseX + 2 * espacio + anchoFila - 1, fil);
        printf("|");
        EsperarMiliSeg(Esperar);
    }
}

/*
    void DibujaDecoracion();
    Recibe: void
    Devuelve: void
    Observaciones: Funcion que dibuja elementos decorativos adicionales para el banco,
                   usando ASCII art sencillo. Ajustada para el nuevo ancho del marco.
*/
void DibujaDecoracion()
{
    int Esperar = 3;
    // Crear una linea punteada horizontal como separador
    for (int x = 2; x < ANCHO - 2; x++) {
        MoverCursor(x, 12);
        printf(".");
        EsperarMiliSeg(Esperar);
    }

    // Logo de coins en esquinas inferiores
    MoverCursor(2, ALTO - 4);
    printf("($$$)");
    MoverCursor(ANCHO - 7, ALTO - 4);
    printf("($$$)");

    // Texto de pie de pantalla (ajustado al nuevo ancho)
    MoverCursor(3, ALTO - 2);
    printf("Banco Plus -   2025 GUIMO Company   |   Ctrl+C para salir");
}

/*
    void DibujaAnuncioAbrir();
    Recibe: void
    Devuelve: void
    Observaciones: Funcion que muestra una cuenta regresiva en ASCII y el anuncio
                   de inicio con una animacion sencilla.
*/
void DibujaAnuncioAbrir()
{
    int centroX = ANCHO / 2 - 3;
    int centroY = ALTO / 2;
    int Esperar = 600;

    // Contador regresivo 3..1
    for (int i = 3; i > 0; i--) {
        MoverCursor(centroX, centroY);
        printf("  %d  ", i);
        EsperarMiliSeg(Esperar);

        // Borrar numero
        MoverCursor(centroX, centroY);
        printf("     ");
        EsperarMiliSeg(200);
    }

    // Mostrar "INICIO"
    MoverCursor(centroX - 1, centroY);
    printf(" INICIO ");
    EsperarMiliSeg(Esperar);

    // Borrar anuncio
    MoverCursor(centroX - 1, centroY);
    printf("        ");
    EsperarMiliSeg(200);
}
