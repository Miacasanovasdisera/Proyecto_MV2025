#include "mem.h"
#include "../Processor/processor.h"
#include "../Utils/errors.h"

void mem_init(mem_t *mem) {
    // Limpia toda la RAM (16 KiB)
    memset(mem->data, 0, MEM_SIZE);

    // Inicializa tabla de segmentos (las 8 entradas)
    for (int i = 0; i < 8; i++) {
        mem->segments[i].base = 0;
        mem->segments[i].size = 0;
    }
}
// scoop install main/gcc

void mem_load(mem_t *mem, char *archivo, cpu_t *cpu) {
    FILE *arch = fopen(archivo, "rb");
    // el archivo no guarda el caracter nulo, por eso id[5]
    char id[5];
    uint8_t version,size_bytes[2];
    uint16_t code_size;

    if (!arch)
        error_Output(LOAD_PROGRAM_ERROR);

    if (fread(id, 1, 5, arch) != 5) 
        error_Output(LOAD_PROGRAM_ERROR);
    
    if (strcmp(id, "VMX25") != 0) 
        error_Output(LOAD_PROGRAM_ERROR);
    
    if (fread(&version,1,1,arch) != 1) 
        error_Output(LOAD_PROGRAM_ERROR);
         
    if (version != 1) 
        error_Output(LOAD_PROGRAM_ERROR);

    if (fread(size_bytes, 1, 2, arch) != 2)
        error_Output(LOAD_PROGRAM_ERROR);

    code_size = (size_bytes[0] << 8) | size_bytes[1];

    if (code_size > MEM_SIZE)
        error_Output(LOAD_PROGRAM_ERROR);
            
    if (fread(mem->data,1,code_size,arch) != code_size)
        error_Output(LOAD_PROGRAM_ERROR);
    
    fclose(arch);

    // Segmento de CODIGO
    mem->segments[0].base = 0;
    mem->segments[0].size = code_size;

    // Segmento de DATOS
    mem->segments[1].base = code_size;
    mem->segments[1].size = MEM_SIZE - code_size;

    cpu->CS = 0x00000000;
    cpu->DS = 0x00010000;
    cpu->IP = cpu->CS;
}

void mem_read(mem_t *mem, cpu_t *cpu, int32_t logical_addr, int32_t *value, int size) {
    // Verificar tamaño
    if (size != 1 && size != 2 && size != 4) {
        error_Output(WRONG_SIZE); //Tamaño inválido
        return;
    }
    
    // Setear MAR (parte alta) y LAR 
    cpu->LAR = logical_addr;
    
    uint32_t physical_addr = cpu_logic_to_physic(*mem, logical_addr, size);
    cpu->MAR = ((uint32_t)size << 16) | physical_addr;
    
    // Leer bytes de la memoria física
    uint32_t aux = 0;
    for (int i = 0; i < size; i++)
        aux = (aux << 8) | (uint8_t)mem->data[physical_addr + i];
    
    *value = (int32_t)aux;

    // Actualizar MBR y cargar parte baja MAR
    cpu->MBR = *value;
    
}

void mem_write(mem_t *mem, cpu_t *cpu, int32_t logical_addr, int32_t value, int size) {
    // Verificar tamaño
    if (size != 1 && size != 2 && size != 4) {
        error_Output(WRONG_SIZE);
        return; //Tamaño inválido
    }

    // Setear LAR, MAR y MBR
    cpu->LAR = logical_addr;
    cpu->MBR = value;
    
    uint32_t physical_addr = cpu_logic_to_physic(*mem, logical_addr, size);
    cpu->MAR = ((uint32_t)size << 16) | physical_addr;
    
    uint32_t aux = (uint32_t)value;
    for (int i = size - 1; i >= 0; i--) {
        mem->data[physical_addr + i] = (uint8_t)(aux & 0xFF);
        aux >>= 8;
    }

    // Escribir bytes en memoria física
    for (int i = size - 1; i >= 0; i--) {
        mem->data[physical_addr + i] = value & 0xFF;
        value >>= 8;
    }
    
}