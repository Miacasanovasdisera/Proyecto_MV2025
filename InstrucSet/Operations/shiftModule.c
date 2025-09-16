#include "shiftModule.h"

int shift_SAR(cpu, int dest_value, int num_bits) {
    // shift aritmetico a la derecha
    int32_t result = dest_value;
    if (num_bits >= 32) {
        result = (dest_value < 0) ? -1 : 0; // Fill with sign bit
    } else {
        for (int i = 0; i < num_bits; i++) {
            result = (result >> 1) | (dest_value & 0x80000000);
        }
    } 
    return result;
}

int shift_SHR(cpu, int dest_value, int num_bits) {
    // shift logico a la derecha
    int32_t result = dest_value;
    if (num_bits >= 32) {
        result = 0; // relleno con ceros
    } else {
        for (int i = 0; i < num_bits; i++) {
            result >>= 1;
        }
    } 
    return result;
}

int shift_SHL(cpu, int dest_value, int num_bits) {
    // shift logico a la izquierda
    int32_t result = dest_value;
    if (num_bits >= 32) {
        result = 0; // relleno con ceros
    } else {
        for (int i = 0; i < num_bits; i++) {
            result <<= 1;
        }
    } 
    return result;
}