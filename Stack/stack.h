#ifndef STACK_H
#define STACK_H

#include "../Utils/common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

// Función auxiliar para inicializar la pila principal (stack) del programa cargado.
void init_main_stack(cpu_t *cpu, mem_t *mem, int argc, uint32_t ps_size);
// stack_push agrega un valor a la pila (stack) del CPU.
void stack_push(cpu_t *cpu, mem_t *mem, int32_t value);
// stack_pop extrae un valor de la pila (stack) del CPU.
int32_t stack_pop(cpu_t *cpu, mem_t *mem);

#endif