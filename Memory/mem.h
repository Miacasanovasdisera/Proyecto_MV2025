#ifndef MEMORY_H
#define MEMORY_H

#include "../common.h"
typedef struct cpu_t cpu_t;

#define MEM_SIZE 16384

//Tabla de segmentos
typedef struct {
    int16_t base;
    int16_t size;
} segment_desc_t;

//Estructura memoria principal
typedef struct {
    int8_t data[MEM_SIZE]; 
    segment_desc_t segments[8]; 
} mem_t;

// mem_init inicializa la memoria, limpiando toda la RAM y seteando a 0 la tabla de segmentos.
void mem_init(mem_t *);
// mem_load carga un programa desde el archivo cuyo nombre se pasa como parámetro en la memoria.
int mem_load(mem_t *, char *,cpu_t *);

// mem_read se encarga de leer una determinada cantidad de bytes (1,2 o 4) de una dirección lógica
// y devolver el valor leído en el parámetro 5to parametro. También actualiza los registros MAR, LAR y MBR del CPU.
int mem_read(mem_t *, cpu_t *, int32_t, int32_t *, int);

// mem_write se encarga de escribir una determinada cantidad de bytes (1,2 o 4) en una dirección lógica
// con el valor pasado en el 4to parámetro. También actualiza los registros MAR, LAR y MBR del CPU.
int mem_write(mem_t *, cpu_t *, int32_t, int32_t, int);

#endif