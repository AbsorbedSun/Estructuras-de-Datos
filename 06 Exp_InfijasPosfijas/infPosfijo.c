/*  
  infPosfijo.c 
  Autor: Aldo Garcia Ambrosio (C) Mayo 2025
  Uso de IA: Claude 3.7 Sonnet
  Version: 2.2
  
  Programa que convierte expresiones aritméticas de notación infija a notación posfija
  y evalúa el resultado. Permite el uso de operadores aritméticos básicos (+, -, *, /, ^),
  paréntesis y valores numéricos representados por letras.
  
  Observaciones: Este programa utiliza una estructura de pila dinámica para realizar
  tanto la conversión de la notación infija a posfija como la evaluación de la expresión.
  Las letras en la expresión son reemplazadas por valores numéricos predefinidos.
  
  Compilación: Windows / Linux: gcc infPosfijo.c Recursos/pila_dinamica/pila_din.c -o infPosfijo
                                gcc infPosfijo.c Recursos/pila_dinamica/pila_din.c -o infPosfijo
  
  Ejecución: ./infPosfijo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "Recursos/pila_dinamica/pila_din.h"
#include "Recursos/pila_estatica/pila_est.h"

// Prototipo de funciones
int contarLetras(char *cadena);
char* coversionNumeros(char *cadena, int *numeros);
void validacionParentesis(char *expresion, pila *mi_pila, elemento e1);
int precedencia(char operador);
char* Posfijo(char *expresion_numerica, pila *mi_pila, elemento e1);
int solucionPosfijo(char *expresion_posfija, pila *mi_pila, elemento e1);

/*
int main()
Función principal que coordina la ejecución del programa.
Recibe: Ningún parámetro.
Devuelve: 0 si el programa termina correctamente.
Observaciones: Esta función solicita una expresión aritmética al usuario, la convierte
a valores numéricos, valida los paréntesis, convierte la expresión a notación posfija,
evalúa el resultado y muestra la información en cada paso del proceso.
*/
int main(){ 
    // Declaración de una pila que almacenará los paréntesis de apertura
    pila mi_pila;          
    // Elemento auxiliar para operaciones de la pila
    elemento e1;           
    // Arreglo para almacenar la expresion ingresada (máximo 199 caracteres + '\0')
    char cadena[100];
    char *expresion_numerica;
    char *expresion_posfija;
    int resultado;
    int cant_letras;
    
    // Inicialización de la pila 
    Initialize(&mi_pila);  
    
    printf("Ingresa la expresion a validar, por ejemplo (a+b)*(c+d): ");
    // Lee la expresion desde la entrada estándar
    scanf("%s", cadena);  
    
    // Primero validamos paréntesis con la expresión original
    validacionParentesis(cadena, &mi_pila, e1);
    
    // Contamos cuántas letras diferentes hay en la expresión
    cant_letras = contarLetras(cadena);
    
    // Reservar memoria para los arreglos de valores
    int *numeros = malloc(cant_letras * sizeof(int));
    if (numeros == NULL) {
        printf("Error: No se pudo asignar memoria para los valores\n");
        exit(1);
    }
    
    // Pedir al usuario que ingrese los valores para cada letra encontrada
    printf("\nSe encontraron %d letras diferentes en la expresion.\n", cant_letras);
    printf("Ingresa los %d valores numericos correspondientes (uno por uno):\n", cant_letras);
    
    for (int i = 0; i < cant_letras; i++) {
        printf("Valor %d: ", i+1);
        scanf("%d", &numeros[i]); // Lee el valor numérico a asignar a las letras
    }
    
    // Convierte la expresion a números
    expresion_numerica = coversionNumeros(cadena, numeros);
    
    // Llama a la funcion para pasar de infijo a posfijo
    expresion_posfija = Posfijo(expresion_numerica, &mi_pila, e1);
    
    // Llama a la funcion para resolver la expresion posfija
    resultado = solucionPosfijo(expresion_posfija, &mi_pila, e1);

    // Muestra la expresion original y la nueva expresion con valores numéricos
    printf("\nExpresion original: %s", cadena);
    printf("\nExpresion con valores numericos: %s", expresion_numerica);
    printf("\nExpresion posfija: %s", expresion_posfija);
    printf("\nResultado: %d\n", resultado);

    // Liberar memoria
    free(numeros);
    free(expresion_numerica);
    free(expresion_posfija);
    Destroy(&mi_pila);
    
    return 0;
}

/*
int contarLetras(char *cadena)
Recibe: char *cadena como la expresión original ingresada por el usuario.
Devuelve: Un entero con la cantidad de letras diferentes encontradas.
Observaciones: Esta función cuenta cuántas letras diferentes hay en la expresión,
para saber cuántos valores numéricos hay que solicitar al usuario.
*/
int contarLetras(char *cadena) {
    int letras_encontradas[26] = {0}; // Para marcar las letras encontradas (solo letras a-z)
    int i, cont = 0;
    
    for (i = 0; i < strlen(cadena); i++) {
        if (isalpha(cadena[i])) {
            char letra = tolower(cadena[i]) - 'a'; // Convertir a minúscula e índice 0-25
            if (letra >= 0 && letra < 26 && letras_encontradas[letra] == 0) {
                letras_encontradas[letra] = 1; // Marcar como encontrada
                cont++;
            }
        }
    }
    
    return cont;
}

/*
char* coversionNumeros(char *cadena, int *numeros)
Recibe: char *cadena como la expresión original ingresada por el usuario y
int *numeros como un arreglo de valores numéricos a asignar a cada letra.
Devuelve: Un puntero char* a la nueva cadena con valores numéricos.
Observaciones: Convierte letras en la expresión a sus valores numéricos correspondientes.
*/
char* coversionNumeros(char *cadena, int *numeros){
    int i, pos = 0; // Inicializa la posición para la nueva cadena
    int longitud_cadena = strlen(cadena);
    // Asignar espacio para la nueva cadena (cada letra podría convertirse en varios dígitos)
    char *neoCadena = malloc(longitud_cadena * sizeof(char));
    if (neoCadena == NULL) {
        perror("Error: No se pudo asignar memoria");
        exit(1);
    }

    // Mapa de letras a valores; -1 significa "aún no asignado"
    int letras_vistas[26];
    for (i = 0; i < 26; i++) {
        letras_vistas[i] = -1;
    }

    int indice_numeros = 0;  // cuántos valores de 'numeros[]' ya usamos

    for (i = 0; cadena[i] != '\0'; i++) {
        if (isalpha(cadena[i])) {
            int indice_letra = tolower(cadena[i]) - 'a';
            // Si es la primera vez que vemos esta letra, le asignamos un número
            if (letras_vistas[indice_letra] < 0) {
                letras_vistas[indice_letra] = numeros[indice_numeros++];
            }
            // Imprimimos el valor asignado
            pos += sprintf(neoCadena + pos, "%d", letras_vistas[indice_letra]);
        } else {
            // Copia paréntesis, operadores, dígitos, etc.
            neoCadena[pos++] = cadena[i];
        }
    }

    neoCadena[pos] = '\0';
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
void validacionParentesis(char *expresion, pila *mi_pila, elemento e1){
    // Variable de índice para recorrer la cadena
    int i, tam_cadena;  
    // Obtiene la longitud de la cadena ingresada
    tam_cadena = strlen(expresion); 
    
    // Limpiar la pila primero (por si acaso)
    while (!Empty(mi_pila)) {
        e1 = Pop(mi_pila);
    }
    
    // Recorre cada carácter de la cadena para verificar los paréntesis
    for(i = 0; i < tam_cadena; i++){
        if(expresion[i] == '(') {
            // Si encuentra un paréntesis de apertura, lo guarda en la pila
            e1.simbolo = '(';
            Push(mi_pila, e1);
        }
        else if(expresion[i] == ')'){
            // Si encuentra un paréntesis de cierre, verifica que exista su correspondiente apertura
            if(Empty(mi_pila)){
                // Si la pila está vacía, significa que hay un cierre sin apertura previa
                printf("\nExpresion no valida, intentas cerrar parentesis que no aperturaron");
                exit(1);  // Termina el programa con código de error
            }
            else{
                // Si hay elementos en la pila, elimina uno (empareja apertura con cierre)
                e1 = Pop(mi_pila);
            }
        }
        // Ignora cualquier otro carácter que no sea paréntesis
    }
    
    // Al finalizar el recorrido, verifica si quedaron paréntesis sin cerrar
    if(Empty(mi_pila)){
        // Si la pila está vacía, todos los paréntesis están balanceados
        printf("\nExpresion con parentesis valida");
    }
    else{
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
int precedencia(char operador) {
    switch (operador) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return -1;  // Para paréntesis u otros caracteres
    }
}

/*
char* Posfijo(char *expresion_numerica, pila *mi_pila, elemento e1)
Recibe: char *expresion_numerica como la expresión infija con valores numéricos,
pila *mi_pila como una estructura de pila para la conversión, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: Un puntero char* a la expresión en notación posfija resultante.
Observaciones: Implementa el algoritmo Shunting Yard para convertir una expresión
en notación infija a notación posfija.
*/
char* Posfijo(char *expresion_numerica, pila *mi_pila, elemento e1){
    // Variable de índice para recorrer la cadena
    int i, j = 0;  
    int longitud = strlen(expresion_numerica);
    char c; // Variable para almacenar el carácter actual
    // Arreglo para almacenar la expresion posfija
    char *cadenaPos = malloc(longitud * sizeof(char));

    if (cadenaPos == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }

    // Limpiar la pila
    while (!Empty(mi_pila)) {
        e1 = Pop(mi_pila);
    }

    // Recorre cada carácter de la cadena
    for (i = 0; i < longitud; i++) {
        // Si es un dígito (operando), se agrega directamente a la salida
        if (isdigit(expresion_numerica[i])) {
            // Si encontramos un número, copiamos todos sus dígitos
            while (i < longitud && isdigit(expresion_numerica[i])) {
                cadenaPos[j++] = expresion_numerica[i++];
            }
            // Añadir un espacio para separar los números
            cadenaPos[j++] = ' ';
            // Decrementar i para compensar el incremento del for
            i--;
        }
        // Si es un paréntesis de apertura, lo añadimos a la pila
        else if (expresion_numerica[i] == '(') {
            e1.simbolo = '(';
            Push(mi_pila, e1);
        }
        // Si es un paréntesis de cierre, sacamos operadores hasta encontrar el paréntesis de apertura
        else if (expresion_numerica[i] == ')') {
            while (!Empty(mi_pila) && Top(mi_pila).simbolo != '(') {
                e1 = Pop(mi_pila);
                cadenaPos[j++] = e1.simbolo;
                cadenaPos[j++] = ' '; // Espacio para separar operadores
            }
            
            if (!Empty(mi_pila) && Top(mi_pila).simbolo == '(') {
                e1 = Pop(mi_pila); // Descartar el paréntesis de apertura
            } else {
                printf("\nExpresion no valida, intentas cerrar expresiones que no aperturaron");
                free(cadenaPos);
                exit(1);
            }
        }
        // Si es un operador
        else if (expresion_numerica[i] == '+' || expresion_numerica[i] == '-' || 
                 expresion_numerica[i] == '*' || expresion_numerica[i] == '/' || 
                 expresion_numerica[i] == '^') {
            
            // Mientras haya operadores en la pila con mayor o igual precedencia
            while (!Empty(mi_pila) && Top(mi_pila).simbolo != '(' && 
                  precedencia(expresion_numerica[i]) <= precedencia(Top(mi_pila).simbolo)) {
                e1 = Pop(mi_pila);
                cadenaPos[j++] = e1.simbolo;
                cadenaPos[j++] = ' '; // Espacio para separar operadores
            }
            
            // Agregar el operador actual a la pila
            e1.simbolo = expresion_numerica[i];
            Push(mi_pila, e1);
        }
        // Ignorar otros caracteres (espacios, etc.)
    }
    
    // Sacar los operadores restantes de la pila
    while (!Empty(mi_pila)) {
        if (Top(mi_pila).simbolo == '(') {
            printf("\nExpresion no valida, hay paréntesis sin cerrar");
            free(cadenaPos);
            exit(1);
        }
        e1 = Pop(mi_pila);
        cadenaPos[j++] = e1.simbolo;
        cadenaPos[j++] = ' '; // Espacio para separar operadores
    }
    
    // Si hay elementos en la cadena, eliminamos el último espacio
    if (j > 0 && cadenaPos[j-1] == ' ') {
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
int solucionPosfijo(char *expresion_posfija, pila *mi_pila, elemento e1)
Recibe: char *expresion_posfija como la expresión en notación posfija a evaluar,
pila *mi_pila como una estructura de pila para el cálculo, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: Entero con el resultado de la evaluación de la expresión.
Observaciones: Evalúa una expresión en notación posfija utilizando una pila.
*/
int solucionPosfijo(char *expresion_posfija, pila *mi_pila, elemento e1) {
    int i, num, op1, op2, resultado;
    char *token;
    
    // Limpiar la pila
    while (!Empty(mi_pila)) {
        e1 = Pop(mi_pila);
    }
    
    // Duplicamos la cadena para no modificar la original
    char *copia = strdup(expresion_posfija);
    if (copia == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    
    // Tokenizamos la cadena usando espacios como delimitadores
    token = strtok(copia, " ");
    
    while (token != NULL) {
        // Si es un número, lo convertimos y lo metemos a la pila
        if (isdigit(token[0])) {
            num = atoi(token);
            e1.numero = num;
            Push(mi_pila, e1);
        }
        // Si es un operador, sacamos dos operandos de la pila y realizamos la operación
        else if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/' || token[0] == '^') {
            // Verificamos que haya al menos dos operandos en la pila
            if (Size(mi_pila) < 2) {
                printf("\nError: Expresión posfija mal formada, faltan operandos\n");
                free(copia);
                return -1;
            }
            
            // Sacamos los operandos
            e1 = Pop(mi_pila);
            op2 = e1.numero;
            e1 = Pop(mi_pila);
            op1 = e1.numero;
            
            // Realizamos la operación correspondiente
            switch (token[0]) {
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
                    if (op2 == 0) {
                        printf("\nError: División por cero\n");
                        free(copia);
                        return -1;
                    }
                    resultado = op1 / op2;
                    break;
                case '^':
                    resultado = 1;
                    for (i = 0; i < op2; i++) {
                        resultado *= op1;
                    }
                    break;
            }
            
            // Metemos el resultado de vuelta a la pila
            e1.numero = resultado;
            Push(mi_pila, e1);
        }
        
        // Avanzamos al siguiente token
        token = strtok(NULL, " ");
    }
    
    // Al final, debe quedar solo un elemento en la pila (el resultado)
    if (Size(mi_pila) != 1) {
        printf("\nError: Expresión posfija mal formada, sobran operandos\n");
        free(copia);
        return -1;
    }
    
    // Obtenemos el resultado final
    e1 = Pop(mi_pila);
    resultado = e1.numero;
    
    // Liberamos la memoria de la copia
    free(copia);
    
    return resultado;
}