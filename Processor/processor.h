#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../common.h"
#include "../Memory/mem.h"
// Estructura del estado del procesador
typedef struct cpu_t{
    
    // Registros de acceso a memoria
    int32_t LAR;
    int32_t MAR;
    int32_t MBR;

    // Registros de control de ejecución
    int32_t IP;
    int32_t OPC;
    int32_t OP1;
    int32_t OP2;

    // Registros de proposito general
    int32_t EAX;
    int32_t EBX;
    int32_t ECX;
    int32_t EDX;
    int32_t EEX;
    int32_t EFX;

    // Registros especiales
    int32_t AC;
    int32_t CC;

    // Registros de segmentos
    int32_t CS;
    int32_t DS;

    // Registros reservados (del 17 al 31)
    int32_t reserved[15];

} cpu_t;

void cpu_init(cpu_t *);
void cpu_update_IP(cpu_t *, int8_t, int8_t);
int32_t cpu_logic_to_physic(mem_t ,int32_t, int);
void Operators_Registers_Load(mem_t, cpu_t *);
int8_t get_operand_type(int32_t);
int32_t get_operand_value(int32_t);
int32_t read_register(cpu_t *, uint8_t);
void write_register(cpu_t *, uint8_t, uint32_t);

#endif