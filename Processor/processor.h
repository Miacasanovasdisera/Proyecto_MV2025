#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../common.h"
#include "../Memory/mem.h"
// Estructura del estado del procesador
typedef struct {
    
    // Registros de acceso a memoria
    uint32_t LAR;
    uint32_t MAR;
    uint32_t MBR;

    // Registros de control de ejecución
    uint32_t IP;
    uint32_t OPC;
    uint32_t OP1;
    uint32_t OP2;

    // Registros de proposito general
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t EEX;
    uint32_t EFX;

    // Registros especiales
    uint32_t AC;
    uint32_t CC;

    // Registros de segmentos
    uint32_t CS;
    uint32_t DS;

    // Registros reservados (del 17 al 31)
    uint32_t reserved[15];

} cpu_t;

void cpu_init(cpu_t *);
// Funcion para coordinar el ciclo de leer de memoria, decodificar, actualizar IP y ejecutar(esta funcion iria en instruc.c)
// int cpu_step(cpu_t *, mem_t *);
// Funcion para leer la instruccion de memoria y guardarla en un vector temporal para trabajar
int cpu_fetch();
// Funcion para trabajar con el vector de cpu_fetch; extaer OPC y decodificar los 2 operandos
int cpu_decode();
// Funcion para actualizar el IP
void cpu_update_IP();

#endif