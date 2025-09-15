#ifndef ALU_H
#define ALU_H

#include "../../Utils/common.h"
#include "../../Processor/processor.h"
#include "../../Memory/mem.h"

// Funciones de la ALU que actualizan el registro CC. Se pasan: el cpu, el valor destino y el valor origen (excepto en NOT que solo tiene un operando)
// Devuelven el resultado de la operación (excepto CMP que no devuelve nada)
int alu_ADD(cpu_t *, int32_t, int32_t);
int alu_SUB(cpu_t *, int32_t, int32_t);
int alu_MUL(cpu_t *, int32_t, int32_t);
void alu_DIV(cpu_t *, int32_t, int32_t, int32_t *);
int alu_NOT(cpu_t *, int32_t);
void alu_CMP(cpu_t *, int32_t, int32_t);
int alu_AND(cpu_t *, int32_t, int32_t);
int alu_OR(cpu_t *, int32_t, int32_t);
int alu_XOR(cpu_t *, int32_t, int32_t);

#endif