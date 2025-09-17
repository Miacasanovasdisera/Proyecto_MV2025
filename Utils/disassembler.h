#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H
#include "../InstrucSet/registers.h"

#include "common.h"

// Toda la lógica para imprimir el código desensamblado cuando se usa el flag -d.
void print_operand(uint32_t op, int type, char *register_name[]);
void Decode(cpu_t *cpu, int8_t *OP1, int8_t *OP2, int8_t *typeOP1, int8_t *typeOP2, int8_t *OPC);
void disassemble(cpu_t *cpu, mem_t *mem);
#endif