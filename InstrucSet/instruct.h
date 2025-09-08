#ifndef INSTRUCT_H
#define INSTRUCT_H

#include "../common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

// En base al valor de cpu->OPC se llama a una funcion.

int execute_instruction(cpu_t *,mem_t *);

#endif