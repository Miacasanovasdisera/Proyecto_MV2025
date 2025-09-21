#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../Utils/common.h"
#include "../Memory/mem.h"
// Estructura del estado del procesador
typedef struct cpu_t {
    
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

// cpu_init inicializa el CPU, seteando todos sus registros a 0.
// cpu_update_IP actualiza el registro IP según los tipos de operandos de la instrucción actual.
// cpu_logic_to_physic convierte una dirección lógica a física, verificando los límites del segmento.
// Operators_Registers_Load carga los operandos de la instrucción actual en los registros OP1 y OP2.
// get_operand_type devuelve el tipo de operando (2 bits más significativos) del registro OP.
// get_operand_value devuelve el valor del operando (30 bits menos significativos) del registro OP.
// read_register lee el valor de un registro dado su código.
// write_register escribe un valor en un registro dado su código.
// calculate_logical_address calcula la dirección lógica a partir del tipo y valor del operando.

void cpu_init(cpu_t *);
void cpu_update_IP(cpu_t *, int8_t, int8_t);
uint32_t cpu_logic_to_physic(mem_t ,uint32_t, int);
void operators_registers_load(cpu_t *, mem_t);
int8_t get_operand_type(uint32_t);
int32_t get_operand_value(int32_t);
int32_t read_register(cpu_t *, uint8_t);
void write_register(cpu_t *, uint8_t, uint32_t);
uint32_t calculate_logical_address(cpu_t *, uint8_t, uint32_t);

#endif