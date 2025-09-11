#ifndef ALU_H
#define ALU_H

#include "../common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

// Funciones de la ALU que actualizan el registro CC.
int execute_ADD(cpu_t *,mem_t *);
int execute_SUB(cpu_t *,mem_t *);
int execute_MUL(cpu_t *,mem_t *);
int execute_DIV(cpu_t *,mem_t *);
int execute_CMP(cpu_t *,mem_t *);
int execute_NOT(cpu_t *,mem_t *);
int execute_SHL(cpu_t *,mem_t *);
int execute_SHR(cpu_t *,mem_t *);
int execute_SAR(cpu_t *,mem_t *);
int execute_AND(cpu_t *,mem_t *);
int execute_OR(cpu_t *,mem_t *);
int execute_XOR(cpu_t *,mem_t *);

#endif