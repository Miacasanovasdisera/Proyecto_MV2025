#include "../common.h"

#define MEM_SIZE 16384

//Tabla de segmentos
typedef struct {
    uint16_t base;
    uint16_t size;
} segment_desc_t;

//Estructura memoria principal
typedef struct {
    uint8_t data[MEM_SIZE]; 
    segment_desc_t segments[8]; 
} mem_t;

void mem_init(mem_t *);
void mem_load(mem_t *, char *, cpu_t *);