#include "mem.h"
#include "../Processor/processor.h"
#include "../Utils/config.h"
#include "../Utils/errors.h"
#include "../Stack/stack.h"

int g_version;

void mem_init(mem_t *mem, uint32_t mem_size_kib) { //*modificacion
    mem->data = (uint8_t *)malloc(mem_size_kib * 1024);
    mem->size = mem_size_kib * 1024;
    if (!mem->data) {
        error_Output(MEMORY_ERROR);
        return;
    }
    memset(mem->data, 0, mem->size);

    // Inicializa tabla de segmentos (las 8 entradas)
    for (int i = 0; i < 8; i++) {
        mem->segments[i].base = 0;
        mem->segments[i].size = 0;
    }
    mem->segment_count = 0;
}

void mem_load(mem_t *mem, char *filename, cpu_t *cpu, char **params, int argc) {
    FILE *arch = fopen(filename, "rb");
    char id[6];
    uint8_t version, size_bytes[2];
    uint16_t code_size;

    
    if (!arch)
        error_Output(LOAD_PROGRAM_ERROR);
    
    if (fread(id, 1, 5, arch) != 5)
        error_Output(LOAD_PROGRAM_ERROR);
    id[5] = '\0';
    
    if (strcmp(id, "VMX25") != 0) 
        error_Output(LOAD_PROGRAM_ERROR);

    if (fread(&version, 1, 1, arch) != 1)
        error_Output(LOAD_PROGRAM_ERROR);

    if (version == 1)
        mem_load_v1(mem, arch, cpu);
    else if (version == 2)
        mem_load_v2(mem, arch, cpu, params, argc);
    else 
        error_Output(LOAD_PROGRAM_ERROR);

    fclose(arch);
}

void mem_load_v1(mem_t *mem, FILE *arch, cpu_t *cpu) {
    uint8_t size_bytes[2];
    uint16_t code_size;
    g_version = 1;

    if (fread(size_bytes, 1, 2, arch) != 2)
        error_Output(LOAD_PROGRAM_ERROR);

    code_size = (size_bytes[0] << 8) | size_bytes[1];

    if (code_size > MEM_SIZE)
        error_Output(LOAD_PROGRAM_ERROR);

    if (fread(mem->data, 1, code_size, arch) != code_size)
        error_Output(LOAD_PROGRAM_ERROR);

    cpu->CS = 0x00000000;
    cpu->DS = 0x00010000;
    cpu->IP = cpu->CS;

    // Segmento de CODIGO
    mem->segments[cpu->CS >> 16].base = 0;
    mem->segments[cpu->CS >> 16].size = code_size;

    // Segmento de DATOS
    mem->segments[cpu->DS >> 16].base = code_size;
    mem->segments[cpu->DS >> 16].size = MEM_SIZE - code_size;

    mem->segment_count = 2;
}

void mem_load_v2(mem_t *mem, FILE *arch, cpu_t *cpu, char **params, int argc) { //*modificacion
    uint8_t size_bytes[2];
    uint16_t cs_size, ds_size, es_size, ss_size, ks_size, entry_point;
    g_version = 2;

    
    // Leer tamaños de segmentos
    fread(size_bytes, 1, 2, arch);
    cs_size = (size_bytes[0] << 8) | size_bytes[1];
    fread(size_bytes, 1, 2, arch);
    ds_size = (size_bytes[0] << 8) | size_bytes[1];
    fread(size_bytes, 1, 2, arch);
    es_size = (size_bytes[0] << 8) | size_bytes[1];
    fread(size_bytes, 1, 2, arch);
    ss_size = (size_bytes[0] << 8) | size_bytes[1];
    fread(size_bytes, 1, 2, arch);
    ks_size = (size_bytes[0] << 8) | size_bytes[1];
    fread(size_bytes, 1, 2, arch);
    entry_point = (size_bytes[0] << 8) | size_bytes[1];

    // Crear Param Segment (si hay parámetros)
    uint32_t ps_size = create_param_segment(mem, params, argc);
    uint32_t offset = 0;
    uint8_t seg_index = 0;

    if (ps_size > 0) {
        mem->segments[seg_index].base = offset;
        mem->segments[seg_index].size = ps_size;
        cpu->PS = (seg_index << 16); //Este shift es para que se forme la entrada a la tabla de segmentos -> 0x000X0000
        offset += ps_size;
        seg_index++;
    }
    else
        cpu->PS = 0xFFFFFFFF;

    // Const Segment
    if (ks_size > 0) {
        mem->segments[seg_index].base = offset;
        mem->segments[seg_index].size = ks_size;
        cpu->KS = (seg_index << 16);

        // Leer constantes después del código (al final del archivo)
        offset += ks_size;
        seg_index++;
    }
    else
        cpu->KS = 0xFFFFFFFF;

    // Code Segment
    if (cs_size > 0) {
        mem->segments[seg_index].base = offset;
        mem->segments[seg_index].size = cs_size;
        cpu->CS = (seg_index << 16);

        // Leer código
        if (fread(mem->data + offset, 1, cs_size, arch) != cs_size) {
            error_Output(INSUFFICIENT_MEMORY);
            return;
        }

        offset += cs_size;
        seg_index++;
    }

    // Data Segment
    if (ds_size > 0) {
        mem->segments[seg_index].base = offset;
        mem->segments[seg_index].size = ds_size;
        cpu->DS = (seg_index << 16);
        offset += ds_size;
        seg_index++;
    }
    else
        cpu->DS = 0xFFFFFFFF;

    // Extra Segment
    if (es_size > 0) {
        mem->segments[seg_index].base = offset;
        mem->segments[seg_index].size = es_size;
        cpu->ES = (seg_index << 16);
        offset += es_size;
        seg_index++;
    }
    else
        cpu->ES = 0xFFFFFFFF;

    // Stack Segment
    if (ss_size > 0) {
        mem->segments[seg_index].base = offset;
        mem->segments[seg_index].size = ss_size;
        cpu->SS = (seg_index << 16);
        cpu->SP = cpu->SS + ss_size; // Pila vacía (apunta fuera) -> segun documentacion
        offset += ss_size;
        seg_index++;
    }
    else
        cpu->SS = 0xFFFFFFFF;

    // Verificar espacio suficiente
    if (offset > mem->size) {
        error_Output(INSUFFICIENT_MEMORY);
        return;
    }

    // Leer Const Segment (está después del código en el archivo)
    if (ks_size > 0) {
        uint32_t ks_base = mem->segments[cpu->KS >> 16].base;
        if (fread(mem->data + ks_base, 1, ks_size, arch) != ks_size) {
            error_Output(INSUFFICIENT_MEMORY);
            return;
        }
    }

    mem->segment_count = seg_index;

    // Inicializar IP con entry point
    cpu->IP = cpu->CS | entry_point;
    // Inicializar pila principal
    init_main_stack(cpu, mem, argc, ps_size);
}

void mem_read(mem_t *mem, cpu_t *cpu, int32_t logical_addr, int32_t *value, int size) {
    // Verificar tamaño
    if (size != 1 && size != 2 && size != 4) {
        error_Output(WRONG_SIZE); // Tamaño inválido
        return;
    }

    // Setear MAR (parte alta) y LAR
    cpu->LAR = logical_addr;

    uint16_t physical_addr = cpu_logic_to_physic(*mem, logical_addr, size);
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
        return; // Tamaño inválido
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
}

void mem_free(mem_t *mem) { //*modificacion
    if (mem->data) {
        free(mem->data);
        mem->data = NULL;
    }
}

uint32_t create_param_segment(mem_t *mem, char **params, int argc) { //*modificacion
    if (argc == 0 || params == NULL) {
        return 0; // No hay parámetros
    }

    // COPIAR LOS STRINGS
    // params es un array de C: params[0], params[1], params[2], ...
    // Cada params[i] es un string (char*)

    uint32_t offset = 0; // Dirección 0 del Param Segment

    // Guardar donde empieza cada string para después
    uint32_t string_offsets[argc];

    for (int i = 0; i < argc; i++) {
        // Ejemplo: params[0] = "probando"
        string_offsets[i] = offset; // Guardamos que "probando" empieza en offset 0

        // Copiar el string a memoria
        strcpy((char *)(mem->data + offset), params[i]);

        // Avanzar offset: strlen("probando") + 1 (por el '\0')
        offset += strlen(params[i]) + 1;
    }

    // Ahora offset = 14 (9 + 3 + 2 bytes de los strings)
    // string_offsets[0] = 0
    // string_offsets[1] = 9
    // string_offsets[2] = 12

    // CREAR EL ARRAY DE PUNTEROS (argv)
    uint32_t argv_start = offset; // argv empieza donde terminan los strings

    for (int i = 0; i < argc; i++) {
        // Crear un puntero de 32 bits (4 bytes)
        // Formato: [16 bits: entrada tabla segmentos | 16 bits: offset]

        // El shift es para dejar claro que es un puntero logico con segmento 0.
        uint32_t pointer = (0x0000 << 16) | string_offsets[i];

        // Ejemplo para params[0]:
        // pointer = 0x00000000 (segmento 0, offset 0)

        // Escribir en memoria(4 bytes)
        uint32_t pos = argv_start + (i * 4); // Cada puntero ocupa 4 bytes

        mem->data[pos + 0] = (pointer >> 24) & 0xFF; // Byte más significativo
        mem->data[pos + 1] = (pointer >> 16) & 0xFF;
        mem->data[pos + 2] = (pointer >> 8) & 0xFF;
        mem->data[pos + 3] = pointer & 0xFF; // Byte menos significativo
    }

    // Tamaño total = strings + punteros
    uint32_t total_size = offset + (argc * 4);

    return total_size;
}