#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include "../../Utils/common.h"
#include "../../Processor/processor.h"
#include "../../Memory/mem.h"

// Funciones de operaciones de transferencia de datos
int execute_MOV(cpu_t *,mem_t *);
int execute_SWAP(cpu_t *,mem_t *);
int execute_LDL(cpu_t *,mem_t *);
int execute_LDH(cpu_t *,mem_t *);
int execute_RND(cpu_t *,mem_t *);

#endif