#ifndef STACK_CONTROL_H
#define STACK_CONTROL_H

#include "../../Utils/common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

int execute_PUSH(cpu_t *cpu, mem_t *mem);
int execute_POP(cpu_t *cpu, mem_t *mem);
int execute_CALL(cpu_t *cpu, mem_t *mem);
int execute_RET(cpu_t *cpu, mem_t *mem);

#endif