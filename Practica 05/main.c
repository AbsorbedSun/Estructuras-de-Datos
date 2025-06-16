/*
Practica 5.
Diccionario Tablas Hash
Autores:
        Garcia Ambrosio Aldo
        Ponce Espino Miguel Angel
        Salinas Aquino Omar Ivan
Version: 3.1 (Corregida - Fix Buffer y Inicialización)
Uso de IA: ChatGPT 3.5, Claude Sonnet 4

Ejecutar como: gcc main.c Recursos/TADListaDL.c Recursos/animacion.c  Hash/tablaH.c -o diccionario
           ./diccionario
           /diccionarios/enciclopedia_de_...txt

Programa que utiliza tablas hash y listas doblemente ligadas.
Permite cargar un archivo de palabras y definiciones,
agregar nuevas entradas, buscar, modificar y eliminar definiciones,
mostrar estadisticas de la tabla hash, exportar datos completos o
en forma individual, y realizar busquedas por patrones.
*/

// Librerias necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Recursos/TADListaDL.h"
#include "Recursos/animacion.h"

// Declaraciones de funciones de tablaH.c
void seleccionar_hash();
void inicializar_tabla_hash();
void crear_tabla(int (*hash)(const char *));
int insertar(const char *palabra, const char *definicion);
elemento *buscar(const char *palabra, int *pasos, int *indice);
int modificar(const char *palabra, const char *nueva_definicion);
int eliminar(const char *palabra);
void cargar_archivo(const char *nombre);
void estadisticas_hash();
void limpiar_cadena(char *s);
void exportar_todo(const char *archivo);
void exportar_definicion(const char *palabra, const char *archivo);
void buscar_por_patron(const char *patron, int modo);

// Funciones para la interfaz mejorada
void mostrar_header();
void mostrar_menu();
void mostrar_separador();
void mostrar_mensaje_exito(const char *mensaje);
void mostrar_mensaje_error(const char *mensaje);
void mostrar_prompt(const char *prompt);
void pausar_pantalla();

/*
Función para mostrar el header del programa
*/
void mostrar_header() {
    system("cls");
    printf("\n");
    printf("  +==============================================================+\n");
    printf("  |                                                              |\n");
    printf("  |           ####   #   #  #####  #   #   ####                 |\n");
    printf("  |          #       #   #    #    ## ##  #    #                |\n");
    printf("  |          #  ##   #   #    #    # # #  #    #                |\n");
    printf("  |          #   #   #   #    #    #   #  #    #                |\n");
    printf("  |           ####    ###   #####  #   #   ####                 |\n");
    printf("  |                                                              |\n");
    printf("  |                  DICCIONARIO INTELIGENTE                    |\n");
    printf("  |                    Tablas Hash + Listas                     |\n");
    printf("  |                                                              |\n");
    printf("  +==============================================================+\n");
}

/*
Función para mostrar el menú principal
*/
void mostrar_menu() {
    printf("\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                      MENU PRINCIPAL                        |\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                                                             |\n");
    printf("  |   [1] | Cargar Diccionario desde Archivo                   |\n");
    printf("  |   [2] | Agregar Nueva Palabra                              |\n");
    printf("  |   [3] | Buscar Definicion                                  |\n");
    printf("  |   [4] | Modificar Definicion                               |\n");
    printf("  |   [5] | Eliminar Palabra                                   |\n");
    printf("  |   [6] | Ver Estadisticas del Diccionario                   |\n");
    printf("  |   [7] | Exportar Todo el Contenido                         |\n");
    printf("  |   [8] | Exportar Definicion Especifica                     |\n");
    printf("  |   [9] | Busqueda Avanzada por Patrones                     |\n");
    printf("  |  [10] | Salir del Programa                                 |\n");
    printf("  |                                                             |\n");
    printf("  +-------------------------------------------------------------+\n");
}

/*
Función para mostrar separadores
*/
void mostrar_separador() {
    printf("  ===============================================================\n");
}

/*
Función para mostrar mensajes de éxito
*/
void mostrar_mensaje_exito(const char *mensaje) {
    printf("\n  [OK] %s\n", mensaje);
}

/*
Función para mostrar mensajes de error
*/
void mostrar_mensaje_error(const char *mensaje) {
    printf("\n  [ERROR] %s\n", mensaje);
}

/*
Función para mostrar prompts de entrada
*/
void mostrar_prompt(const char *prompt) {
    printf("\n  >> %s: ", prompt);
}

/*
Función para pausar la pantalla
*/
void pausar_pantalla() {
    printf("\n  Presiona Enter para continuar...");
    getchar();
}

/*
Función para mostrar el menú de búsqueda avanzada
*/
void mostrar_menu_busqueda() {
    printf("\n  +-----------------------------------------+\n");
    printf("  |          BUSQUEDA AVANZADA              |\n");
    printf("  +-----------------------------------------+\n");
    printf("  |  [1] | Por letra inicial                |\n");
    printf("  |  [2] | Frase en definicion              |\n");
    printf("  |  [3] | Subcadena en palabra             |\n");
    printf("  +-----------------------------------------+\n");
}

/*
Funcion principal: muestra el menu de operaciones del diccionario hash.
Opciones: carga de archivo, insercion, busqueda, modificacion,
eliminacion, estadisticas, exportacion y busqueda por patron.
Se repite hasta que el usuario seleccione Salir.
*/
int main()
{
    char buffer[256];
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    system("ani.bat");
    anima();

    mostrar_header();
    mostrar_separador();
    printf("\n  Iniciando sistema de diccionario...\n");

    // CRÍTICO: Seleccionar la función hash ANTES de cualquier operación
    printf("  >> Seleccionando funcion hash optima...\n");
    seleccionar_hash();

    // Inicializar la tabla hash después de seleccionar la función
    printf("  >> Inicializando tabla hash...\n");
    inicializar_tabla_hash();
    
    mostrar_mensaje_exito("Sistema inicializado correctamente");
    printf("\n");
    system("pause");

    int op;
    char palabra[101], definicion[251], archivo[200];

    // Menu principal
    do
    {
        mostrar_header();
        mostrar_menu();
        
        mostrar_prompt("Selecciona una opcion (1-10)");

        // Mejorar la lectura de la opción
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            mostrar_mensaje_error("Error leyendo entrada. Intente nuevamente");
            clearerr(stdin);
            continue;
        }

        // Limpiar el buffer y convertir a entero
        buffer[strcspn(buffer, "\n\r")] = '\0';
        op = atoi(buffer);

        // Validar que la opción esté en el rango correcto
        if (op < 1 || op > 10)
        {
            mostrar_mensaje_error("Opcion no valida. Debe ser entre 1 y 10");
            pausar_pantalla();
            continue;
        }

        printf("\n");
        mostrar_separador();

        switch (op)
        {
        case 1:
            printf("\n  [ARCHIVO] CARGAR DICCIONARIO\n");
            mostrar_prompt("Nombre del archivo");
            if (!fgets(archivo, sizeof(archivo), stdin))
            {
                mostrar_mensaje_error("Error leyendo nombre del archivo");
                break;
            }
            limpiar_cadena(archivo);
            if (strlen(archivo) == 0)
            {
                mostrar_mensaje_error("Nombre de archivo vacio");
                break;
            }
            printf("\n  [CARGANDO] Procesando archivo...\n");
            cargar_archivo(archivo);
            break;

        case 2:
            printf("\n  [AGREGAR] NUEVA PALABRA\n");
            mostrar_prompt("Palabra");
            if (!fgets(palabra, sizeof(palabra), stdin))
            {
                mostrar_mensaje_error("Error leyendo palabra");
                break;
            }
            limpiar_cadena(palabra);
            if (strlen(palabra) == 0)
            {
                mostrar_mensaje_error("Palabra vacia");
                break;
            }

            mostrar_prompt("Definicion");
            if (!fgets(definicion, sizeof(definicion), stdin))
            {
                mostrar_mensaje_error("Error leyendo definicion");
                break;
            }
            limpiar_cadena(definicion);
            if (strlen(definicion) == 0)
            {
                mostrar_mensaje_error("Definicion vacia");
                break;
            }

            if (insertar(palabra, definicion)) {
                mostrar_mensaje_exito("Palabra agregada exitosamente");
            } else {
                mostrar_mensaje_error("La palabra ya existe en el diccionario");
            }
            break;

        case 3:
        {
            printf("\n  [BUSCAR] DEFINICION\n");
            int pasos = 0, idx = -1;
            mostrar_prompt("Palabra a buscar");
            //libro();
            mostrar_header();
            printf("\n  [BUSCAR] DEFINICION\n");
            mostrar_separador();
            
            if (!fgets(palabra, sizeof(palabra), stdin))
            {
                mostrar_mensaje_error("Error leyendo palabra");
                break;
            }
            limpiar_cadena(palabra);
            if (strlen(palabra) == 0)
            {
                mostrar_mensaje_error("Palabra vacia");
                break;
            }

            printf("\n  [BUSCANDO] '%s'...\n", palabra);

            elemento *e = buscar(palabra, &pasos, &idx);

            if (e)
            {
                printf("\n  +-----------------------------------------------------------------------+\n");
                printf("  |                        RESULTADO                                      |\n");
                printf("  +-----------------------------------------------------------------------+\n");
                printf("  | Palabra: %-50s            |\n", e->palabra);
                printf("  |                                                                       |\n");
                printf("  | Definicion:                                                           |\n");
                printf("  | %-59s |\n", e->definicion);
                printf("  |                                                                         |\n");
                printf("  | Ubicacion: Lista %d | Pasos: %d                                         |\n", idx, pasos);
                printf("  +-----------------------------------------------------------------------+\n");
                mostrar_mensaje_exito("Palabra encontrada");
            }
            else
            {
                mostrar_mensaje_error("Palabra no encontrada en el diccionario");
            }
            break;
        }

        case 4:
            printf("\n  [MODIFICAR] DEFINICION\n");
            mostrar_prompt("Palabra a modificar");
            if (!fgets(palabra, sizeof(palabra), stdin))
            {
                mostrar_mensaje_error("Error leyendo palabra");
                break;
            }
            limpiar_cadena(palabra);
            if (strlen(palabra) == 0)
            {
                mostrar_mensaje_error("Palabra vacia");
                break;
            }

            mostrar_prompt("Nueva definicion");
            if (!fgets(definicion, sizeof(definicion), stdin))
            {
                mostrar_mensaje_error("Error leyendo definicion");
                break;
            }
            limpiar_cadena(definicion);
            if (strlen(definicion) == 0)
            {
                mostrar_mensaje_error("Definicion vacia");
                break;
            }

            if (modificar(palabra, definicion)) {
                mostrar_mensaje_exito("Definicion modificada exitosamente");
            } else {
                mostrar_mensaje_error("Palabra no encontrada");
            }
            break;

        case 5:
            printf("\n  [ELIMINAR] PALABRA\n");
            mostrar_prompt("Palabra a eliminar");
            if (!fgets(palabra, sizeof(palabra), stdin))
            {
                mostrar_mensaje_error("Error leyendo palabra");
                break;
            }
            limpiar_cadena(palabra);
            if (strlen(palabra) == 0)
            {
                mostrar_mensaje_error("Palabra vacia");
                break;
            }

            printf("\n  [CONFIRMACION] Esta seguro de eliminar '%s'? (s/n): ", palabra);
            char confirmacion;
            scanf(" %c", &confirmacion);
            getchar(); // Limpiar buffer
            
            if (confirmacion == 's' || confirmacion == 'S') {
                if (eliminar(palabra)) {
                    mostrar_mensaje_exito("Palabra eliminada exitosamente");
                } else {
                    mostrar_mensaje_error("Palabra no encontrada");
                }
            } else {
                printf("\n  [INFO] Operacion cancelada");
            }
            break;

        case 6:
            printf("\n  [ESTADISTICAS] DEL DICCIONARIO\n");
            mostrar_separador();
            estadisticas_hash();
            break;

        case 7:
            printf("\n  [EXPORTAR] TODO EL CONTENIDO\n");
            mostrar_prompt("Nombre del archivo de destino");
            if (!fgets(archivo, sizeof(archivo), stdin))
            {
                mostrar_mensaje_error("Error leyendo nombre del archivo");
                break;
            }
            limpiar_cadena(archivo);
            if (strlen(archivo) == 0)
            {
                mostrar_mensaje_error("Nombre de archivo vacio");
                break;
            }
            printf("\n  [EXPORTANDO] Contenido completo...\n");
            exportar_todo(archivo);
            break;

        case 8:
            printf("\n  [EXPORTAR] DEFINICION ESPECIFICA\n");
            mostrar_prompt("Palabra a exportar");
            if (!fgets(palabra, sizeof(palabra), stdin))
            {
                mostrar_mensaje_error("Error leyendo palabra");
                break;
            }
            limpiar_cadena(palabra);
            if (strlen(palabra) == 0)
            {
                mostrar_mensaje_error("Palabra vacia");
                break;
            }

            mostrar_prompt("Archivo de destino");
            if (!fgets(archivo, sizeof(archivo), stdin))
            {
                mostrar_mensaje_error("Error leyendo archivo");
                break;
            }
            limpiar_cadena(archivo);
            if (strlen(archivo) == 0)
            {
                mostrar_mensaje_error("Nombre de archivo vacio");
                break;
            }
            printf("\n  [EXPORTANDO] Definicion...\n");
            exportar_definicion(palabra, archivo);
            break;

        case 9:
        {
            printf("\n  [BUSQUEDA] AVANZADA POR PATRONES\n");
            mostrar_menu_busqueda();
            
            int modo;
            mostrar_prompt("Selecciona modo de busqueda (1-3)");
            if (!fgets(buffer, sizeof(buffer), stdin))
            {
                mostrar_mensaje_error("Error leyendo opcion");
                break;
            }
            buffer[strcspn(buffer, "\n\r")] = '\0';
            modo = atoi(buffer);

            if (modo < 1 || modo > 3)
            {
                mostrar_mensaje_error("Modo invalido. Debe ser 1, 2 o 3");
                break;
            }

            mostrar_prompt("Ingresa patron de busqueda");
            if (!fgets(palabra, sizeof(palabra), stdin))
            {
                mostrar_mensaje_error("Error leyendo patron");
                break;
            }
            limpiar_cadena(palabra);
            if (strlen(palabra) == 0)
            {
                mostrar_mensaje_error("Patron vacio");
                break;
            }
            
            printf("\n  [BUSCANDO] Coincidencias...\n");
            buscar_por_patron(palabra, modo);
            break;
        }

        case 10:
            printf("\n  [SALIENDO] DEL PROGRAMA\n");
            mostrar_separador();
            printf("\n  Gracias por usar el Diccionario GUIMO\n");
            printf("  Desarrollado por: Garcia, Ponce & Salinas\n\n");
            break;

        default:
            mostrar_mensaje_error("Opcion no valida");
        }
        
        if (op != 10) {
            printf("\n");
            pausar_pantalla();
        }

    } while (op != 10);

    return 0;
}