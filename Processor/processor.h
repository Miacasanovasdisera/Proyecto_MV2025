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

    // Registros de pila
    int32_t SP;
    int32_t BP;

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

    // Registros de segmentos (ahora son 6)
    int32_t CS;
    int32_t DS;
    int32_t ES;
    int32_t SS;
    int32_t KS;
    int32_t PS;

    // Registros reservados
    int32_t reserved[9];

} cpu_t;

// cpu_init inicializa el CPU, seteando todos sus registros a 0.
// cpu_update_IP actualiza el registro IP según los tipos de operandos de la instrucción actual.
// make_logical_in_cs crea una dirección lógica combinando el segmento CS, si es necesario, y un valor dado.
// cpu_logic_to_physic convierte una dirección lógica a física, verificando los límites del segmento.
// operators_registers_load carga los operandos de la instrucción actual en los registros OP1 y OP2.
// get_operand_type devuelve el tipo de operando (2 bits más significativos) del registro OP.
// get_operand_value devuelve el valor del operando (30 bits menos significativos) del registro OP.
// read_register lee el valor de un registro dado su código.
// write_register escribe un valor en un registro dado su código.
// calculate_logical_address calcula la dirección lógica a partir del tipo y valor del operando.

void cpu_init(cpu_t *);
void cpu_update_IP(cpu_t *, int8_t, int8_t);
uint32_t make_logical_in_cs(uint32_t, uint32_t);
uint16_t cpu_logic_to_physic(mem_t ,uint32_t, int);
void operators_registers_load(cpu_t *, mem_t);
int8_t get_operand_type(uint32_t);
int32_t get_operand_value(uint32_t);
int32_t read_register(cpu_t *, uint8_t);
void write_register(cpu_t *, uint8_t, uint32_t);
uint32_t calculate_logical_address(cpu_t *, uint8_t, uint32_t);

#endif