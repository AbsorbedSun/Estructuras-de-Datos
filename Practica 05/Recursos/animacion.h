#include <stdio.h>
#include <windows.h>
#include <string.h>

void moverCursor(HANDLE h, int fila, int col);

void limpiarPantalla(HANDLE h);

void mostrarArteASCII(HANDLE h);

void animacionOndas(HANDLE h, WORD colorDestino, char caracterDestino);

void dibujarMarcoLibro(HANDLE h);

void rellenarPagina(HANDLE h, int lado, WORD color, char caracter);

void animarPasarPagina(HANDLE h, WORD colorAnterior, char carAnterior, WORD colorNuevo, char carNuevo);

void anima();

void libro();
