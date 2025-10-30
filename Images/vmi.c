#include "vmi.h"
#include "errors.h"

void save_vmi(cpu_t *cpu, mem_t *mem, char *filename) {
    if (filename == NULL) {
        return; // Si no hay archivo, no hacer nada
    }
    
    FILE *arch = fopen(filename, "wb");
    if (!arch) {
        error_Output(IMAGE_ERROR);
        return;
    }
    
    // HEADER (8 bytes)
    // ID: "VMI25" (5 bytes)
    fwrite("VMI25", 1, 5, arch);
    
    // Versión: 1 (1 byte)
    uint8_t version = 1;
    fwrite(&version, 1, 1, arch);
    
    // Tamaño de memoria en KiB (2 bytes, big endian)
    uint16_t mem_size_kib = mem->size / 1024;
    uint8_t size_bytes[2];
    size_bytes[0] = (mem_size_kib >> 8) & 0xFF;
    size_bytes[1] = mem_size_kib & 0xFF;
    fwrite(size_bytes, 1, 2, arch);
    
    // REGISTROS (32 × 4 = 128 bytes)
    // Escribir todos los registros en big endian
    int32_t registers[32] = {
        cpu->LAR, cpu->MAR, cpu->MBR, cpu->IP,
        cpu->OPC, cpu->OP1, cpu->OP2, cpu->SP,
        cpu->BP, 0, // Registro 9 reservado
        cpu->EAX, cpu->EBX, cpu->ECX, cpu->EDX, cpu->EEX, cpu->EFX,
        cpu->AC, cpu->CC,
        0, 0, 0, 0, 0, 0, 0, 0, // Registros 18-25 reservados
        cpu->CS, cpu->DS, cpu->ES, cpu->SS, cpu->KS, cpu->PS
    };
    
    for (int i = 0; i < 32; i++) { //Esto se hace para escribir en big endian (seguro)
        uint32_t reg = (uint32_t)registers[i];
        uint8_t reg_bytes[4];
        reg_bytes[0] = (reg >> 24) & 0xFF;
        reg_bytes[1] = (reg >> 16) & 0xFF;
        reg_bytes[2] = (reg >> 8) & 0xFF;
        reg_bytes[3] = reg & 0xFF;
        fwrite(reg_bytes, 1, 4, arch);
    }
    
    // TABLA DE SEGMENTOS (8 × 4 = 32 bytes)
    // Cada entrada: base (2 bytes) + size (2 bytes)
    for (int i = 0; i < 8; i++) {
        uint16_t base = mem->segments[i].base;
        uint16_t size = mem->segments[i].size;
        
        uint8_t seg_bytes[4];
        seg_bytes[0] = (base >> 8) & 0xFF;
        seg_bytes[1] = base & 0xFF;
        seg_bytes[2] = (size >> 8) & 0xFF;
        seg_bytes[3] = size & 0xFF;
        
        fwrite(seg_bytes, 1, 4, arch);
    }
    
    // MEMORIA COMPLETA (variable)
    fwrite(mem->data, 1, mem->size, arch);

    fclose(arch);
}

void load_vmi(cpu_t *cpu, mem_t *mem, char *filename) {
    FILE *arch = fopen(filename, "rb");
    if (!arch) {
        error_Output(IMAGE_ERROR);
        return;
    }
    
    // HEADER (8 bytes)
    char id[6];
    if (fread(id, 1, 5, arch) != 5) {
        fclose(arch);
        error_Output(IMAGE_ERROR);
        return;
    }
    
    if (strcmp(id, "VMI25") != 0) {
        fclose(arch);
        error_Output(IMAGE_ERROR);
        return;
    }
    
    uint8_t version;
    if (fread(&version, 1, 1, arch) != 1 || version != 1) {
        fclose(arch);
        error_Output(IMAGE_ERROR);
        return;
    }
    
    uint8_t size_bytes[2];
    if (fread(size_bytes, 1, 2, arch) != 2) {
        fclose(arch);
        error_Output(IMAGE_ERROR);
        return;
    }
    
    uint16_t mem_size_kib = (size_bytes[0] << 8) | size_bytes[1];
    
    // Verificar que la memoria actual tenga el tamaño correcto
    if (mem->size != mem_size_kib * 1024) {
        fclose(arch);
        error_Output(IMAGE_ERROR);
        return;
    }
    
    //  REGISTROS (32 × 4 = 128 bytes)
    int32_t registers[32];
    for (int i = 0; i < 32; i++) { //Esto se hace para leer en big endian (seguro)
        uint8_t reg_bytes[4];
        if (fread(reg_bytes, 1, 4, arch) != 4) {
            fclose(arch);
            error_Output(IMAGE_ERROR);
            return;
        }
        
        uint32_t reg = ((uint32_t)reg_bytes[0] << 24) |
                       ((uint32_t)reg_bytes[1] << 16) |
                       ((uint32_t)reg_bytes[2] << 8) |
                       ((uint32_t)reg_bytes[3]);
        registers[i] = (int32_t)reg;
    }
    
    // Asignar registros
    cpu->LAR = registers[0];
    cpu->MAR = registers[1];
    cpu->MBR = registers[2];
    cpu->IP  = registers[3];
    cpu->OPC = registers[4];
    cpu->OP1 = registers[5];
    cpu->OP2 = registers[6];
    cpu->SP  = registers[7];
    cpu->BP  = registers[8];
    cpu->EAX = registers[10];
    cpu->EBX = registers[11];
    cpu->ECX = registers[12];
    cpu->EDX = registers[13];
    cpu->EEX = registers[14];
    cpu->EFX = registers[15];
    cpu->AC  = registers[16];
    cpu->CC  = registers[17];
    cpu->CS  = registers[26];
    cpu->DS  = registers[27];
    cpu->ES  = registers[28];
    cpu->SS  = registers[29];
    cpu->KS  = registers[30];
    cpu->PS  = registers[31];
    
    // TABLA DE SEGMENTOS (8 × 4 = 32 bytes)
    for (int i = 0; i < 8; i++) {
        uint8_t seg_bytes[4];
        if (fread(seg_bytes, 1, 4, arch) != 4) {
            fclose(arch);
            error_Output(IMAGE_ERROR);
            return;
        }
        
        mem->segments[i].base = (seg_bytes[0] << 8) | seg_bytes[1];
        mem->segments[i].size = (seg_bytes[2] << 8) | seg_bytes[3];
    }
    
    // MEMORIA COMPLETA (variable)
    if (fread(mem->data, 1, mem->size, arch) != mem->size) {
        fclose(arch);
        error_Output(IMAGE_ERROR);
        return;
    }

    fclose(arch);
}