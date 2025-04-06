#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void combinar(int n, char simbolos[]);

int main(int num_arg, char *cadenas[]){
    if (num_arg != 2)
    {
        exit(1);
    };
    int n = atoi(cadenas[1]);
    char simbolos[n];
    strcpy(simbolos, "");
    combinar(n, simbolos);
    return 0;
};

void combinar(int n, char simbolos[]){
    if (n == 0)
    {
        printf("%s\n", simbolos);
    }
    else
    {
        char simbtemp1[n], simbtemp2[n];
        strcpy(simbtemp1, simbolos);
        strcpy(simbtemp2, simbolos);
        strcat(simbolos, "a");
        combinar(n - 1, simbolos);
        strcat(simbtemp1, "b");
        combinar(n - 1, simbtemp1);
        strcat(simbtemp2, "c");
        combinar(n - 1, simbtemp2);
    }
}