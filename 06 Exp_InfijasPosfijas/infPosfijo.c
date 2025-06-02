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
  
  Ejecución: ./infPosfijo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Recursos/pila_dinamica/pila_din.h"

// Prototipo de funciones
char* coversionNumeros(char *cadena, int *numeros);
void validacionParentesis(char *expresion_numerica, pila *mi_pila, elemento e1);
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
	int i;
    // Declaración de una pila que almacenará los paréntesis de apertura
	pila mi_pila;          
    // Elemento auxiliar para operaciones de la pila
	elemento e1;           
	// Arreglo para almacenar la expresion ingresada (máximo 199 caracteres + '\0')
    char cadena[200];
	char *expresion_numerica;
    char *expresion_posfija;
    int resultado;
	// Arreglo de valores enteros que corresponden a las letras en la expresión
    unsigned int numeros[12] = {10, 20, 30, 40, 50, 60, 70, 80, 1, 2, 4, 1};
    
    // Inicialización de la pila 
	Initialize(&mi_pila);  
    
	printf("Ingresa la expresion a validar, por ejemplo (a+b)*(c+d) o (a+b)*(c+d): ");
	// Lee la expresion desde la entrada estándar
    scanf("%s", cadena);  
	
	// Convierte la expresion a números
	expresion_numerica = coversionNumeros(cadena, numeros);
    	
	// Llama a la función para validar paréntesis
	validacionParentesis(expresion_numerica, &mi_pila, e1);  
	// Llama a la funcion para pasar de infijo a posfijo
    expresion_posfija = Posfijo(expresion_numerica, &mi_pila, e1);
	// Llama a la funcion para resolver la expresion posfija
    resultado = solucionPosfijo(expresion_posfija, &mi_pila, e1);

    // Muestra la expresion original y la nueva expresion con valores numéricos
    printf("\nExpresion original: %s", cadena);
    printf("\nExpresion con valores numericos: %s\n", expresion_numerica);
    printf("\nExpresion posfija: %s\n", expresion_posfija);
    printf("\nResultado: %d\n", resultado);

	free(expresion_numerica); // Libera la memoria asignada para la expresion numérica
    free(expresion_posfija); // Libera la memoria asignada para la expresion posfija
    Destroy(&mi_pila);  // Libera la memoria utilizada por la pila
    return 0;          // Termina el programa con código de éxito
}

/*
char* coversionNumeros(char *cadena, int *numeros)
Recibe: char *cadena como la expresión original ingresada por el usuario y
int *numeros como un arreglo de valores numéricos a asignar a cada letra.
Devuelve: Un puntero char* a la nueva cadena con valores numéricos.
Observaciones: Convierte letras específicas en la expresión (A-H, X, Y, W, Z) a sus 
valores numéricos correspondientes. Las letras minúsculas son convertidas a mayúsculas
antes de realizar la asignación.
*/
char* coversionNumeros(char *cadena, int *numeros){
	int i, pos = 0; // Inicializa la posición para la nueva cadena
	char *neoCadena = malloc(sizeof(cadena) * sizeof(int));// Asigna espacio suficiente

	if (neoCadena == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }

	// Recorre la cadena original
    for (i = 0; i < strlen(cadena); i++) {
		// Convertir a mayúscula si es una letra minúscula
        char c = toupper(cadena[i]);
        
        // Manejar cada letra específica
        if (c >= 'A' && c <= 'H') {
            // A-H corresponden a los índices 0-7
            pos += sprintf(neoCadena + pos, "%d", numeros[c - 'A']);
        }
        else if (c == 'X') {
            pos += sprintf(neoCadena + pos, "%d", numeros[8]); // X es el índice 8
        }
        else if (c == 'Y') {
            pos += sprintf(neoCadena + pos, "%d", numeros[9]); // Y es el índice 9
        }
        else if (c == 'W') {
            pos += sprintf(neoCadena + pos, "%d", numeros[10]); // W es el índice 10
        }
        else if (c == 'Z') {
            pos += sprintf(neoCadena + pos, "%d", numeros[11]); // Z es el índice 11
        }
        else {
            // Cualquier otro carácter se copia tal cual
            neoCadena[pos++] = cadena[i];
        }
    }
    
    // Añade el carácter nulo al final
    neoCadena[pos] = '\0';
    
    return neoCadena;
}

/*
void validacionParentesis(char *expresion_numerica, pila *mi_pila, elemento e1)
Recibe: char *expresion_numerica como la expresión con valores numéricos,
pila *mi_pila como una estructura de pila para verificar el balance de paréntesis, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: void (No retorna valor explícito)
Observaciones: Esta función verifica el balance de paréntesis en una expresión aritmética.
Utiliza una pila para rastrear los paréntesis de apertura y comprobar si cada paréntesis 
de cierre corresponde a uno de apertura. Si la expresión es válida, lo informa; si no, 
muestra el error y termina el programa.
*/
void validacionParentesis(char *expresion_numerica, pila *mi_pila, elemento e1){
	// Variable de índice para recorrer la cadena
	int i, tam_cadena;  
	// Obtiene la longitud de la cadena ingresada
    tam_cadena = strlen(expresion_numerica); 
    
    // Recorre cada carácter de la cadena para verificar los paréntesis
    for(i = 0; i < tam_cadena; i++){
        if(expresion_numerica[i] == '(') {
            // Si encuentra un paréntesis de apertura, lo guarda en la pila
            Push(mi_pila, e1);  // El valor de e1 no importa, solo se usa la pila como contador
        }
        else if(expresion_numerica[i] == ')'){
            // Si encuentra un paréntesis de cierre, verifica que exista su correspondiente apertura
            if(Empty(mi_pila)){
                // Si la pila está vacía, significa que hay un cierre sin apertura previa
                printf("\nExpresion no valida, intentas cerrar expresiones que no aperturaron");
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
        printf("\nExpresion valida");
    }
    else{
        // Si la pila NO está vacía, quedaron paréntesis de apertura sin su correspondiente cierre
        printf("\nExpresion no valida, hay expresiones que no han cerrado");
    }
    
    // Muestra el tamaño final de la pila (debe ser 0 si la expresion es válida)
    // printf("\nTamanio de pila antes de salir(0 para valida) = %d", Size(mi_pila));
}

/*
int precedencia(char operador)
Recibe: char operador como el símbolo del operador a evaluar.
Devuelve: Entero que representa el nivel de precedencia del operador.
Observaciones: Define la precedencia de los operadores aritméticos siguiendo
las reglas matemáticas convencionales. Mayor número significa mayor precedencia.
- +, -: precedencia 1
- *, /: precedencia 2
- ^: precedencia 3
- Otros caracteres: -1 (para paréntesis u otros caracteres)
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
en notación infija a notación posfija. Utiliza una pila para gestionar operadores
según su precedencia. La expresión posfija resultante separa los elementos con espacios.
*/
char* Posfijo(char *expresion_numerica, pila *mi_pila, elemento e1){
	// Variable de índice para recorrer la cadena
	int i, j = 0;  
    char c; // Variable para almacenar el carácter actual
	char *cadenaPos = malloc(sizeof(expresion_numerica) * sizeof(int)); // Arreglo para almacenar la expresion posfija

    if (cadenaPos == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }

    // Recorre cada carácter de la cadena
    for (i = 0; i < strlen(expresion_numerica); i++) {
        // Si es un dígito (operando), se agrega directamente a la salida
        if (isdigit(expresion_numerica[i])) {
            // Si encontramos un número, copiamos todos sus dígitos
            while (i < strlen(expresion_numerica) && isdigit(expresion_numerica[i])) {
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
    Destroy(mi_pila); // Destruir la pila después de usarla
    Initialize(mi_pila); // Re-inicializar la pila para su uso posterior
    
    return cadenaPos;
}

/*
int solucionPosfijo(char *expresion_posfija, pila *mi_pila, elemento e1)
Recibe: char *expresion_posfija como la expresión en notación posfija a evaluar,
pila *mi_pila como una estructura de pila para el cálculo, y
elemento e1 como auxiliar para operaciones de la pila.
Devuelve: Entero con el resultado de la evaluación de la expresión.
Observaciones: Evalúa una expresión en notación posfija utilizando una pila.
Procesa la expresión de izquierda a derecha, añadiendo operandos a la pila
y aplicando operadores a los dos operandos superiores de la pila.
Maneja los operadores aritméticos básicos (+, -, *, /, ^) y verifica errores
como división por cero o expresión mal formada.
*/
int solucionPosfijo(char *expresion_posfija, pila *mi_pila, elemento e1) {
    int i, num, op1, op2, resultado;
    char *token;
    
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
    
    printf("\nResultado de la evaluación: %d\n", resultado);
    return resultado;
}