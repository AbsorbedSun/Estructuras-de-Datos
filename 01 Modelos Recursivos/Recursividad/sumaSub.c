#include <stdio.h>
#include <stdbool.h>

bool SumSub(int C[], int i, int k) {
    if (k == 0)
        return true;
    if (i < 0)
        return false;
    
    if (C[i] <= k) {
        bool izq = SumSub(C, i - 1, k - C[i]);
        bool der = SumSub(C, i - 1, k);
        return izq || der;
    }
    
    return SumSub(C, i - 1, k);
}

int main() {
    int C[] = {3, 34, 4, 12, 5, 2};
    int k = 9;
    int n = sizeof(C) / sizeof(C[0]);
    
    if (SumSub(C, n - 1, k))
        printf("Existe un subconjunto con suma %d\n", k);
    else
        printf("No existe un subconjunto con suma %d\n", k);
    
    return 0;
}