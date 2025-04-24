/*  
  cambioColor.c
  Practica 02: Backtracking en imagenes
  Autor: Garcia Ambrosio Aldo (C) Abril 2025
         Ponce Espino Miguel Angel (C) Abril 2025
         Salinas Aquino Omar Ivan (C) Abril 2025
  Grupo: 2CM6
  Autor original: Edgardo Adrián Franco Martínez
  Uso de IA (GPT o4, Claude)
  Version: 1.9
  
  Programa que recibe una imagen en formato BMP de 24 bits, un pixel dado de dicha imagen y un color 
  a través del estándar RGB con el objetivo de cambiar el color de ese pixel y todos los adyacentes
  que tengan el mismo color a un nuevo color (similar a la función "relleno" en programas de dibujo).
  
  Observaciones: Este programa utiliza un algoritmo de backtracking para recorrer la imagen y cambiar
  los colores de los píxeles adyacentes de manera recursiva.
  
  Compilación: Windows / Linux: gcc cambioColor.c -o cambioColor

  Ejecución: ./cambioColor {nombre}.bmp (x,y) (R_nuevo,G_nuevo,B_nuevo)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IMAGEN_TRATADA "tratada.bmp"  // Nombre del archivo de salida con la imagen modificada

// Estructura para representar un color en formato RGB
typedef struct RGB
{
    unsigned char R, G, B;  // Valores para Rojo (0-255), Verde (0-255), Azul (0-255)
} RGB;

// Estructura para representar una imagen BMP
typedef struct BMP
{
    // Cabecera de archivo BMP (14 bytes)
    char bm[2];              // (2 Bytes) BM (Tipo de archivo)
    int tamano;              // (4 Bytes) Tamaño del archivo en bytes
    int reservado;           // (4 Bytes) Reservado
    int offset;              // (4 Bytes) Offset, distancia en bytes entre la cabecera y los píxeles
    
    // Cabecera de información DIB (40 bytes)
    int tamanoMetadatos;     // (4 Bytes) Tamaño de Metadatos (tamaño de esta estructura = 40)
    int alto;                // (4 Bytes) Alto (número de pixeles verticales)
    int ancho;               // (4 Bytes) Ancho (número de píxeles horizontales)
    short int numeroPlanos;  // (2 Bytes) Número de planos de color
    short int profundidadCOLORor; // (2 Bytes) Profundidad de color (debe ser 24 para nuestro caso)
    int tipoCompresion;      // (4 Bytes) Tipo de compresión (Vale 0, ya que el bmp es descomprimido)
    int tamanoEstructura;    // (4 Bytes) Tamaño de la estructura Imagen (Paleta)
    int pxmh;                // (4 Bytes) Píxeles por metro horizontal
    int pxmv;                // (4 Bytes) Píxeles por metro vertical
    int COLORoresUsados;     // (4 Bytes) Cantidad de colores usados 
    int COLORoresImportantes; // (4 Bytes) Cantidad de colores importantes
    
    // Campos adicionales para el manejo de la imagen
    int padding;             // Bytes necesarios para que cada fila sea múltiplo de 4 Bytes
    RGB **pixelRGB;          // Matriz dinámica para almacenar los valores RGB de cada píxel
} BMP;

// Prototipos de funciones
void abrir_imagen(BMP *imagen, char ruta[]);
void crear_imagen(BMP *imagen, char ruta[]);
void cambioColor(BMP *mapa, char **mapaR, int x, int y, RGB colorActual, RGB colorNuevo);
char comprobar_Color(BMP *mapa, char **mapaR, int x, int y, RGB colorActual, RGB colorNuevo);

/*
int main(int argc, char* argv[])
Recibe: int argc como el número total de argumentos ingresados al programa y char* argv[] 
como un arreglo que almacena los argumentos de entrada del programa.
Devuelve: 0 si termina correctamente
Observaciones: Función principal que permite cargar una imagen BMP, cambiar el color de un 
área conectada (tipo "relleno") y guardar el resultado en una nueva imagen. Además, muestra 
estadísticas sobre los píxeles modificados y el tiempo de ejecución.
*/
int main(int argc, char* argv[])
{   
    // Variables para la medición de tiempos de ejecución
    clock_t t_inicio, t_final;
    double t_intervalo;
    
    // Variables auxiliares
    int i, j, x, y, cont = 0;
    
    // Variables de la imagen BMP
    BMP img;                     
    RGB colorActual, colorNuevo;
    unsigned char pixel;
    char IMAGEN[45], COORDENADAS[20], COLOR[20];
    
    // Verificar que se hayan proporcionado los argumentos necesarios
    if (argc != 4)
    {
        printf("\nIndique el nombre del archivo, las coordenadas del color a cambiar y el color en estándar RGB");
        printf("\nEjemplo: [user@equipo]$ %s imagen.bmp (x,y) (000,000,000)\n", argv[0]);
        exit(1);
    } 
    
    // Procesar los argumentos recibidos
    strcpy(IMAGEN, argv[1]);
    strcpy(COORDENADAS, argv[2]);
    strcpy(COLOR, argv[3]);
    sscanf(COORDENADAS, "(%d,%d)", &x, &y);
    sscanf(COLOR, "(%hhu,%hhu,%hhu)", &colorNuevo.R, &colorNuevo.G, &colorNuevo.B);
    
    // Abrir la imagen BMP de 24 bits
    abrir_imagen(&img, IMAGEN);
    
    // Mostrar información de la imagen cargada
    printf("\n*************************************************************************");
    printf("\nImagen: %s", IMAGEN);
    printf("\nDimensiones de la imagen:\tAncho=%d\tAlto=%d\n", img.ancho, img.alto);
    
    // Obtener el color actual del píxel seleccionado
    colorActual.R = img.pixelRGB[x][y].R;
    colorActual.G = img.pixelRGB[x][y].G;
    colorActual.B = img.pixelRGB[x][y].B;
    
    // Crear mapa de recorrido para marcar píxeles ya procesados
    char **mapa = malloc(img.ancho * sizeof(char *));
    for(i = 0; i < img.ancho; i++)
    {
        mapa[i] = malloc(img.alto * sizeof(char));
    }
    // Inicializar el mapa con ceros (píxeles no visitados)
    for(i = 0; i < img.ancho; i++)
        for(j = 0; j < img.alto; j++)
            mapa[i][j] = 0;
    
    // Iniciar el algoritmo de cambio de color y medir el tiempo
    t_inicio = clock();
    cambioColor(&img, mapa, x, y, colorActual, colorNuevo);
    t_final = clock();
    
    // Contar los píxeles modificados
    for(i = 0; i < img.ancho; i++)
        for(j = 0; j < img.alto; j++)
            if(mapa[i][j])
                cont++;
    
    // Liberar memoria del mapa de recorrido
    for (i = 0; i < img.ancho; i++) {
        free(mapa[i]);
    }
    free(mapa);
        
    // Crear la imagen BMP tratada y mostrar estadísticas
    crear_imagen(&img, IMAGEN_TRATADA);
    printf("\nImagen BMP tratada guardada en el archivo: %s\n", IMAGEN_TRATADA);
    printf("\nPixeles modificados:\t%d\n", cont);
    t_intervalo = (double)(t_final - t_inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo medido:\t%.8f segundos.\n", t_intervalo);
    
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
void abrir_imagen(BMP *imagen, char *ruta)
{
    FILE *archivo;    
    int i, j, k, resto;
    unsigned char R, B, G, var;
    
    // Abrir archivo en modo binario para lectura
    archivo = fopen(ruta, "rb+");
    if(!archivo)
    { 
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
    fread(&imagen->profundidadCOLORor, sizeof(short int), 1, archivo);
    fread(&imagen->tipoCompresion, sizeof(int), 1, archivo);
    fread(&imagen->tamanoEstructura, sizeof(int), 1, archivo);
    fread(&imagen->pxmh, sizeof(int), 1, archivo);
    fread(&imagen->pxmv, sizeof(int), 1, archivo);
    fread(&imagen->COLORoresUsados, sizeof(int), 1, archivo);
    fread(&imagen->COLORoresImportantes, sizeof(int), 1, archivo);

    // Verificar formato BMP y profundidad de color
    if (imagen->bm[0] != 'B' || imagen->bm[1] != 'M')
    {
        printf("La imagen debe ser un bitmap.\n");
        exit(1);
    }
    if (imagen->profundidadCOLORor != 24) 
    {
        printf("La imagen debe ser de 24 bits.\n"); 
        exit(1);
    }
    
    // Calcular el padding (relleno) para que cada fila sea múltiplo de 4 bytes
    imagen->padding = (4 - (imagen->ancho * 3) % 4) % 4;
    printf("Padding: %d\n", imagen->padding);

    // Asignar memoria para la matriz de píxeles
    imagen->pixelRGB = malloc(imagen->ancho * sizeof(RGB *));
    for(i = 0; i < imagen->ancho; i++)
    {
        imagen->pixelRGB[i] = malloc(imagen->alto * sizeof(RGB));
    }
        
    // Posicionar el puntero al inicio de los datos de píxeles
    fseek(archivo, imagen->offset, SEEK_SET);

    // Leer los datos de píxeles (formato BMP almacena de abajo hacia arriba)
    printf("\n");
    for (j = imagen->alto - 1; j >= 0; j--)
    {
        for (i = 0; i < imagen->ancho; i++)
        {  
            fread(&B, sizeof(char), 1, archivo);  // Byte Blue del píxel
            fread(&G, sizeof(char), 1, archivo);  // Byte Green del píxel
            fread(&R, sizeof(char), 1, archivo);  // Byte Red del píxel
            imagen->pixelRGB[i][j].R = R;
            imagen->pixelRGB[i][j].G = G;
            imagen->pixelRGB[i][j].B = B;
            // Descomentar para debug: printf("(%hhu,%hhu,%hhu) ", R, G, B);
        }
        
        // Saltar los bytes de padding al final de cada fila
        for (k = 1; k <= imagen->padding; k++)
        {
            fread(&var, sizeof(char), 1, archivo);
        }
    }
    printf("\n");
    fclose(archivo);
}

/*
void cambioColor(BMP *mapa, char **mapaR, int x, int y, RGB colorActual, RGB colorNuevo)
Recibe: BMP *mapa como puntero a la estructura BMP que contiene la imagen, char **mapaR como matriz
para registrar píxeles ya procesados, int x e int y como coordenadas del píxel actual, RGB colorActual
como el color original a reemplazar y RGB colorNuevo como el nuevo color a aplicar.
Devuelve: void (No retorna valor explícito)
Observaciones: Función recursiva que implementa un algoritmo de backtracking para cambiar el color
de un área conectada de píxeles del mismo color. Utiliza una estrategia de exploración en cuatro 
direcciones principales (arriba, derecha, abajo, izquierda) y luego rellena las regiones interiores.
La recursividad se detiene cuando se encuentra un píxel de otro color o que ya ha sido procesado.
*/
void cambioColor(BMP *mapa, char **mapaR, int x, int y, RGB colorActual, RGB colorNuevo) {
    // Verificar y cambiar el color del píxel actual
    // Si retorna 1, significa que el píxel ya fue procesado o no cumple con los criterios
    if(comprobar_Color(mapa, mapaR, x, y, colorActual, colorNuevo)) {
        return;
    }
    
    // Variables para marcar límites del área a explorar en cada dirección
    int arb = -1;       // Límite superior
    int abj = mapa->alto; // Límite inferior
    int drc = mapa->ancho; // Límite derecho
    int izq = -1;       // Límite izquierdo
    int i, j;
    
    // =============================================
    // FASE 1: EXPLORACIÓN DE LÍNEAS EN 4 DIRECCIONES
    // =============================================
    
    // --- Explorar hacia arriba hasta encontrar un límite ---
    for(j = y - 1; j >= 0; j--) {
        if(comprobar_Color(mapa, mapaR, x, j, colorActual, colorNuevo)) {
            arb = j;  // Encontró un límite (píxel diferente o ya procesado)
            break;
        }
    }
    
    // --- Explorar hacia la derecha ---
    cambioColor(mapa, mapaR, x + 1, y, colorActual, colorNuevo);
    for(i = x + 1; i < mapa->ancho; i++) {
        if(comprobar_Color(mapa, mapaR, i, y, colorActual, colorNuevo)) {
            drc = i;  // Encontró un límite
            break;
        }
    }
    
    // --- Explorar hacia abajo ---
    cambioColor(mapa, mapaR, x, y + 1, colorActual, colorNuevo);
    for(j = y + 1; j < mapa->alto; j++) {
        if(comprobar_Color(mapa, mapaR, x, j, colorActual, colorNuevo)) {
            abj = j;  // Encontró un límite
            break;
        }
    }
    
    // --- Explorar hacia la izquierda ---
    cambioColor(mapa, mapaR, x - 1, y, colorActual, colorNuevo);
    for(i = x - 1; i >= 0; i--) {
        if(comprobar_Color(mapa, mapaR, i, y, colorActual, colorNuevo)) {
            izq = i;  // Encontró un límite
            break;
        }
    }
    
    // Explorar el píxel por encima
    cambioColor(mapa, mapaR, x, y - 1, colorActual, colorNuevo);
    
    // =============================================
    // FASE 2: RELLENAR ÁREAS ENTRE LOS LÍMITES ENCONTRADOS
    // =============================================
    
    // --- Procesar píxeles a los lados de la línea vertical hacia arriba ---
    for(j = y - 1; j > arb; j--) {
        cambioColor(mapa, mapaR, x + 1, j, colorActual, colorNuevo);  // Píxel a la derecha
        cambioColor(mapa, mapaR, x - 1, j, colorActual, colorNuevo);  // Píxel a la izquierda
    }
    
    // --- Procesar píxeles arriba y abajo de la línea horizontal a la derecha ---
    for(i = x + 1; i < drc; i++) {
        cambioColor(mapa, mapaR, i, y - 1, colorActual, colorNuevo);  // Píxel arriba
        cambioColor(mapa, mapaR, i, y + 1, colorActual, colorNuevo);  // Píxel abajo
    }
    
    // --- Procesar píxeles a los lados de la línea vertical hacia abajo ---
    for(j = y + 1; j < abj; j++) {
        cambioColor(mapa, mapaR, x + 1, j, colorActual, colorNuevo);  // Píxel a la derecha
        cambioColor(mapa, mapaR, x - 1, j, colorActual, colorNuevo);  // Píxel a la izquierda
    }
    
    // --- Procesar píxeles arriba y abajo de la línea horizontal a la izquierda ---
    for(i = x - 1; i > izq; i--) {
        cambioColor(mapa, mapaR, i, y - 1, colorActual, colorNuevo);  // Píxel arriba
        cambioColor(mapa, mapaR, i, y + 1, colorActual, colorNuevo);  // Píxel abajo
    }
}

/*
char comprobar_Color(BMP *mapa, char **mapaR, int x, int y, RGB colorActual, RGB colorNuevo)
Recibe: BMP *mapa como puntero a la estructura BMP que contiene la imagen, char **mapaR como matriz
para registrar píxeles ya procesados, int x e int y como coordenadas del píxel a comprobar, 
RGB colorActual como el color original a reemplazar y RGB colorNuevo como el nuevo color a aplicar.
Devuelve: char como indicador booleano (1 si el píxel no debe procesarse, 0 si se procesó correctamente)
Observaciones: Función auxiliar que verifica si un píxel en las coordenadas dadas cumple con los 
criterios para ser coloreado (estar dentro de los límites de la imagen y tener el color original).
Si cumple los criterios, cambia su color al nuevo valor y lo marca como procesado en el mapa de 
recorrido.
*/
char comprobar_Color(BMP *mapa, char **mapaR, int x, int y, RGB colorActual, RGB colorNuevo) {
    // Comprobar que las coordenadas estén dentro de los límites de la imagen
    if(x < 0 || x >= mapa->ancho || y < 0 || y >= mapa->alto) {
        return 1;  // Fuera de límites
    }
    
    // Comprobar si el píxel actual tiene el color que buscamos reemplazar
    if(mapa->pixelRGB[x][y].R != colorActual.R 
    || mapa->pixelRGB[x][y].G != colorActual.G 
    || mapa->pixelRGB[x][y].B != colorActual.B) {
        return 1;  // Color diferente o ya procesado
    }
    
    // Cambiar el color del píxel y marcar como procesado
    mapa->pixelRGB[x][y].R = colorNuevo.R;
    mapa->pixelRGB[x][y].G = colorNuevo.G;
    mapa->pixelRGB[x][y].B = colorNuevo.B;
    mapaR[x][y] = 1;  // Marcar como procesado
    
    return 0;  // Procesado correctamente
}

/*
void crear_imagen(BMP *imagen, char ruta[])
Recibe: BMP *imagen como puntero a la estructura BMP que contiene la imagen modificada y 
char ruta[] como la ruta donde se guardará el nuevo archivo BMP.
Devuelve: void (No retorna valor explícito)
Observaciones: Función que crea un nuevo archivo BMP a partir de los datos almacenados en la 
estructura BMP. Escribe la cabecera y los datos de los píxeles en el formato adecuado para BMP 
(de abajo hacia arriba con padding). Al finalizar, libera la memoria reservada para la matriz 
de píxeles.
*/
void crear_imagen(BMP *imagen, char ruta[]) {
    FILE *archivo;
    int i, j, k;
    unsigned char var = 0x00;  // Byte de relleno para el padding

    // Crear archivo en modo binario para escritura
    archivo = fopen(ruta, "wb+");
    if(!archivo) { 
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
    fwrite(&imagen->profundidadCOLORor, sizeof(short int), 1, archivo);    
    fwrite(&imagen->tipoCompresion, sizeof(int), 1, archivo);
    fwrite(&imagen->tamanoEstructura, sizeof(int), 1, archivo);
    fwrite(&imagen->pxmh, sizeof(int), 1, archivo);
    fwrite(&imagen->pxmv, sizeof(int), 1, archivo);
    fwrite(&imagen->COLORoresUsados, sizeof(int), 1, archivo);
    fwrite(&imagen->COLORoresImportantes, sizeof(int), 1, archivo);
            
    // Posicionar el puntero al inicio de los datos de píxeles
    fseek(archivo, imagen->offset, SEEK_SET);

    // Escribir los datos de píxeles (de abajo hacia arriba, formato BMP)
    for (j = imagen->alto - 1; j >= 0; j--) {
        for (i = 0; i < imagen->ancho; i++) {
            fwrite(&imagen->pixelRGB[i][j].B, sizeof(char), 1, archivo);  // Byte Blue del píxel 
            fwrite(&imagen->pixelRGB[i][j].G, sizeof(char), 1, archivo);  // Byte Green del píxel
            fwrite(&imagen->pixelRGB[i][j].R, sizeof(char), 1, archivo);  // Byte Red del píxel
        }
        
        // Escribir bytes de padding al final de cada fila
        for (k = 1; k <= imagen->padding; k++)
            fwrite(&var, sizeof(char), 1, archivo);  // Escribir bytes de relleno        
    }
    
    // Liberar la memoria de la matriz de píxeles
    for (i = 0; i < imagen->ancho; i++) {
        free(imagen->pixelRGB[i]);
    }
    free(imagen->pixelRGB);

    fclose(archivo);
}