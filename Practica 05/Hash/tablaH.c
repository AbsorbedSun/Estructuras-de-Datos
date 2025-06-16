/*
hash_diccionario_funciones.c
Practica 5. Diccionario con Tabla Hash - VERSION OPTIMIZADA
Autores:
        Garcia Ambrosio Aldo
        Ponce Espino Miguel Angel
        Salinas Aquino Omar Ivan
Version: 2.6 
Uso de IA: ChatGPT 3.5, Claude Sonnet 4

Compilacion:
    gcc -o diccionario_func funciones_hash.o TADListaDL.o hash_diccionario_funciones.c

Este programa extiende el diccionario con tabla hash para permitir:
- Seleccionar en tiempo de ejecucion entre dos funciones de hash (hashSuma o hashDJB2)
- Insercion, busqueda, modificacion y eliminacion de entradas
- Carga desde archivo, estadisticas, exportacion y busqueda por patron
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Recursos/TADListaDL.h"

#define TAM_TABLA 101 // Numero de listas en la tabla
#define MAX_BUFFER 512
#define MAX_PALABRA 100
#define MAX_DEFINICION 300

int hashSuma(const char *);
int hashDJB2(const char *);

lista tabla[TAM_TABLA];
int (*funcion_hash)(const char *); // Puntero a función hash

/*
calcular_hash_normalizado
Función auxiliar que calcula el hash y lo normaliza al rango de la tabla
Parámetros:
    palabra: cadena para calcular hash
Retorna:
    Índice válido en el rango [0, TAM_TABLA-1]
*/
static inline unsigned int calcular_hash_normalizado(const char *palabra)
{
    int valor_hash = funcion_hash(palabra);
    // Manejo más eficiente de valores negativos
    return (unsigned int)(valor_hash < 0 ? -valor_hash : valor_hash) % TAM_TABLA;
}

/*
leer_entrada_segura
Función auxiliar para leer entrada del usuario de forma segura
Parámetros:
    buffer: buffer donde almacenar la entrada
    tamanio: tamaño máximo del buffer
Retorna:
    1 si se leyó correctamente, 0 en caso de error
*/
static int leer_entrada_segura(char *buffer, int tamanio)
{
    if (!fgets(buffer, tamanio, stdin))
        return 0;
    
    // Eliminar salto de línea
    buffer[strcspn(buffer, "\n\r")] = '\0';
    return 1;
}

/*
seleccionar_hash
Presenta un menu al usuario para elegir la funcion de hash:
1. hashSuma (suma de caracteres)
2. hashDJB2 (algoritmo DJB2)
Asigna el puntero global 'funcion_hash'.
*/
void seleccionar_hash()
{
    char buffer[10];
    int opcion;

    do
    {
        printf("\nSeleccione la tecnica de hash a utilizar:\n");
        printf("1. Suma\n");
        printf("2. DJB2\n");
        printf("Opcion: ");

        if (!leer_entrada_segura(buffer, sizeof(buffer)))
        {
            printf("Error leyendo entrada. Intente de nuevo.\n");
            continue;
        }

        opcion = atoi(buffer);
        
        if (opcion < 1 || opcion > 2)
        {
            printf("Opcion no valida. Debe ser 1 o 2.\n");
        }

    } while (opcion < 1 || opcion > 2);

    // Asignación más limpia usando operador ternario
    funcion_hash = (opcion == 1) ? hashSuma : hashDJB2;
    printf("Funcion hash seleccionada: %s\n", 
           (opcion == 1) ? "hashSuma" : "hashDJB2");
}

/*
limpiar_cadena
Remueve salto de linea y espacios/tabs iniciales, luego convierte
toda la cadena a minusculas
Parametros:
    s: cadena a limpiar (char *).
Retorna:
    Void; modifica la cadena in situ
*/
void limpiar_cadena(char *s)
{
    if (!s) return;

    // Eliminar salto de línea
    s[strcspn(s, "\n\r")] = '\0';
    
    // Saltar espacios y tabs iniciales
    char *inicio = s;
    while (*inicio && (*inicio == ' ' || *inicio == '\t'))
        inicio++;
    
    // Si hay espacios iniciales, mover el contenido
    if (inicio != s)
        memmove(s, inicio, strlen(inicio) + 1);
    
    // Convertir a minúsculas de forma más eficiente
    for (char *p = s; *p; p++)
        *p = tolower((unsigned char)*p);
}

/*
crear_tabla
Inicializa el arreglo global 'tabla' con listas vacias y fija
la funcion de hash a usar
Parametros:
    hash: puntero a la funcion de hash a emplear
Retorna:
    Void; deja lista la tabla para operaciones posteriores
*/
void crear_tabla(int (*hash)(const char *))
{
    funcion_hash = hash;
    
    // Inicialización más eficiente
    for (int i = 0; i < TAM_TABLA; i++)
        Initialize(&tabla[i]);
}

/*
insertar
Agrega una nueva palabra y definicion a la tabla hash.
Parametros:
    palabra: cadena con la clave a insertar.
    definicion: cadena con la definicion asociada.
Retorna:
    1 si se inserto correctamente,
    0 si ya existia la palabra.
Accion adicional:
    Imprime numero de colisiones, pasos y valor de hash.
*/
int insertar(const char *palabra, const char *definicion)
{
    if (!palabra || !definicion) return 0;

    unsigned int h = calcular_hash_normalizado(palabra);
    
    // Verificar si ya existe y contar pasos
    posicion aux = First(&tabla[h]);
    int pasos = 0;
    
    while (aux)
    {
        pasos++;
        elemento tmp = Position(&tabla[h], aux);
        if (strcmp(tmp.palabra, palabra) == 0)
            return 0; // Ya existe
        aux = Following(&tabla[h], aux);
    }

    // Crear y agregar nuevo elemento
    elemento e;
    strncpy(e.palabra, palabra, sizeof(e.palabra) - 1);
    strncpy(e.definicion, definicion, sizeof(e.definicion) - 1);
    e.palabra[sizeof(e.palabra) - 1] = '\0';
    e.definicion[sizeof(e.definicion) - 1] = '\0';

    Add(&tabla[h], e);
    
    printf("Insertada en lista %u detras de %d palabras (pasos: %d, hash: %u, colisiones: %d)\n",
           h, Size(&tabla[h]) - 1, pasos, h, pasos);
    
    return 1;
}

/*
buscar_nodo_por_posicion
Función auxiliar para encontrar el nodo en una posición específica
Parámetros:
    lista_ptr: puntero a la lista
    posicion: posición del elemento (1-indexed)
Retorna:
    Puntero al nodo o NULL si no se encuentra
*/
static nodo* buscar_nodo_por_posicion(lista *lista_ptr, int posicion)
{
    if (posicion <= 0) return NULL;
    
    nodo *actual = lista_ptr->frente;
    for (int i = 1; i < posicion && actual; i++)
        actual = actual->siguiente;
    
    return actual;
}

/*
buscar
Busca una palabra en la tabla hash.
Parametros:
    palabra: clave a buscar.
    pasos: puntero donde se almacenan las comparaciones realizadas.
    indice: puntero donde se guarda el indice hash.
Retorna:
    Puntero al elemento encontrado, o NULL si no existe.
*/
elemento *buscar(const char *palabra, int *pasos, int *indice)
{
    if (!palabra || !pasos || !indice) return NULL;

    // Limpiar palabra para comparación
    char palabra_limpia[MAX_PALABRA];
    strncpy(palabra_limpia, palabra, sizeof(palabra_limpia) - 1);
    palabra_limpia[sizeof(palabra_limpia) - 1] = '\0';
    limpiar_cadena(palabra_limpia);

    unsigned int h = calcular_hash_normalizado(palabra_limpia);
    *indice = h;
    *pasos = 0;

    posicion aux = First(&tabla[h]);
    while (aux)
    {
        (*pasos)++;
        elemento tmp = Position(&tabla[h], aux);
        if (strcmp(tmp.palabra, palabra_limpia) == 0)
        {
            // Encontrar el nodo correspondiente
            nodo *nodo_encontrado = buscar_nodo_por_posicion(&tabla[h], *pasos);
            return nodo_encontrado ? &nodo_encontrado->e : NULL;
        }
        aux = Following(&tabla[h], aux);
    }
    
    return NULL;
}

/*
modificar
Cambia la definicion de una palabra existente.
Parametros:
    palabra: clave a modificar.
    nueva_definicion: definicion de reemplazo.
Retorna:
    1 si se modifico, 0 si la clave no existe.
Accion adicional:
    Imprime pasos y posicion donde se realizo el cambio.
*/
int modificar(const char *palabra, const char *nueva_definicion)
{
    if (!palabra || !nueva_definicion) return 0;

    int pasos, h;
    elemento *e = buscar(palabra, &pasos, &h);
    
    if (!e) return 0;
    
    // Copia segura de la nueva definición
    strncpy(e->definicion, nueva_definicion, sizeof(e->definicion) - 1);
    e->definicion[sizeof(e->definicion) - 1] = '\0';
    
    printf("Modificada en lista %d detras de %d palabras (pasos: %d, hash: %d)\n", 
           h, pasos - 1, pasos, h);
    
    return 1;
}

/*
eliminar
Suprime una entrada de la tabla hash.
Parametros:
    palabra: clave a eliminar.
Retorna:
    1 si se elimino, 0 si no se encontro la clave.
Accion adicional:
    Imprime pasos y posicion de la eliminacion.
*/
int eliminar(const char *palabra)
{
    if (!palabra) return 0;

    unsigned int h = calcular_hash_normalizado(palabra);
    int pasos = 0;
    
    nodo *aux = tabla[h].frente;
    while (aux)
    {
        pasos++;
        if (strcmp(aux->e.palabra, palabra) == 0)
        {
            Remove(&tabla[h], aux);
            printf("Eliminada en lista %u detras de %d palabras (pasos: %d, hash: %u)\n", 
                   h, pasos - 1, pasos, h);
            return 1;
        }
        aux = aux->siguiente;
    }
    
    return 0;
}

/*
procesar_linea_archivo
Función auxiliar para procesar una línea del archivo
Parámetros:
    linea: línea a procesar
Retorna:
    1 si se procesó correctamente, 0 si hubo error
*/
static int procesar_linea_archivo(char *linea)
{
    char *separador = strchr(linea, ':');
    if (!separador) return 0;
    
    *separador = '\0';
    char *palabra = linea;
    char *definicion = separador + 1;
    
    // Limpiar palabra y definición
    palabra[strcspn(palabra, "\n\r")] = '\0';
    definicion[strcspn(definicion, "\n\r")] = '\0';
    
    // Saltar espacios iniciales en definición
    while (*definicion && (*definicion == '\t' || *definicion == ' '))
        definicion++;
    
    limpiar_cadena(palabra);
    
    if (strlen(palabra) > 0 && strlen(definicion) > 0)
    {
        insertar(palabra, definicion);
        return 1;
    }
    
    return 0;
}

/*
cargar_archivo
Lee un archivo de texto con lineas "palabra:definicion"
e inserta cada par en la tabla hash
Parametros:
    nombre: ruta del archivo de entrada
Retorna:
    Void; informa errores de apertura si los hay
*/
void cargar_archivo(const char *nombre)
{
    if (!nombre) return;

    FILE *f = fopen(nombre, "r");
    if (!f)
    {
        perror("No se pudo abrir el archivo");
        return;
    }

    char linea[MAX_BUFFER];
    int lineas_procesadas = 0;
    
    while (fgets(linea, sizeof(linea), f))
    {
        if (procesar_linea_archivo(linea))
            lineas_procesadas++;
    }
    
    fclose(f);
    printf("Archivo cargado: %d entradas procesadas.\n", lineas_procesadas);
}

/*
estadisticas_hash
Calcula e imprime:
- Total de palabras
- Numero de listas vacias
- Promedio de entradas por lista
- Tamano maximo de lista
Parametros:
    Nada
Retorna:
    Void; muestra resultados en pantalla
*/
void estadisticas_hash()
{
    int total = 0, vacias = 0, max_tamanio = 0;
    
    // Calcular estadísticas en una sola pasada
    for (int i = 0; i < TAM_TABLA; i++)
    {
        int tamanio = Size(&tabla[i]);
        total += tamanio;
        
        if (tamanio == 0)
            vacias++;
        else if (tamanio > max_tamanio)
            max_tamanio = tamanio;
    }
    
    printf("\n--- Estadisticas Hash ---\n");
    printf("Total palabras: %d\n", total);
    printf("Listas vacias: %d\n", vacias);
    printf("Promedio por lista: %.2f\n", 
           total > 0 ? (float)total / TAM_TABLA : 0.0f);
    printf("Maximo en una lista: %d\n", max_tamanio);
    printf("Factor de carga: %.2f%%\n", 
           (float)(TAM_TABLA - vacias) / TAM_TABLA * 100);
}

/*
exportar_todo
Guarda todas las entradas del diccionario en un archivo de texto
Parametros:
    archivo: ruta del archivo de salida
Retorna:
    Void; informa si no puede abrir el archivo
*/
void exportar_todo(const char *archivo)
{
    if (!archivo) return;

    FILE *f = fopen(archivo, "w");
    if (!f)
    {
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }

    int total_exportadas = 0;
    
    for (int i = 0; i < TAM_TABLA; i++)
    {
        posicion pos = First(&tabla[i]);
        while (pos)
        {
            elemento e = Position(&tabla[i], pos);
            fprintf(f, "%s: %s\n", e.palabra, e.definicion);
            total_exportadas++;
            pos = Following(&tabla[i], pos);
        }
    }

    fclose(f);
    printf("Exportacion completada en '%s' (%d entradas)\n", archivo, total_exportadas);
}

/*
exportar_definicion
Escribe la definicion de una palabra dada en un archivo de texto
Parametros:
    palabra: clave a buscar
    archivo: ruta de salida
Retorna:
    Void; avisa si la palabra no existe o no puede crear el archivo
*/
void exportar_definicion(const char *palabra, const char *archivo)
{
    if (!palabra || !archivo) return;

    int pasos, indice;
    elemento *e = buscar(palabra, &pasos, &indice);
    
    if (!e)
    {
        printf("Palabra no encontrada.\n");
        return;
    }

    FILE *f = fopen(archivo, "w");
    if (!f)
    {
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }

    fprintf(f, "%s: %s\n", e->palabra, e->definicion);
    fclose(f);
    printf("Definicion exportada a '%s' (pasos de busqueda: %d)\n", archivo, pasos);
}

/*
convertir_a_minusculas
Función auxiliar para convertir cadena a minúsculas
Parámetros:
    origen: cadena origen
    destino: cadena destino
    tamanio: tamaño máximo del destino
*/
static void convertir_a_minusculas(const char *origen, char *destino, int tamanio)
{
    int i;
    for (i = 0; i < tamanio - 1 && origen[i]; i++)
        destino[i] = tolower((unsigned char)origen[i]);
    destino[i] = '\0';
}

/*
buscar_por_patron
Busca coincidencias en palabra o definicion segun modo:
1: misma primera letra
2: patron en la definicion
3: patron en la palabra
Parametros:
    patron: texto a buscar
    modo: 1, 2 o 3
Retorna:
    Void; imprime los resultados o mensaje de "no encontrado"
*/
void buscar_por_patron(const char *patron, int modo)
{
    if (!patron || modo < 1 || modo > 3) return;

    char patron_min[MAX_DEFINICION];
    convertir_a_minusculas(patron, patron_min, sizeof(patron_min));
    
    int encontrados = 0;
    char primera_letra = patron_min[0];
    
    printf("Resultados de busqueda:\n");

    for (int i = 0; i < TAM_TABLA; i++)
    {
        posicion pos = First(&tabla[i]);
        while (pos)
        {
            elemento e = Position(&tabla[i], pos);
            int coincide = 0;

            switch (modo)
            {
                case 1: // Primera letra
                    coincide = (tolower((unsigned char)e.palabra[0]) == primera_letra);
                    break;
                    
                case 2: // Patrón en definición
                {
                    char def_min[MAX_DEFINICION];
                    convertir_a_minusculas(e.definicion, def_min, sizeof(def_min));
                    coincide = (strstr(def_min, patron_min) != NULL);
                    break;
                }
                
                case 3: // Patrón en palabra
                {
                    char pal_min[MAX_PALABRA];
                    convertir_a_minusculas(e.palabra, pal_min, sizeof(pal_min));
                    coincide = (strstr(pal_min, patron_min) != NULL);
                    break;
                }
            }

            if (coincide)
            {
                printf("- %s: %s\n", e.palabra, e.definicion);
                encontrados++;
            }

            pos = Following(&tabla[i], pos);
        }
    }

    if (encontrados == 0)
        printf("Sin coincidencias.\n");
    else
        printf("Total encontradas: %d\n", encontrados);
}

/*
inicializar_tabla_hash
Inicializa todas las listas de la tabla hash después de seleccionar la función hash
Esta función debe llamarse después de seleccionar_hash() y antes de cualquier operación
*/
void inicializar_tabla_hash()
{
    for (int i = 0; i < TAM_TABLA; i++)
        Initialize(&tabla[i]);
    
    printf("Tabla hash inicializada correctamente con %d listas.\n", TAM_TABLA);
}

/*
hashSuma
Calcula un valor hash sumando los codigos ASCII de cada caracter
Parametros:
    s: puntero a una cadena terminada en '\0' (const char*)
Retorna:
    Numero entero resultado de la suma de valores ASCII de cada caracter
Comentarios:
    - Metodo muy simple, alta probabilidad de colisiones en cadenas cortas o repetitivas
*/
int hashSuma(const char *s)
{
    if (!s) return 0;
    
    int suma = 0;
    while (*s)
        suma += (unsigned char)*s++;
    
    return suma;
}

/*
hashDJB2
Aplica el algoritmo DJB2, desarrollado por Dan Bernstein
Parametros:
    s: puntero a una cadena terminada en '\0' (const char*)
Retorna:
    Numero entero obtenido al aplicar la formula:
        hash(i+1) = hash(i) * 33 + c
    convertido a tipo int.
Observaciones:
    - Ofrece buen balance entre rapidez y distribucion
    - Puede generar desbordamiento en unsigned long; aqui se trunca a int
*/
int hashDJB2(const char *s)
{
    if (!s) return 5381;
    
    unsigned long hash = 5381;
    unsigned char c;
    
    while ((c = *s++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    
    return (int)hash;
}