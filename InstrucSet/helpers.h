#ifndef HELPERS_H
#define HELPERS_H

#include "../Utils/common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

// get_value devuelve en el tercer parametro el valor del operando, que le pasas como tercer parametro. (ya sea registro, inmediato o memoria)
// get_destination_address devuelve en el tercer parametro la direccion del destino, pasado en el segundo parametro, (registro o memoria) para luego escribir en esa direccion
// write_dest escribe en la direccion obtenida por get_destination_address el valor pasado como parametro

int get_value(cpu_t *, mem_t *,int32_t, int32_t *);
int get_destination_address(cpu_t *, int32_t , int32_t *dest_addrss);
int write_dest(cpu_t *, mem_t *, int8_t , int32_t , int32_t );
void update_CC(cpu_t *cpu,int32_t result);

// Ejemplo:  mov edx,5  edx==destino 5==origen
// SUB ECX, EDX        ; ECX = ECX - EDX    (destino=ECX, origen=EDX)
// ejemplo: add edx,5   edx==destino 5==origen 

#endif