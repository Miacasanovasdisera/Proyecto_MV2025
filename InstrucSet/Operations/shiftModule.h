#ifndef SHIFT_MODULE_H
#define SHIFT_MODULE_H

#include "../../Utils/common.h"
#include "../../Processor/processor.h"
#include "../../Memory/mem.h"

// Funciones auxiliares para desplazamientos.
int shift_SHL(cpu_t *cpu, int value, int num_bits);
int shift_SHR(cpu_t *cpu, int value, int num_bits);
int shift_SAR(cpu_t *cpu, int value, int num_bits);
#endif