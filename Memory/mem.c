#include "mem.h"
#include "../Processor/processor.h"
#include "../Memory/mem.h"

void mem_load(mem_t *mem, char *archivo, cpu_t *cpu)
{
    FILE *arch = fopen(*archivo, "rb");
    char id[6];
    uint8_t version, size_byte[2];
    uint16_t code_size;

    if (fread(id, 1, 5, arch) != 5)
    {
        fprintf(stderr, "Error: No se pudo leer la cabecera del archivo.\n");
        fclose(arch);
        return 0;
    }

    if (strcmp(id, "VMX25") != 0){
        
        return 0;
    }

    if (fread(&version,1,1,arch) != 1) {
        return 0;
    }

    

        if (arch == NULL)
            printf("no se pudo abrir el archivo");
        else
        {
            for (size_t i = 0; i < code_size; i++)
                mem->data[i] = fgetc(arch); // Lee un byte del archivo y lo guarda en la RAM
        }
}