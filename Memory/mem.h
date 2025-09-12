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

void mem_init(mem_t *);
int mem_load(mem_t *, char *,cpu_t *);
int mem_read(mem_t *, cpu_t *, int32_t, int32_t *, int);
int mem_write(mem_t *, cpu_t *, int32_t, int32_t, int);

#endif