#include "alu.h"

int32_t Aritmetic_Shifter(int32_t K, int n) {
    int32_t X = K;
    int i;
    
    X = X >> 31;
    
    if (X != 0)
        for (i = 1; i<n ;i++) {
            K = K >> 1;
            K = K & 0x80000000;
        }
    
    else
        K >> n;
}