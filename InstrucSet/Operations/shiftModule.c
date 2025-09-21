#include "shiftModule.h"

int shift_SAR(cpu_t *cpu, int32_t value, int32_t num_bits) {
    // shift aritmetico a la derecha
    int32_t result = value;
    
    if(result & 0x00800000){
        result = result | 0xFF000000;
    }
    
    
    if (num_bits >= 32) {
        result = (value < 0) ? -1 : 0;
    } else {
        for (int i = 0; i < num_bits; i++) {
            if(result & 0x80000000){
                result = (result >> 1);
                result = result | 0x80000000;
            }
            else
                result = (result >> 1);

            
        }
    } 
   if(result & 0x80000000)
        result = result & 0x00FFFFFF;
   
    return result;
}

int shift_SHR(cpu_t *cpu, int value, int num_bits) {
    // shift logico a la derecha
    int32_t result = value;
    if (num_bits >= 32) {
        result = 0; // relleno con ceros
    } else {
        for (int i = 0; i < num_bits; i++) {
            result >>= 1;
        }
    } 
    return result;
}

int shift_SHL(cpu_t *cpu, int value, int num_bits) {
    // shift logico a la izquierda
    int32_t result = value;
    if (num_bits >= 32) {
        result = 0; // relleno con ceros
    } else {
        for (int i = 0; i < num_bits; i++) {
            result <<= 1;
        }
    } 
    return result;
}