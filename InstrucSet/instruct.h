#ifndef INSTRUCT_H
#define INSTRUCT_H

#include "../Utils/common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

// Tipo de función para todas las instrucciones
typedef int (*instruction_handler_t)(cpu_t*, mem_t*);

// Inicia los manejadores
void InstrucSet_init();

// Función principal de ejecución
int execute_instruction(cpu_t *, mem_t *);

// Codigo OPC no valido
int execute_ILLEGAL(cpu_t *, mem_t *);

#endif