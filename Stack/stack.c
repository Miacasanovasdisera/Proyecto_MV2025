#include "stack.h"
#include "../Utils/errors.h"

void init_main_stack(cpu_t *cpu, mem_t *mem, int argc, uint32_t ps_size) {
    // PUSH en orden inverso (último en entrar, primero en el tope)
    
    // 3. PUSH argv
    if (argc > 0) {
        // argv está al final del Param Segment
        uint32_t argv_offset = ps_size - (argc * 4);
        stack_push(cpu, mem, (0x0000 << 16) | argv_offset);
    } else {
        stack_push(cpu, mem, 0xFFFFFFFF);
    }
    
    // 2. PUSH argc
    stack_push(cpu, mem, argc);
    
    // 1. PUSH dirección de retorno (0xFFFFFFFF = salir)
    stack_push(cpu, mem, 0xFFFFFFFF);
}

void stack_push(cpu_t *cpu, mem_t *mem, int32_t value) {
    // Decrementar SP (la pila crece hacia direcciones más bajas)
    cpu->SP -= 4;
    
    // Verificar desbordamiento de pila
    if (cpu->SP < cpu->SS) {
        error_Output(MEMORY_ERROR);
        return;
    }
    
    // Escribir el valor en la dirección apuntada por SP
    mem_write(mem, cpu, cpu->SP, value, 4);
}

int32_t stack_pop(cpu_t *cpu, mem_t *mem) {
    int32_t value;
    
    // Verificar subdesbordamiento de pila
    if (cpu->SP >= cpu->SS + (mem->segments[(cpu->SS >> 16) & 0xFF].size)) {
        error_Output(MEMORY_ERROR);
        return 0; // Valor de retorno arbitrario en caso de error
    }
    
    // Leer el valor en la dirección apuntada por SP
    mem_read(mem, cpu, cpu->SP, &value, 4);
    
    // Incrementar SP (la pila se reduce hacia direcciones más altas)
    cpu->SP += 4;
    
    return value;
}