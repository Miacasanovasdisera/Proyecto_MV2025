#include "shiftModule.h"

int shift_SAR(cpu_t *cpu, int value, int num_bits) {
    // shift aritmetico a la derecha
    int32_t result = value;
    if (num_bits >= 32) {
        result = (value < 0) ? -1 : 0; // Fill with sign bit
    } else {
        for (int i = 0; i < num_bits; i++) {
            result = (result >> 1) | (value & 0x80000000);
        }
    } 
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