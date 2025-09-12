#ifndef HELPERS_H
#define HELPERS_H

#include "../common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

int get_value(cpu_t *cpu, int32_t OP, mem_t *mem, int32_t *origin_operand_value);
int get_destination_address(cpu_t *, int32_t , int32_t *dest_addrss);
int write_dest(cpu_t *, mem_t *, int8_t , int32_t , int32_t );
void update_CC(cpu_t *cpu,int32_t result);

// Ejemplo:  mov edx,5  edx==destino 5==origen
// SUB ECX, EDX        ; ECX = ECX - EDX    (destino=ECX, origen=EDX)
// ejemplo: add edx,5   edx==destino 5==origen 

#endif