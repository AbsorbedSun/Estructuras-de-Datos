#include <stdio.h>
#include <windows.h>
#include <string.h>

// CONSTANTES
#define ALTO 24 // Se piensa en un pantalla de 24 filas x 79 columnas
#define ANCHO 79
#define TIEMPO_BASE 10 // Tiempo base en milisegundos

// Dimensiones del libro ajustadas a la pantalla
#define ANCHO_PAGINA 37 // Cada página ocupa 37 caracteres
#define SEPARACION 5    // Separación central del libro

// Colores para diferentes páginas
#define COLOR_PAGINA1 BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY // Blanco
#define COLOR_PAGINA2 BACKGROUND_BLUE | BACKGROUND_INTENSITY                                     // Azul
#define COLOR_PAGINA3 BACKGROUND_GREEN | BACKGROUND_INTENSITY                                    // Verde
#define COLOR_PAGINA4 BACKGROUND_RED | BACKGROUND_INTENSITY                                      // Rojo
#define COLOR_BORDE FOREGROUND_INTENSITY

// Arte ASCII inicial
const char *arteASCII[] = {
    "MMMWKOO0XNWMMMMMMMMMMMMMMMMMMWX0OOKWMMMM",
    "MMMXd;;;;;coONMMMMMMMMMMMN0dc;;,;;oXMMMM",
    "MMMMNXXK0xo;.,xNMMMMMMMNk;.;lxOKXXWMMMMM",
    "MMMMMMMMMMMWO;.:O00K0K0c.,kNMMMMMMMMMMMM",
    "MMMMMMMMMMMMNo. ...... .oXMMMMMMMMMMMMM",
    "MMMMMMMMMMMMKl.         .cKMMMMMMMMMMMMM",
    "MMMMMMMMNOdxKW0,       'kWXxokNMMMMMMMMM",
    "MMMMMMMNo.  ,OWk.     .xW0;  lNMMMMMMMM",
    "MMMMMMM0'    lWK,     .OWo   '0MMMMMMMM",
    "MMMMMMMK,    .dW0'   .kWx.   ,KMMMMMMMM",
    "MMMMMMMWO:.'oXNo     cNNd'.;kWMMMMMMMM",
    "MMMMMWWWMWXNWKc.     .:0WNXNMWWWMMMMMM",
    "MMMNk:,lKMMMWO;       ,kWMMMXo,:xXMMMM",
    "MNx,   ,0WMMMNOl;''',ckXMMMMK:   'dXMM",
    "K:   ...'kWMMMMMWNNNWMMMMMWO,...   ;0W",
    ",   .cOXd..xNMMMMMMMMMMMMMWk'.dX0l.  ,0",
    ";0WMMWO,.oNMMMMMMMMMMMNx.'kWMMWK:   .d",
    ".  ;KMMMMMM0;.lXMMMMMMMMMNo.,OWMMMMMXc .x",
    "kdKMMMMMMMMK:.:KMMMMMMMXl.;0MMMMMMMMKdxN",
    "MMMMMMMMMMMMXl.;0MMMMMXc.cKMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMNklOWMMMMKoxNMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "",
    ""};

// Mueve el cursor a una posición específica
void moverCursor(HANDLE h, int fila, int col)
{
    COORD pos = {(SHORT)col, (SHORT)fila};
    SetConsoleCursorPosition(h, pos);
}

// Limpia la pantalla
void limpiarPantalla(HANDLE h)
{
    system("cls");
}

// Muestra el arte ASCII inicial
void mostrarArteASCII(HANDLE h)
{
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    for (int i = 0; i < ALTO && i < 24; i++)
    {
        moverCursor(h, i, 0);
        if (strlen(arteASCII[i]) > 0)
        {
            // Centrar el arte si es más corto que el ancho de pantalla
            int padding = (ANCHO - strlen(arteASCII[i])) / 2;
            if (padding > 0)
            {
                for (int j = 0; j < padding; j++)
                    printf(" ");
            }
            printf("%.79s", arteASCII[i]);
        }
    }
}

// Animación de transición por ondas para pasar del ASCII al libro
void animacionOndas(HANDLE h, WORD colorDestino, char caracterDestino)
{
    DWORD escrito;

    // Transición por ondas desde el centro hacia afuera
    int centroX = ANCHO / 2;
    int centroY = ALTO / 2;

    for (int radio = 0; radio <= centroX + centroY; radio += 1)
    {
        for (int y = 0; y < ALTO; y++)
        {
            for (int x = 0; x < ANCHO; x++)
            {
                int distancia = abs(x - centroX) + abs(y - centroY); // Distancia Manhattan

                if (distancia <= radio)
                {
                    moverCursor(h, y, x);
                    SetConsoleTextAttribute(h, colorDestino | FOREGROUND_INTENSITY);
                    WriteConsoleA(h, &caracterDestino, 1, &escrito, NULL);
                }
            }
        }
        Sleep(TIEMPO_BASE * 2);
    }
}

// Dibuja el marco del libro adaptado a 79x24
void dibujarMarcoLibro(HANDLE h)
{
    SetConsoleTextAttribute(h, COLOR_BORDE);

    // Línea superior
    moverCursor(h, 0, 1);
    for (int i = 0; i < ANCHO_PAGINA; i++)
        printf("-");
    printf(" __");
    for (int i = 0; i < ANCHO_PAGINA; i++)
        printf("-");

    // Líneas laterales y centrales
    for (int i = 1; i < ALTO - 1; i++)
    {
        moverCursor(h, i, 0);
        printf("||");
        moverCursor(h, i, ANCHO_PAGINA + 1);
        printf("|  |");
        moverCursor(h, i, ANCHO_PAGINA + SEPARACION);
        printf("||");
    }

    // Línea inferior
    moverCursor(h, ALTO - 1, 1);
    for (int i = 0; i < ANCHO_PAGINA; i++)
        printf("-");
    printf("");
    for (int i = 0; i < ANCHO_PAGINA; i++)
        printf("-");
}

// Rellena una página con un color y carácter específicos
void rellenarPagina(HANDLE h, int lado, WORD color, char caracter)
{
    int colInicio = (lado == 0) ? 2 : (ANCHO_PAGINA + SEPARACION + 1);
    DWORD escrito;

    SetConsoleTextAttribute(h, color | FOREGROUND_INTENSITY);

    for (int i = 1; i < ALTO - 1; i++)
    {
        moverCursor(h, i, colInicio);
        for (int j = 0; j < ANCHO_PAGINA - 1; j++)
        {
            WriteConsoleA(h, &caracter, 1, &escrito, NULL);
        }
    }
}

// Animación de pasar página (de derecha a izquierda)
void animarPasarPagina(HANDLE h, WORD colorAnterior, char carAnterior, WORD colorNuevo, char carNuevo)
{
    int colDerechaPagina = ANCHO_PAGINA + SEPARACION + 1;
    DWORD escrito;

    // Animación: la página derecha se "dobla" hacia la izquierda
    for (int paso = 0; paso <= ANCHO_PAGINA; paso += 1)
    {
        // Redibujar marco
        dibujarMarcoLibro(h);

        // Página izquierda (se mantiene con el color anterior)
        rellenarPagina(h, 0, colorAnterior, carAnterior);

        // Efecto de página doblándose en la página derecha
        for (int i = 1; i < ALTO - 1; i++)
        {
            moverCursor(h, i, colDerechaPagina);

            for (int j = 0; j < ANCHO_PAGINA - 1; j++)
            {
                if (j < paso)
                {
                    // Parte de la nueva página que ya es visible
                    SetConsoleTextAttribute(h, colorNuevo | FOREGROUND_INTENSITY);
                    WriteConsoleA(h, &carNuevo, 1, &escrito, NULL);
                }
                else if (j == paso && paso < ANCHO_PAGINA - 1)
                {
                    // Línea de "doblez"
                    SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
                    WriteConsoleA(h, "|", 1, &escrito, NULL);
                }
                else
                {
                    // Resto de la página anterior
                    SetConsoleTextAttribute(h, colorAnterior | FOREGROUND_INTENSITY);
                    WriteConsoleA(h, &carAnterior, 1, &escrito, NULL);
                }
            }
        }

        Sleep(TIEMPO_BASE);
    }
}

void anima()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Configurar cursor invisible
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(h, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(h, &cursorInfo);

    // Colores y caracteres para las páginas
    WORD colores[] = {COLOR_PAGINA1, COLOR_PAGINA2, COLOR_PAGINA3, COLOR_PAGINA4};
    char caracteres[] = {'1', '2', '3', ' '};

    // 1. Mostrar arte ASCII inicial por 5 segundos
    limpiarPantalla(h);
    mostrarArteASCII(h);
    Sleep(5000); // 5 segundos

    // 2. Transición del ASCII al libro con animación de ondas
    animacionOndas(h, COLOR_BORDE, ' ');
    Sleep(500);

    // 3. Mostrar libro inicial
    limpiarPantalla(h);
    dibujarMarcoLibro(h);

    // Página inicial (izquierda vacía, derecha con primer contenido)
    rellenarPagina(h, 0, COLOR_PAGINA1, ' ');        // Página izquierda vacía
    rellenarPagina(h, 1, colores[0], caracteres[0]); // Página derecha con contenido

    Sleep(1000);

    // 4. Hacer solo 2 cambios de página
    for (int i = 0; i < 2; i++)
    {
        animarPasarPagina(h, colores[i], caracteres[i], colores[i + 1], caracteres[i + 1]);
        Sleep(800);
    }

    // 5. Restaurar cursor y limpiar para programa normal
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(h, &cursorInfo);

    // Limpiar pantalla y restaurar color original
    limpiarPantalla(h);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // La consola está lista para un programa normal
    printf("Consola lista para usar...\n");
}

void libro()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Configurar cursor invisible
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(h, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(h, &cursorInfo);

    // Colores y caracteres para las páginas
    WORD colores[] = {COLOR_PAGINA1, COLOR_PAGINA2, COLOR_PAGINA3, COLOR_PAGINA4};
    char caracteres[] = {'1', '2', '3', ' '};

    // 3. Mostrar libro inicial
    limpiarPantalla(h);
    dibujarMarcoLibro(h);

    // Página inicial (izquierda vacía, derecha con primer contenido)
    rellenarPagina(h, 0, COLOR_PAGINA1, ' ');        // Página izquierda vacía
    rellenarPagina(h, 1, colores[0], caracteres[0]); // Página derecha con contenido

    Sleep(1000);

    // 4. Hacer solo 2 cambios de página
    for (int i = 0; i < 2; i++)
    {
        animarPasarPagina(h, colores[i], caracteres[i], colores[i + 1], caracteres[i + 1]);
        Sleep(800);
    }

    // 5. Restaurar cursor y limpiar para programa normal
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(h, &cursorInfo);

    // Limpiar pantalla y restaurar color original
    limpiarPantalla(h);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // La consola está lista para un programa normal
    printf("Consola lista para usar...\n");
}