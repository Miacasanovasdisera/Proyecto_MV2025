#ifndef VMI_H
#define VMI_H

#include "../Utils/common.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

// Guarda el estado completo de la VM en un archivo .vmi
void save_vmi(cpu_t *cpu, mem_t *mem, char *filename);

// Carga el estado completo de la VM desde un archivo .vmi
void load_vmi(cpu_t *cpu, mem_t *mem, char *filename);

#endif