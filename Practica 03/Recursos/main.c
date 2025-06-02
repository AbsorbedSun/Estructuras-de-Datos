#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pila_dinamica/pila_din.h"
int main(){
	int i,tam_cadena;
	pila mi_pila;
	elemento e1;
	char cadena[200];
	
	Initialize(&mi_pila);
	
	scanf("%s",cadena);
	tam_cadena=strlen(cadena);
	
	for(i=0;i<tam_cadena;i++){
		if(cadena[i]=='(')
			Push(&mi_pila,e1);
		else if(cadena[i]==')'){
			if(Empty(&mi_pila)){
				printf("\nExpresión no valida, intentas cerrar expresiones que no aperturaron"),
				exit(1);
			}
			else{
				e1=Pop(&mi_pila);
			}
		}
	}
	if(Empty(&mi_pila)){
		printf("\nExpresión valida");
	}
	else{
		printf("\nExpresión no valida, hay expresiones que no han cerrado");
	}
	
	printf("Tamanio de pila antesde salir=%d",Size(&mi_pila));
	Destroy(&mi_pila);
	return 0;
}