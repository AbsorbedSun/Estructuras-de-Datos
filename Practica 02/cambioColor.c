/*
  cambioColor.c
  Practica 02: Backtracking en imagenes
  Autor: Garcia Ambrosio Aldo (C) Abril 2025
         Ponce Espino Miguel Angel (C) Abril 2025
         Salinas Aquino Omar Ivan (C) Abril 2025
  Grupo: 2CM6
  Autor original: Edgardo Adrián Franco Martínez
  Uso de IA (GPT o4, Claude)
  Version: 2.7

  Programa que recibe una imagen en formato BMP de 24 bits, un pixel dado de dicha imagen y un color
  a través del estándar RGB con el objetivo de cambiar el color de ese pixel y todos los adyacentes
  que tengan el mismo color a un nuevo color (similar a la función "relleno" en programas de dibujo).

  Observaciones: Este programa utiliza un algoritmo de backtracking para recorrer la imagen y cambiar
  los colores de los píxeles adyacentes de manera recursiva.

  Compilación: Windows / Linux: gcc cambioColor.c -o cambioColor

Ejecución: ./cambioColor {nombre}.bmp (x,y) (R_nuevo,G_nuevo,B_nuevo) tolerancia
Ejecución(zsh): ./cambioColor {nombre}.bmp "(x,y)" "(R_nuevo,G_nuevo,B_nuevo)"" tolerancia

Ejemplo: ./cambioColor imagen.bmp (100,150) (255,0,0) 30

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IMAGEN_TRATADA "tratada.bmp" // Nombre del archivo de salida con la imagen modificada

// Estructura para representar una imagen BMP
typedef struct BMP
{
    // Cabecera de archivo BMP (14 bytes)
    char bm[2];    // (2 Bytes) BM (Tipo de archivo)
    int tamano;    // (4 Bytes) Tamaño del archivo en bytes
    int reservado; // (4 Bytes) Reservado
    int offset;    // (4 Bytes) Offset, distancia en bytes entre la cabecera y los píxeles

    // Cabecera de información DIB (40 bytes)
    int tamanoMetadatos;        // (4 Bytes) Tamaño de Metadatos (tamaño de esta estructura = 40)
    int alto;                   // (4 Bytes) Alto (número de pixeles verticales)
    int ancho;                  // (4 Bytes) Ancho (número de píxeles horizontales)
    short int numeroPlanos;     // (2 Bytes) Número de planos de color
    short int profundidadColor; // (2 Bytes) Profundidad de color (debe ser 24 para nuestro caso)
    int tipoCompresion;         // (4 Bytes) Tipo de compresión (Vale 0, ya que el bmp es descomprimido)
    int tamanoEstructura;       // (4 Bytes) Tamaño de la estructura Imagen (Paleta)
    int pxmh;                   // (4 Bytes) Píxeles por metro horizontal
    int pxmv;                   // (4 Bytes) Píxeles por metro vertical
    int coloresUsados;          // (4 Bytes) Cantidad de colores usados
    int coloresImportantes;     // (4 Bytes) Cantidad de colores importantes

    // Campos adicionales para el manejo de la imagen
    int padding;            // Bytes necesarios para que cada fila sea múltiplo de 4 Bytes
    unsigned char **pixelR; // Puntero a los píxeles del plano R (Rojo)
    unsigned char **pixelG; // Puntero a los píxeles del plano G (Verde)
    unsigned char **pixelB; // Puntero a los píxeles del plano B (Azul)
} BMP;

// Prototipos de funciones
void abrir_imagen(BMP *imagen, char ruta[]);
void crear_imagen(BMP *imagen, char ruta[]);
void cambioColor(BMP *mapa, char **mapaR, int x, int y,
                 unsigned char r_actual, unsigned char g_actual, unsigned char b_actual,
                 unsigned char r_nuevo, unsigned char g_nuevo, unsigned char b_nuevo,
                 int *contador, int tolerancia);
int colores_similares(unsigned char r1, unsigned char g1, unsigned char b1,
                      unsigned char r2, unsigned char g2, unsigned char b2,
                      int tolerancia);

/*
int main(int argc, char* argv[])
Recibe: int argc como el número total de argumentos ingresados al programa y char* argv[]
como un arreglo que almacena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite cargar una imagen BMP, cambiar el color de un
área conectada (tipo "relleno") y guardar el resultado en una nueva imagen. Además, muestra
estadísticas sobre los píxeles modificados y el tiempo de ejecución.
*/
int main(int argc, char *argv[])
{
    // Variables para la medición de tiempos de ejecución
    clock_t t_inicio, t_final;
    double t_intervalo;

    // Variables auxiliares
    int i, j, x, y, cont, contador, tolerancia;
    cont = 0, contador = 0;

    // Variables de la imagen BMP
    BMP img;
    unsigned char r_actual, g_actual, b_actual;
    unsigned char r_nuevo, g_nuevo, b_nuevo;
    char IMAGEN[45], COORDENADAS[20], COLOR[20];

    // Verificar que se hayan proporcionado los argumentos necesarios
    if (argc != 5)
    {
        printf("\nIndique el nombre del archivo, las coordenadas del color a cambiar, el color en estándar RGB y la tolerancia de color");
        printf("\nEjemplo: [user@equipo]$ %s imagen.bmp (x,y) (000,000,000) 30\n", argv[0]);
        exit(1);
    }

    // Procesar los argumentos recibidos
    strcpy(IMAGEN, argv[1]);
    strcpy(COORDENADAS, argv[2]);
    strcpy(COLOR, argv[3]);
    tolerancia = atoi(argv[4]);

    sscanf(COORDENADAS, "(%d,%d)", &x, &y);
    sscanf(COLOR, "(%hhu,%hhu,%hhu)", &r_nuevo, &g_nuevo, &b_nuevo);

    // Abrir la imagen BMP de 24 bits
    abrir_imagen(&img, IMAGEN);

    // Mostrar información de la imagen cargada
    printf("\n*************************************************************************");
    fflush(stdout);
    printf("\nImagen: %s", IMAGEN);
    fflush(stdout);
    printf("\nDimensiones de la imagen:\tAncho=%d\tAlto=%d\n", img.ancho, img.alto);
    fflush(stdout);

    // Obtener el color actual del píxel seleccionado
    r_actual = img.pixelR[x][y];
    g_actual = img.pixelG[x][y];
    b_actual = img.pixelB[x][y];

    // Crear mapa de recorrido para marcar píxeles ya procesados
    char **mapa = malloc(img.ancho * sizeof(char *));
    for (i = 0; i < img.ancho; i++)
    {
        mapa[i] = malloc(img.alto * sizeof(char));
    }
    // Inicializar el mapa con ceros (píxeles no visitados)
    for (i = 0; i < img.ancho; i++)
        for (j = 0; j < img.alto; j++)
            mapa[i][j] = 0;

    // Iniciar el algoritmo de cambio de color y medir el tiempo
    t_inicio = clock();
    cambioColor(&img, mapa, x, y, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, &contador, tolerancia);
    t_final = clock();

    // Contar los píxeles modificados
    for (i = 0; i < img.ancho; i++)
        for (j = 0; j < img.alto; j++)
            if (mapa[i][j])
                cont++;

    // Liberar memoria del mapa de recorrido
    for (i = 0; i < img.ancho; i++)
        free(mapa[i]);
    free(mapa);

    // Crear la imagen BMP tratada y mostrar estadísticas
    crear_imagen(&img, IMAGEN_TRATADA);
    printf("\nImagen BMP tratada guardada en el archivo: %s\n", IMAGEN_TRATADA);
    fflush(stdout);
    printf("\nPixeles modificados:\t%d\n", cont);
    fflush(stdout);
    t_intervalo = (double)(t_final - t_inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo medido:\t%.8f segundos.\n", t_intervalo);
    fflush(stdout);
    printf("\nRecursiones hechas:\t%d\n", contador);
    fflush(stdout);
    printf("*************************************************************************\n");
    fflush(stdout);

    exit(0);
}

/*
void abrir_imagen(BMP *imagen, char ruta[])
Recibe: BMP *imagen como puntero a una estructura BMP donde se cargará la imagen y
char ruta[] como la ruta del archivo BMP a abrir.
Devuelve: void (No retorna valor explícito)
Observaciones: Función que abre un archivo BMP de 24 bits, lee su cabecera y los datos de
los píxeles, y los almacena en la estructura BMP proporcionada. La función verifica que el
archivo sea un bitmap válido y que tenga la profundidad de color adecuada (24 bits).
También calcula el padding necesario para cada fila de píxeles.
*/
void abrir_imagen(BMP *imagen, char *ruta){
    FILE *archivo;
    int i, j, k;
    unsigned char R, G, B, var;

    // Abrir archivo en modo binario para lectura
    archivo = fopen(ruta, "rb+");
    if (!archivo){
        printf("La imagen %s no se encontró\n", ruta);
        exit(1);
    }

    // Leer cabecera del archivo BMP
    fseek(archivo, 0, SEEK_SET);
    fread(&imagen->bm, sizeof(char), 2, archivo);
    fread(&imagen->tamano, sizeof(int), 1, archivo);
    fread(&imagen->reservado, sizeof(int), 1, archivo);
    fread(&imagen->offset, sizeof(int), 1, archivo);
    fread(&imagen->tamanoMetadatos, sizeof(int), 1, archivo);
    fread(&imagen->ancho, sizeof(int), 1, archivo);
    fread(&imagen->alto, sizeof(int), 1, archivo);
    fread(&imagen->numeroPlanos, sizeof(short int), 1, archivo);
    fread(&imagen->profundidadColor, sizeof(short int), 1, archivo);
    fread(&imagen->tipoCompresion, sizeof(int), 1, archivo);
    fread(&imagen->tamanoEstructura, sizeof(int), 1, archivo);
    fread(&imagen->pxmh, sizeof(int), 1, archivo);
    fread(&imagen->pxmv, sizeof(int), 1, archivo);
    fread(&imagen->coloresUsados, sizeof(int), 1, archivo);
    fread(&imagen->coloresImportantes, sizeof(int), 1, archivo);

    // Verificar formato BMP y profundidad de color
    if (imagen->bm[0] != 'B' || imagen->bm[1] != 'M'){
        printf("La imagen debe ser un bitmap.\n");
        exit(1);
    }
    if (imagen->profundidadColor != 24){
        printf("La imagen debe ser de 24 bits.\n");
        exit(1);
    }

    // Calcular el padding (relleno) para que cada fila sea múltiplo de 4 bytes
    imagen->padding = (4 - (imagen->ancho * 3) % 4) % 4;
    printf("Padding: %d\n", imagen->padding);

    // Asignar memoria para las matrices de componentes RGB
    imagen->pixelR = malloc(imagen->ancho * sizeof(unsigned char *));
    imagen->pixelG = malloc(imagen->ancho * sizeof(unsigned char *));
    imagen->pixelB = malloc(imagen->ancho * sizeof(unsigned char *));

    for (i = 0; i < imagen->ancho; i++){
        imagen->pixelR[i] = malloc(imagen->alto * sizeof(unsigned char));
        imagen->pixelG[i] = malloc(imagen->alto * sizeof(unsigned char));
        imagen->pixelB[i] = malloc(imagen->alto * sizeof(unsigned char));
    }

    // Posicionar el puntero al inicio de los datos de píxeles
    fseek(archivo, imagen->offset, SEEK_SET);

    // Leer los datos de píxeles (formato BMP almacena de abajo hacia arriba)
    for (j = imagen->alto - 1; j >= 0; j--){
        for (i = 0; i < imagen->ancho; i++){
            fread(&B, sizeof(char), 1, archivo); // Byte Blue del píxel
            fread(&G, sizeof(char), 1, archivo); // Byte Green del píxel
            fread(&R, sizeof(char), 1, archivo); // Byte Red del píxel
            imagen->pixelR[i][j] = R;
            imagen->pixelG[i][j] = G;
            imagen->pixelB[i][j] = B;
        }

        // Saltar los bytes de padding al final de cada fila
        for (k = 1; k <= imagen->padding; k++){
            fread(&var, sizeof(char), 1, archivo);
        }
    }

    fclose(archivo);
}

/*
void cambioColor(BMP *mapa, char **mapaR, int x, int y,
                unsigned char r_actual, unsigned char g_actual, unsigned char b_actual,
                unsigned char r_nuevo, unsigned char g_nuevo, unsigned char b_nuevo, int * contador, int tolerancia)
Recibe: BMP *mapa como puntero a la estructura BMP que contiene la imagen, char **mapaR como matriz
para registrar píxeles ya procesados, int x e int y como coordenadas del píxel actual,
componentes RGB del color actual y nuevo a aplicar,
y valor entero de tolerancia que define el rango permitido de diferencia entre colores.
Devuelve: void (No retorna valor explícito)
Observaciones: Función recursiva que implementa un algoritmo de backtracking para cambiar el color
de un área conectada de píxeles del mismo color.
*/
void cambioColor(BMP *mapa, char **mapaR, int x, int y,
                 unsigned char r_actual, unsigned char g_actual, unsigned char b_actual,
                 unsigned char r_nuevo, unsigned char g_nuevo, unsigned char b_nuevo,
                 int *contador, int tolerancia){
    // Apuntador a entero que cuenta el numero de recurciones
    (*contador)++;

    // Comprobar que las coordenadas estén dentro de los límites de la imagen
    if (x < 0 || x >= mapa->ancho || y < 0 || y >= mapa->alto){
        return; // Fuera de límites
    }

    // Verificar si el píxel ya fue procesado o no es similar al color base
    if (!colores_similares(mapa->pixelR[x][y], mapa->pixelG[x][y], mapa->pixelB[x][y],
                           r_actual, g_actual, b_actual, tolerancia) ||
        mapaR[x][y] == 1){           // Si ya fue procesado
        return; // Color diferente o ya procesado
    }

    // Cambiar el color del píxel y marcar como procesado
    mapa->pixelR[x][y] = r_nuevo;
    mapa->pixelG[x][y] = g_nuevo;
    mapa->pixelB[x][y] = b_nuevo;
    mapaR[x][y] = 1; // Marcar como procesado

    // Variables para marcar límites del área a explorar en cada dirección
    int arb = -1;          // Límite superior
    int abj = mapa->alto;  // Límite inferior
    int drc = mapa->ancho; // Límite derecho
    int izq = -1;          // Límite izquierdo
    int i, j;

    // =============================================
    // FASE 1: EXPLORACIÓN DE LÍNEAS EN 4 DIRECCIONES
    // =============================================

    // --- Explorar hacia arriba hasta encontrar un límite ---
    for (j = y - 1; j >= 0; j--){
        if (!colores_similares(mapa->pixelR[x][j], mapa->pixelG[x][j], mapa->pixelB[x][j],
                               r_actual, g_actual, b_actual, tolerancia) ||
            mapaR[x][j] == 1){
            arb = j; // Encontró un límite (píxel diferente o ya procesado)
            break;
        }
        // Cambiar el color del píxel y marcar como procesado
        mapa->pixelR[x][j] = r_nuevo;
        mapa->pixelG[x][j] = g_nuevo;
        mapa->pixelB[x][j] = b_nuevo;
        mapaR[x][j] = 1;
    }

    // --- Explorar hacia la derecha ---
    cambioColor(mapa, mapaR, x + 1, y, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia);
    for (i = x + 1; i < mapa->ancho; i++){
        if (!colores_similares(mapa->pixelR[i][y], mapa->pixelG[i][y], mapa->pixelB[i][y],
                               r_actual, g_actual, b_actual, tolerancia) ||
            mapaR[i][y] == 1){
            drc = i; // Encontró un límite
            break;
        }
        // Cambiar el color del píxel y marcar como procesado
        mapa->pixelR[i][y] = r_nuevo;
        mapa->pixelG[i][y] = g_nuevo;
        mapa->pixelB[i][y] = b_nuevo;
        mapaR[i][y] = 1;
    }

    // --- Explorar hacia abajo ---
    cambioColor(mapa, mapaR, x, y + 1, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia);
    for (j = y + 1; j < mapa->alto; j++){
        if (!colores_similares(mapa->pixelR[x][j], mapa->pixelG[x][j], mapa->pixelB[x][j],
                               r_actual, g_actual, b_actual, tolerancia) ||
            mapaR[x][j] == 1){
            abj = j; // Encontró un límite
            break;
        }
        // Cambiar el color del píxel y marcar como procesado
        mapa->pixelR[x][j] = r_nuevo;
        mapa->pixelG[x][j] = g_nuevo;
        mapa->pixelB[x][j] = b_nuevo;
        mapaR[x][j] = 1;
    }

    // --- Explorar hacia la izquierda ---
    cambioColor(mapa, mapaR, x - 1, y, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia);
    for (i = x - 1; i >= 0; i--){
        if (!colores_similares(mapa->pixelR[i][y], mapa->pixelG[i][y], mapa->pixelB[i][y],
                               r_actual, g_actual, b_actual, tolerancia) ||
            mapaR[i][y] == 1){
            izq = i; // Encontró un límite
            break;
        }
        // Cambiar el color del píxel y marcar como procesado
        mapa->pixelR[i][y] = r_nuevo;
        mapa->pixelG[i][y] = g_nuevo;
        mapa->pixelB[i][y] = b_nuevo;
        mapaR[i][y] = 1;
    }

    // Explorar el píxel por encima
    cambioColor(mapa, mapaR, x, y - 1, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia);

    // =============================================
    // FASE 2: RELLENAR ÁREAS ENTRE LOS LÍMITES ENCONTRADOS
    // =============================================

    // --- Procesar píxeles a los lados de la línea vertical hacia arriba ---
    for (j = y - 1; j > arb; j--){
        cambioColor(mapa, mapaR, x + 1, j, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel a la derecha
        cambioColor(mapa, mapaR, x - 1, j, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel a la izquierda
    }

    // --- Procesar píxeles arriba y abajo de la línea horizontal a la derecha ---
    for (i = x + 1; i < drc; i++){
        cambioColor(mapa, mapaR, i, y - 1, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel arriba
        cambioColor(mapa, mapaR, i, y + 1, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel abajo
    }

    // --- Procesar píxeles a los lados de la línea vertical hacia abajo ---
    for (j = y + 1; j < abj; j++){
        cambioColor(mapa, mapaR, x + 1, j, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel a la derecha
        cambioColor(mapa, mapaR, x - 1, j, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel a la izquierda
    }

    // --- Procesar píxeles arriba y abajo de la línea horizontal a la izquierda ---
    for (i = x - 1; i > izq; i--){
        cambioColor(mapa, mapaR, i, y - 1, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel arriba
        cambioColor(mapa, mapaR, i, y + 1, r_actual, g_actual, b_actual, r_nuevo, g_nuevo, b_nuevo, contador, tolerancia); // Píxel abajo
    }

    // printf("Recursion #%d\n", *contador);
    // fflush(stdout);
}

/*
void crear_imagen(BMP *imagen, char ruta[])
Recibe: BMP *imagen como puntero a la estructura BMP que contiene la imagen modificada y
char ruta[] como la ruta donde se guardará el nuevo archivo BMP.
Devuelve: void (No retorna valor explícito)
Observaciones: Función que crea un nuevo archivo BMP a partir de los datos almacenados en la
estructura BMP. Escribe la cabecera y los datos de los píxeles en el formato adecuado para BMP
(de abajo hacia arriba con padding). Al finalizar, libera la memoria reservada para las matrices
de componentes RGB.
*/
void crear_imagen(BMP *imagen, char ruta[]){
    FILE *archivo;
    int i, j, k;
    unsigned char var = 0x00; // Byte de relleno para el padding

    // Crear archivo en modo binario para escritura
    archivo = fopen(ruta, "wb+");
    if (!archivo){
        printf("La imagen %s no se pudo crear\n", ruta);
        exit(1);
    }

    // Escribir la cabecera del archivo BMP
    fseek(archivo, 0, SEEK_SET);
    fwrite(&imagen->bm, sizeof(char), 2, archivo);
    fwrite(&imagen->tamano, sizeof(int), 1, archivo);
    fwrite(&imagen->reservado, sizeof(int), 1, archivo);
    fwrite(&imagen->offset, sizeof(int), 1, archivo);
    fwrite(&imagen->tamanoMetadatos, sizeof(int), 1, archivo);
    fwrite(&imagen->ancho, sizeof(int), 1, archivo);
    fwrite(&imagen->alto, sizeof(int), 1, archivo);
    fwrite(&imagen->numeroPlanos, sizeof(short int), 1, archivo);
    fwrite(&imagen->profundidadColor, sizeof(short int), 1, archivo);
    fwrite(&imagen->tipoCompresion, sizeof(int), 1, archivo);
    fwrite(&imagen->tamanoEstructura, sizeof(int), 1, archivo);
    fwrite(&imagen->pxmh, sizeof(int), 1, archivo);
    fwrite(&imagen->pxmv, sizeof(int), 1, archivo);
    fwrite(&imagen->coloresUsados, sizeof(int), 1, archivo);
    fwrite(&imagen->coloresImportantes, sizeof(int), 1, archivo);

    // Posicionar el puntero al inicio de los datos de píxeles
    fseek(archivo, imagen->offset, SEEK_SET);

    // Escribir los datos de píxeles (de abajo hacia arriba, formato BMP)
    for (j = imagen->alto - 1; j >= 0; j--){
        for (i = 0; i < imagen->ancho; i++){
            fwrite(&imagen->pixelB[i][j], sizeof(char), 1, archivo); // Byte Blue del píxel
            fwrite(&imagen->pixelG[i][j], sizeof(char), 1, archivo); // Byte Green del píxel
            fwrite(&imagen->pixelR[i][j], sizeof(char), 1, archivo); // Byte Red del píxel
        }

        // Escribir bytes de padding al final de cada fila
        for (k = 1; k <= imagen->padding; k++)
            fwrite(&var, sizeof(char), 1, archivo); // Escribir bytes de relleno
    }

    // Liberar la memoria de las matrices de componentes RGB
    for (i = 0; i < imagen->ancho; i++){
        free(imagen->pixelR[i]);
        free(imagen->pixelG[i]);
        free(imagen->pixelB[i]);
    }
    free(imagen->pixelR);
    free(imagen->pixelG);
    free(imagen->pixelB);

    fclose(archivo);
}

/*
int colores_similares(unsigned char r1, unsigned char g1, unsigned char b1,
                      unsigned char r2, unsigned char g2, unsigned char b2,
                      int tolerancia)
Recibe: tres componentes RGB (r1, g1, b1) de un color de referencia, tres componentes RGB (r2, g2, b2) del color a comparar,
y un valor entero que representa la tolerancia máxima permitida por componente de color.
Devuelve: int (1 si los colores están dentro del rango de tolerancia, 0 en caso contrario)
Observaciones: Compara dos colores y determina si sus diferencias en cada componente (rojo, verde, azul)
están dentro de un rango aceptable especificado por el usuario. Esta función permite flexibilizar el cambio de color
para incluir tonalidades similares y no solo colores idénticos.
*/
int colores_similares(unsigned char r1, unsigned char g1, unsigned char b1,
                      unsigned char r2, unsigned char g2, unsigned char b2,
                      int tolerancia){
    return abs(r1 - r2) <= tolerancia &&
           abs(g1 - g2) <= tolerancia &&
           abs(b1 - b2) <= tolerancia;
}