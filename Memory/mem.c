#include "mem.h"
#include "../Processor/processor.h"

void mem_init(mem_t *mem) {
    // Limpia toda la RAM (16 KiB)
    memset(mem->data, 0, MEM_SIZE);

    // Inicializa tabla de segmentos (las 8 entradas)
    for (int i = 0; i < 8; i++) {
        mem->segments[i].base = 0;
        mem->segments[i].size = 0;
    }
}

int mem_load(mem_t *mem, char *archivo, cpu_t *cpu) {
    FILE *arch = fopen(archivo, "rb");
    // el archivo no guarda el caracter nulo, por eso id[5]
    char id[5];
    int8_t version,size_bytes[2];
    int16_t code_size;

    if (arch) {

        if (fread(id, 1, 5, arch) != 5) 
            return 1;
        
        if (strcmp(id, "VMX25") != 0) 
            return 2;
        
        if (fread(&version,1,1,arch) != 1) 
            return 3;
            
        if (version != 1) 
            return 4;        

        if (fread(size_bytes, 1, 2, arch) != 2)
            return 5;

        code_size = (size_bytes[0] << 8) | size_bytes[1];

        if (code_size > MEM_SIZE)
            return 6;
                
        if (fread(mem->data,1,code_size,arch) != code_size)
            return 7;
        
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

        return 0;
    }
    return 8;
}