#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../Processor/processor.h"
#include "../Memory/mem.h"
#include "common.h"

static const char *opcode_name[] = { 
        "SYS", "JMP", "JZ", "JP", "JN", "JNZ", "JNP", "JNN",
        "NOT", "UNK","UNK","PUSH","POP","CALL","RET","STOP", 
        "MOV", "ADD", "SUB", "MUL", "DIV", "CMP", "SHL", "SHR",
        "SAR", "AND", "OR",  "XOR","SWAP","LDL","LDH","RND"
};  // solo declaración
//para los registros de 16 bits
static const char *register_name_16[] = {
        "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
        "SP" "BP", "?",
        "AX", "BX", "CX", "DX", "EX", "FX",
        "AC", "CC",
        "?","?","?","?","?","?","?","?",
       "CS", "DS","ES","SS","KS","PS"
};
//p los reg de 8 bits
static const char *register_name_8L[] = {
        "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
        "SP" "BP", "?",
        "AL", "BL", "CL", "DL", "EL", "FL",
        "AC", "CC",
        "?","?","?","?","?","?","?","?",
        "CS", "DS","ES","SS","KS","PS"
};
//p los reg de 8 bits altos
static const char *register_name_8H[] = {
        "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
        "SP" "BP", "?",
        "AH", "BH", "CH", "DH", "EH","FH",
        "AC", "CC",
        "?","?","?","?","?","?","?","?",
        "CS", "DS","ES","SS","KS","PS"
};
// prefijos de tamaño de operando
static const char *size_prefix[] = { "l", "?", "w","b" };

static const char *register_name[] = {
        "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
        "SP", "BP", "?",
        "EAX", "EBX", "ECX", "EDX", "EEX", "EFX",
        "AC", "CC",
        "?","?","?","?","?","?","?","?",
        "CS", "DS","ES","SS","KS","PS"
};

// Toda la lógica para imprimir el código desensamblado cuando se usa el flag -d.
void print_operand(uint32_t);
void disassembler(cpu_t , mem_t);

#endif