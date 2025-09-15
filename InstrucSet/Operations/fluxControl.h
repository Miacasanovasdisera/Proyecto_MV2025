#ifndef FLUX_H
#define FLUX_H

#include "../../Utils/common.h"
#include "../../Processor/processor.h"
#include "../../Memory/mem.h"

// Funciones de operaciones de control de flujo:
int execute_JMP(cpu_t *, mem_t *);
int execute_JZ(cpu_t *, mem_t *);
int execute_JP(cpu_t *, mem_t *);
int execute_JN(cpu_t *,mem_t *); 
int execute_JNZ(cpu_t *,mem_t *);
int execute_JNP(cpu_t *,mem_t *);
int execute_JNN(cpu_t *,mem_t *);
int execute_STOP(cpu_t *,mem_t *);

#endif