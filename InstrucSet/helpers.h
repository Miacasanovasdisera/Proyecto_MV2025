#ifndef HELPERS_H
#define HELPERS_H

#include "../Utils/common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

//mascaras para los bits del registro CC. 
//NMask: El bit más significativo es el indicador de
//signo (bit N), que valdrá 1 cuando la última operación matemática o lógica haya dado por resultado un
//valor negativo y 0 en cualquier otro caso.
//ZMask: El segundo bit más significativo es el indicador de cero (bit Z),
//que valdrá 1 cuando la última operación matemática o lógica haya dado por resultado cero y 0 en
//cualquier otro caso.
#define NMask 0x80000000  // Bit de signo
#define ZMask 0x40000000  // Bit de cero

// get_value devuelve en el cuarto parametro el valor del operando, que le pasas como tercer parametro. (ya sea registro, inmediato o memoria)
// get_destination_address devuelve en el tercer parametro la direccion del destino, pasado en el segundo parametro, (registro o memoria) para luego escribir en esa direccion
// write_dest escribe en la direccion obtenida por get_destination_address el valor pasado como parametro
// get_memory_size devuelve el tamaño en bytes del operando (1,2 o 4) según los bits 31-30 del registro OP

void get_value(cpu_t *, mem_t *,int32_t, int32_t *);
void get_destination_address(cpu_t *, int32_t , uint32_t *dest_addrss);
void write_dest(cpu_t *, mem_t *, uint8_t , uint32_t, int32_t, int32_t);
void update_CC(cpu_t *cpu,int32_t result);
int get_memory_size(uint32_t OP_register);
int auxiliar_SAR(cpu_t *cpu, int32_t value, int32_t num_bits);

// Ejemplo:  mov edx,5  edx==destino 5==origen
// SUB ECX, EDX        ; ECX = ECX - EDX    (destino=ECX, origen=EDX)
// ejemplo: add edx,5   edx==destino 5==origen 

#endif