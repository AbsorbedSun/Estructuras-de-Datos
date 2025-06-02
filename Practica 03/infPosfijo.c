/*
  infPosfijo.c
  Autor: Garcia Ambrosio Aldo (C) Abril 2025
         Ponce Espino Miguel Angel (C) Abril 2025
         Salinas Aquino Omar Ivan (C) Abril 2025
  Grupo: 2CM6
  Uso de IA: Claude 3.7 Sonnet
  Version: 2.2

  Programa que convierte expresiones aritméticas de notación infija a notación posfija
  y evalúa el resultado. Permite el uso de operadores aritméticos básicos (+, -, *, /, ^),
  paréntesis y valores numéricos representados por letras.

  Observaciones: Este programa utiliza una estructura de pila dinámica para realizar
  tanto la conversión de la notación infija a posfija como la evaluación de la expresión.
  Las letras en la expresión son reemplazadas por valores numéricos predefinidos.

  Compilación: Windows / Linux: gcc infPosfijo.c Recursos/pila_estatica/pila_est.c -o infPosfijo
                                gcc infPosfijo.c Recursos/pila_dinamica/pila_din.c -o infPosfijo

  Ejecución: ./infPosfijo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// #include "Recursos/pila_dinamica/pila_din.h"
#include "Recursos/pila_estatica/pila_est.h"

// Prototipo de funciones
int obtenerLetrasUnicas(char *cadena, char *letras_vistas);
char *conversionNumeros(char *cadena, float *numeros, char *letras, int cant_letras);
void validacionParentesis(char *expresion, pila *mi_pila, elemento e1);
int precedencia(char operador);
char *Posfijo(char *expresion, pila *mi_pila, elemento e1);
int validarPosfija(char *expresion_posfija);
float evaluarPosfija(char *expresion_posfija, pila *mi_pila, elemento e1);

/*
int main()
Función principal que coordina la ejecución del programa.
Recibe: Ningún parámetro.
Devuelve: 0 si el programa termina correctamente.
Observaciones: Esta función solicita una expresión aritmética al usuario, la convierte
a valores numéricos, valida los paréntesis, convierte la expresión a notación posfija,
evalúa el resultado y muestra la información en cada paso del proceso.
*/
int main()
{
    // Declaración de una pila que almacenará los paréntesis de apertura
    pila mi_pila;
    // Elemento auxiliar para operaciones de la pila
    elemento e1;
    // Arreglo para almacenar la expresion ingresada (máximo 100 caracteres + '\0')
    char cadena[101];
    // Arreglo de letras unicas en la expresion
    char letras_vistas[26];
    char *expresion_numerica;
    char *expresion_posfija;
    char *expresion_posfija_Letras;
    float resultado;
    int cant_letras;

    // Inicialización de la pila
    Initialize(&mi_pila);

    printf("Ingresa la expresion a validar, por ejemplo (A+B)*(C+D): ");
    // Lee la expresion desde la entrada estándar
    fgets(cadena, sizeof(cadena), stdin);
    cadena[strcspn(cadena, "\n")] = '\0'; // Elimina salto de línea

    // Validar que la cadena tenga una longitud maxima de 100 caracteres
    if (strlen(cadena) > 100)
    {
        printf("Error: La expresión excede los 100 caracteres permitidos.\n");
        exit(1);
    }

    // Primero validamos paréntesis con la expresión original
    validacionParentesis(cadena, &mi_pila, e1);

    // Contamos cuántas letras diferentes hay en la expresión. Máximo 26 letras distintas
    cant_letras = obtenerLetrasUnicas(cadena, letras_vistas);

    expresion_posfija_Letras = Posfijo(cadena, &mi_pila, e1);

    if (validarPosfija(expresion_posfija_Letras) == 0)
    {
        // Reservar memoria para los arreglos de valores
        float *numeros = malloc(cant_letras * sizeof(float));
        if (numeros == NULL)
        {
            printf("Error: No se pudo asignar memoria para los valores\n");
            exit(1);
        }

        // Pedir al usuario que ingrese los valores para cada letra encontrada
        printf("\nSe encontraron %d letras diferentes en la expresion.\n", cant_letras);
        printf("Ingresa los %d valores numericos correspondientes (uno por uno):\n", cant_letras);

        for (int i = 0; i < cant_letras; i++)
        {
            printf("Valor de %c: ", letras_vistas[i] - 'a' + 'A'); // Imprime como mayúscula
            scanf("%f", &numeros[i]);
        }

        // Convierte la expresion a números
        expresion_numerica = conversionNumeros(cadena, numeros, letras_vistas, cant_letras);

        // Llama a la funcion para pasar de infijo a posfijo
        expresion_posfija = Posfijo(expresion_numerica, &mi_pila, e1);

        // Llama a la funcion para resolver la expresion posfija
        resultado = evaluarPosfija(expresion_posfija, &mi_pila, e1);

        // Muestra la expresion original y la nueva expresion con valores numéricos
        printf("\nExpresion original: %s", cadena);
        printf("\nExpresion con valores numericos: %s \n", expresion_numerica);
        printf("\nExpresion posfija antes de valores numericos: %s", expresion_posfija_Letras);
        printf("\nExpresion posfija: %s\n", expresion_posfija);
        printf("\nResultado: %.2f\n", resultado);
        if (numeros != NULL)
            free(numeros);
    }
    else
        printf("Expresion no valida ");
    // Liberar memoria
    if (expresion_numerica != NULL)
        free(expresion_numerica);
    if (expresion_posfija != NULL)
        free(expresion_posfija);
    if (expresion_posfija_Letras != NULL)
        free(expresion_posfija_Letras);
    Destroy(&mi_pila);

    return 0;
}

/*
int obtenerLetrasUnicas(char *cadena, char *letras_vistas)
Recibe: char *cadena como la expresión original ingresada por el usuario.
Devuelve: Un entero con la cantidad de letras diferentes encontradas.
Observaciones: Esta función cuenta cuántas letras diferentes hay en la expresión,
para saber cuántos valores numéricos hay que solicitar al usuario.
*/
int obtenerLetrasUnicas(char *cadena, char *letras_vistas)
{
    int letras_encontradas[26] = {0}; // Para marcar las letras encontradas (solo letras a-z)
    int i, cont = 0;

    for (i = 0; cadena[i] != '\0'; i++)
    {
        if (isalpha(cadena[i]))
        {
            char letra = tolower(cadena[i]); // Convertir a minúscula
            int indice = letra - 'a';        // índice 0-25
            if (!letras_encontradas[indice]) // Registra la letra encontrada si no ha sido agregada
            {
                letras_encontradas[indice] = 1; // Marca como registrado
                letras_vistas[cont++] = letra;  // Cuenta la cantidad de letras unicas y las agrega al arreglo letras
            }
        }
    }

    return cont;
}

/*
char *conversionNumeros(char *cadena, float *numeros, char *letras_vistas, int cant_letras)
Recibe: char *cadena como la expresión original ingresada por el usuario y
int *numeros como un arreglo de valores numéricos a asignar a cada letra.
Devuelve: Un puntero char* a la nueva cadena con valores numéricos.
Observaciones: Convierte letras en la expresión a sus valores numéricos correspondientes.
*/
char *conversionNumeros(char *cadena, float *numeros, char *letras_vistas, int cant_letras)
{
    int i, indice_numeros, indice_letra, pos;
    char letra;
    pos = 0; // Inicializa la posición para la nueva cadena
    int longitud_cadena = strlen(cadena);
    // Asignar espacio para la nueva cadena (cada letra podría convertirse en varios dígitos)
    char *neoCadena = malloc(longitud_cadena * 15); // Espacio extra por si los numeros son muy grandes
    if (neoCadena == NULL)
    {
        perror("Error: No se pudo asignar memoria");
        exit(1);
    }

    for (i = 0; cadena[i] != '\0'; i++)
    {

        if (isalpha(cadena[i]))
        {
            letra = tolower(cadena[i]);
            for (indice_numeros = 0; indice_numeros < cant_letras; indice_numeros++)
            {
                if (letras_vistas[indice_numeros] == letra)
                {
                    // Imprimimos el valor asignado
                    pos += sprintf(neoCadena + pos, "%.2f ", numeros[indice_numeros]);
                    break; // salir al encontrar la coincidencia
                }
            }
        }
        else if (cadena[i] == '(' || cadena[i] == ')')
        {
            // Paréntesis se agregan con espacio
            pos += sprintf(neoCadena + pos, "%c ", cadena[i]);
        }
        else if (cadena[i] == '+' || cadena[i] == '-' || cadena[i] == '*' ||
                 cadena[i] == '/' || cadena[i] == '^')
        {
            // Operadores también separados por espacio
            pos += sprintf(neoCadena + pos, "%c ", cadena[i]);
        }
    }

    // Eliminar último espacio extra si existe
    if (pos > 0 && neoCadena[pos - 1] == ' ')
    {
        neoCadena[pos - 1] = '\0';
    }
    else
    {
        neoCadena[pos] = '\0';
    }
    return neoCadena;
}

/*
void validacionParentesis(char *expresion, pila *mi_pila, elemento e1)
Recibe: char *expresion como la expresión a validar,
pila *mi_pila como una estructura de pila para verificar el balance de paréntesis, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: void (No retorna valor explícito)
Observaciones: Esta función verifica el balance de paréntesis en una expresión aritmética.
*/
void validacionParentesis(char *expresion, pila *mi_pila, elemento e1)
{
    // Variable de índice para recorrer la cadena
    int i, j, tam_cadena;
    i = 0, j = 0;
    // Obtiene la longitud de la cadena ingresada
    tam_cadena = strlen(expresion);

    // Eliminar espacios en la misma cadena
    for (i = 0; i < tam_cadena; i++)
    {
        if (expresion[i] != ' ')
        {
            expresion[j++] = expresion[i];
        }
    }
    expresion[j] = '\0'; // Termina la cadena modificada

    // Limpiar la pila primero (por si acaso)
    while (!Empty(mi_pila))
    {
        e1 = Pop(mi_pila);
    }

    // Recorre cada carácter de la cadena para verificar los paréntesis
    for (i = 0; i < j; i++)
    {
        if (expresion[i] == '(')
        {
            // Si encuentra un paréntesis de apertura, lo guarda en la pila
            e1.simbolo = '(';
            Push(mi_pila, e1);
        }
        else if (expresion[i] == ')')
        {
            // Si encuentra un paréntesis de cierre, verifica que exista su correspondiente apertura
            if (Empty(mi_pila))
            {
                // Si la pila está vacía, significa que hay un cierre sin apertura previa
                printf("\nExpresion no valida, intentas cerrar parentesis que no aperturaron");
                exit(1); // Termina el programa con código de error
            }
            else
            {
                // Si hay elementos en la pila, elimina uno (empareja apertura con cierre)
                e1 = Pop(mi_pila);
            }
        }
        // Ignora cualquier otro carácter que no sea paréntesis
    }

    // Al finalizar el recorrido, verifica si quedaron paréntesis sin cerrar
    if (Empty(mi_pila))
    {
        // Si la pila está vacía, todos los paréntesis están balanceados
        printf("\nExpresion con parentesis valida");
    }
    else
    {
        // Si la pila NO está vacía, quedaron paréntesis de apertura sin su correspondiente cierre
        printf("\nExpresion no valida, hay parentesis que no han cerrado");
        exit(1);
    }

    // Reinicializar la pila
    Destroy(mi_pila);
    Initialize(mi_pila);
}

/*
int precedencia(char operador)
Recibe: char operador como el símbolo del operador a evaluar.
Devuelve: Entero que representa el nivel de precedencia del operador.
Observaciones: Define la precedencia de los operadores aritméticos siguiendo
las reglas matemáticas convencionales. Mayor número significa mayor precedencia.
*/
int precedencia(char operador)
{
    switch (operador)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return -1; // Para paréntesis u otros caracteres
    }
}

/*
char *Posfijo(char *expresion, pila *mi_pila, elemento e1)
Recibe: char *expresion_numerica como la expresión infija con valores numéricos,
pila *mi_pila como una estructura de pila para la conversión, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: Un puntero char* a la expresión en notación posfija resultante.
Observaciones: Implementa el algoritmo Shunting Yard para convertir una expresión
en notación infija a notación posfija.
*/
char *Posfijo(char *expresion, pila *mi_pila, elemento e1)
{
    // Variable de índice para recorrer la cadena
    int i, j = 0;
    int longitud = strlen(expresion);
    char c; // Variable para almacenar el carácter actual
    // Arreglo para almacenar la expresion posfija
    char *cadenaPos = malloc(longitud * sizeof(char));

    if (cadenaPos == NULL)
    {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }

    // Limpiar la pila
    while (!Empty(mi_pila))
    {
        e1 = Pop(mi_pila);
    }

    // Recorre cada carácter de la cadena
    for (i = 0; i < longitud; i++)
    {
        // Si es una letra (operando alfabético)
        if (isalpha(expresion[i]))
        {
            cadenaPos[j++] = expresion[i];
            cadenaPos[j++] = ' ';
        }
        // Si es un número (con posible signo negativo y punto decimal)
        else if (isdigit(expresion[i]) ||
                 (expresion[i] == '-' && i + 1 < longitud && isdigit(expresion[i + 1])))
        {
            cadenaPos[j++] = expresion[i++];
            while (i < longitud && (isdigit(expresion[i]) || expresion[i] == '.'))
            {
                cadenaPos[j++] = expresion[i++];
            }
            cadenaPos[j++] = ' ';
            i--; // para compensar el for
        }
        // Si es un paréntesis de apertura, lo añadimos a la pila
        else if (expresion[i] == '(')
        {
            e1.simbolo = '(';
            Push(mi_pila, e1);
        }
        // Si es un paréntesis de cierre, sacamos operadores hasta encontrar el paréntesis de apertura
        else if (expresion[i] == ')')
        {
            while (!Empty(mi_pila) && Top(mi_pila).simbolo != '(')
            {
                e1 = Pop(mi_pila);
                cadenaPos[j++] = e1.simbolo;
                cadenaPos[j++] = ' '; // Espacio para separar operadores
            }

            if (!Empty(mi_pila) && Top(mi_pila).simbolo == '(')
            {
                e1 = Pop(mi_pila); // Descartar el paréntesis de apertura
            }
            else
            {
                printf("\nExpresion no valida, intentas cerrar expresiones que no aperturaron");
                free(cadenaPos);
                exit(1);
            }
        }
        // Si es un operador
        else if (expresion[i] == '+' || expresion[i] == '-' ||
                 expresion[i] == '*' || expresion[i] == '/' ||
                 expresion[i] == '^')
        {

            // Mientras haya operadores en la pila con mayor o igual precedencia
            while (!Empty(mi_pila) && Top(mi_pila).simbolo != '(' &&
                   precedencia(expresion[i]) <= precedencia(Top(mi_pila).simbolo))
            {
                e1 = Pop(mi_pila);
                cadenaPos[j++] = e1.simbolo;
                cadenaPos[j++] = ' '; // Espacio para separar operadores
            }

            // Agregar el operador actual a la pila
            e1.simbolo = expresion[i];
            Push(mi_pila, e1);
        }
        // Ignorar otros caracteres (espacios, etc.)
    }

    // Sacar los operadores restantes de la pila
    while (!Empty(mi_pila))
    {
        if (Top(mi_pila).simbolo == '(')
        {
            printf("\nExpresion no valida, hay paréntesis sin cerrar");
            free(cadenaPos);
            exit(1);
        }
        e1 = Pop(mi_pila);
        cadenaPos[j++] = e1.simbolo;
        cadenaPos[j++] = ' '; // Espacio para separar operadores
    }

    // Si hay elementos en la cadena, eliminamos el último espacio
    if (j > 0 && cadenaPos[j - 1] == ' ')
    {
        j--;
    }

    // Terminar la cadena con el carácter nulo
    cadenaPos[j] = '\0';

    // Reinicializar la pila para su uso posterior
    Destroy(mi_pila);
    Initialize(mi_pila);

    return cadenaPos;
}

/*
int validarPosfija(char *expresion_posfija)
Recibe: char *expresion_posfija como la expresión en notación posfija a verificar.
Devuelve: 0 si la expresión es válida, 1 si no lo es.
Observaciones: Valida sintácticamente una expresión en notación posfija con letras como operandos.
*/
int validarPosfija(char *expresion_posfija)
{
    int contador = 0;
    char *token;

    // Duplicamos la cadena para no modificar la original
    char *copia = strdup(expresion_posfija);
    if (copia == NULL)
    {
        printf("Error: No se pudo asignar memoria\n");
        return 1;
    }

    // Tokenizamos la cadena usando espacios como delimitadores
    token = strtok(copia, " ");

    while (token != NULL)
    {
        // Si el token es una letra única (operando válido)
        if (isalpha(token[0]) && strlen(token) == 1)
        {
            contador++;
        }
        // Si es un operador válido y de un solo carácter
        else if ((token[0] == '+' || token[0] == '-' || token[0] == '*' ||
                  token[0] == '/' || token[0] == '^') &&
                 strlen(token) == 1)
        {
            if (contador < 2)
            {
                printf("\nError: Expresión posfija mal formada, faltan operandos para el operador '%c'\n", token[0]);
                if (copia != NULL)
                    free(copia);
                return 1;
            }
            contador--; // Se combinan dos operandos en uno
        }
        else
        {
            printf("\nError: Token inválido '%s'\n", token);
            if (copia != NULL)
                free(copia);
            return 1;
        }

        // Avanzamos al siguiente token
        token = strtok(NULL, " ");
    }
    if (copia != NULL)
        free(copia);

    // Al final debe quedar exactamente un resultado
    if (contador != 1)
    {
        printf("\nError: Expresión posfija mal formada, sobran o faltan operandos\n");
        return 1;
    }

    return 0;
}

/*
float evaluarPosfija(char *expresion_posfija, pila *mi_pila, elemento e1)
Recibe: char *expresion_posfija como la expresión en notación posfija a evaluar,
pila *mi_pila como una estructura de pila para el cálculo, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: Flotante con el resultado de la evaluación de la expresión.
Observaciones: Evalúa una expresión en notación posfija utilizando una pila.
               Se asume que la expresión ya fue validada.
*/
float evaluarPosfija(char *expresion_posfija, pila *mi_pila, elemento e1)
{
    int i;
    float num, op1, op2, resultado;
    char *token;

    // Limpiar la pila
    while (!Empty(mi_pila))
    {
        e1 = Pop(mi_pila);
    }

    // Duplicamos la cadena para no modificar la original
    char *copia = strdup(expresion_posfija);
    if (copia == NULL)
    {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }

    // Tokenizamos la cadena usando espacios como delimitadores
    token = strtok(copia, " ");

    while (token != NULL)
    {
        // Si es un número, lo convertimos y lo metemos a la pila. Acepta números negativos
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        {
            num = atof(token);
            e1.numero = num;
            Push(mi_pila, e1);
        }
        // Si es un operador, sacamos dos operandos de la pila y realizamos la operación
        else if (token[0] == '+' || token[0] == '-' || token[0] == '*' ||
                 token[0] == '/' || token[0] == '^')
        {
            // Sacamos los operandos
            e1 = Pop(mi_pila);
            op2 = e1.numero;
            e1 = Pop(mi_pila);
            op1 = e1.numero;

            // Realizamos la operación correspondiente
            switch (token[0])
            {
            case '+':
                resultado = op1 + op2;
                break;
            case '-':
                resultado = op1 - op2;
                break;
            case '*':
                resultado = op1 * op2;
                break;
            case '/':
                if (op2 == 0)
                {
                    printf("\nError: División por cero\n");
                    free(copia);
                    return -1;
                }
                resultado = op1 / op2;
                break;
            case '^':
                resultado = pow(op1, op2);
                break;
            }

            // Metemos el resultado de vuelta a la pila
            e1.numero = resultado;
            Push(mi_pila, e1);
        }

        // Avanzamos al siguiente token
        token = strtok(NULL, " ");
    }

    // Obtenemos el resultado final
    e1 = Pop(mi_pila);
    resultado = e1.numero;

    // Liberamos la memoria de la copia
    if (copia != NULL)
        free(copia);

    return resultado;
}
