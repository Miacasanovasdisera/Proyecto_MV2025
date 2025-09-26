#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../Processor/processor.h"
#include "../Memory/mem.h"
#include "common.h"

static const char *opcode_name[] = { 
        "SYS", "JMP", "JZ", "JP", "JN", "JNZ", "JNP", "JNN",
        "NOT", "UNK","UNK","UNK","UNK","UNK","UNK","STOP", 
        "MOV", "ADD", "SUB", "MUL", "DIV", "CMP", "SHL", "SHR",
        "SAR", "AND", "OR",  "XOR","SWAP","LDL","LDH","RND"
};  // solo declaración

static const char *register_name[] = {
        "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
        "?", "?", "?",
        "EAX", "EBX", "ECX", "EDX", "EEX", "EFX",
        "AC", "CC",
        "?","?","?","?","?","?","?","?",
        "CS", "DS","?","?","?","?"
};
// Toda la lógica para imprimir el código desensamblado cuando se usa el flag -d.
void print_operand(uint32_t);
void disassembler(cpu_t , mem_t);

#endif