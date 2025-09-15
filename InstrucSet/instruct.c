#include "instruct.h"
#include "opcod.h"
#include "../Utils/errors.h"
#include "Operations/executeFunctions.h"

instruction_handler_t instruction_table[256] = {
    [OP_SYS]  = execute_SYS,
    [OP_JMP]  = execute_JMP,
    [OP_JZ]   = execute_JZ,
    [OP_JP]   = execute_JP,
    [OP_JN]   = execute_JN,
    [OP_JNZ]  = execute_JNZ,
    [OP_JNP]  = execute_JNP,
    [OP_JNN]  = execute_JNN,
    [OP_NOT]  = execute_NOT,
    [OP_MOV]  = execute_MOV,
    [OP_ADD]  = execute_ADD,
    [OP_SUB]  = execute_SUB,
    [OP_MUL]  = execute_MUL,
    [OP_DIV]  = execute_DIV,
    [OP_CMP]  = execute_CMP,
    [OP_SHL]  = execute_SHL,
    [OP_SHR]  = execute_SHR,
    [OP_SAR]  = execute_SAR,
    [OP_AND]  = execute_AND,
    [OP_OR]   = execute_OR,
    [OP_XOR]  = execute_XOR,
    [OP_SWAP] = execute_SWAP,
    [OP_LDL]  = execute_LDL,
    [OP_LDH]  = execute_LDH,
    [OP_RND]  = execute_RND,
    [OP_STOP] = execute_STOP
};

void InstrucSet_init() {
    for (int i = 0; i < 256; i++) {
        if (instruction_table[i] == NULL) {
            instruction_table[i] = execute_ILLEGAL;
        }
    }
}

int execute_ILLEGAL(cpu_t *cpu, mem_t *mem) {
    return error_Output(INVALID_INSTRUCTION);
}

int execute_instruction(cpu_t *cpu, mem_t *mem) {
    
    if (cpu->OPC < 256) {
        return instruction_table[cpu->OPC](cpu, mem);
    }
    return execute_ILLEGAL(cpu, mem);
}