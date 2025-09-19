#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../Processor/processor.h"
#include "../Memory/mem.h"
#include "common.h"

static const char *opcode_name[28] = { 
        "SYS", "JMP", "JZ", "JP", "JN", "JNZ", "JNP", "JNN",
        "NOT", "UNK", "MOV", "ADD", "SUB", "MUL", "DIV", "STOP",
        "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
        "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
        "MOV", "ADD", "SUB", "MUL", "DIV", "CMP", "SHL", "SHR",
        "SAR", "AND", "OR",  "XOR","SWAP","LDL","LDH","RND"
};  // solo declaración

static const char *register_name[28] = {
        "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
        "?", "?", "?",
        "EAX", "EBX", "ECX", "EDX", "EEX", "EFX",
        "AC", "CC",
        "?","?","?","?","?","?","?","?",
        "CS", "DS"
};
// Toda la lógica para imprimir el código desensamblado cuando se usa el flag -d.
void print_operand(uint32_t, int);
void Decode(cpu_t *, mem_t, int32_t *, int32_t *, int8_t *, int8_t *, int32_t *);
void disassemble(cpu_t *, mem_t *);

#endif