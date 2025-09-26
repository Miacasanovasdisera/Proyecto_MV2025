#include "fluxControl.h"
#include "../helpers.h"
#include "../../Utils/errors.h"
#include "operands.h"
#include "../instruct.h"

int isNegative(cpu_t cpu) {
    return (cpu.CC & NMask) != 0; // Bit N (bit 31)
}

int isZero(cpu_t cpu) {
    return (cpu.CC & ZMask) != 0; // Bit Z (bit 30)
}

int execute_JMP(cpu_t *cpu, mem_t *mem){
    int32_t src_value;
    get_value(cpu, mem, cpu->OP1, &src_value);
    cpu->IP = src_value;
    return 0;
}

int execute_JZ(cpu_t *cpu, mem_t *mem){
    return isZero(*cpu) ? execute_JMP(cpu, mem) : 0;
}

int execute_JNZ(cpu_t *cpu, mem_t *mem){
    return !isZero(*cpu) ? execute_JMP(cpu, mem) : 0;
}

int execute_JP(cpu_t *cpu, mem_t *mem){
    return !isZero(*cpu) && !isNegative(*cpu) ? execute_JMP(cpu, mem) : 0;
}

int execute_JNP(cpu_t *cpu, mem_t *mem){
    return (isZero(*cpu) || isNegative(*cpu)) ? execute_JMP(cpu, mem) : 0;
}

int execute_JNN(cpu_t *cpu, mem_t *mem){
    return !isNegative(*cpu) ? execute_JMP(cpu, mem) : 0;
}

int execute_JN(cpu_t *cpu, mem_t *mem){
    return isNegative(*cpu) ? execute_JMP(cpu, mem) : 0;
}

int execute_STOP(cpu_t *cpu, mem_t *mem){
    cpu->IP = 0xFFFFFFFF; // Valor especial para indicar que la CPU está detenida
    return 0;
}