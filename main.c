#include "Utils/common.h"
#include "Processor/processor.h"
#include "Memory/mem.h"
#include "InstrucSet/instruct.h"
#include "Utils/disassembler.h"
#include "Utils/errors.h"
#include "Utils/config.h"
#include "Stack/stack.h"
#include "Images/vmi.h"

// Nombre del archivo .vmi (si se usa)
const char *g_vmi_filename = NULL; 

void execute_program(cpu_t *cpu, mem_t *mem);

int main(int argc, char *argv[]) {
    
    cpu_t cpu;
    mem_t mem;
    vm_config_t config;

    config_init(&config);
    config_parse(&config, argc, argv); 
    config_validate(&config);

    cpu_init(&cpu);
    mem_init(&mem, config.memory_size_kib);
    InstrucSet_init();

    printf("=== CARGA ===\n");

    if (config.vmx_file != NULL && config.vmi_file == NULL) {
        // MODO A: Solo .vmx (ejecución normal)
        printf("Cargando programa: %s\n", config.vmx_file);
        printf("Memoria: %u KiB\n", config.memory_size_kib);
        
        if (config.param_count > 0) {
            printf("Parametros (%d): ", config.param_count);
            for (int i = 0; i < config.param_count; i++) {
                printf("\"%s\" ", config.params[i]);
            }
            printf("\n");
        }
        
        mem_load(&mem, config.vmx_file, &cpu, config.params, config.param_count);
        printf("Extra Segment: %d\n", mem.segments[cpu.ES >> 16].base);
        if (config.disassem_mode) {
            printf("\n=== DISASSEMBLER ===\n");
            disassembler(cpu, mem);
        }
        
        printf("\n=== EJECUCION ===\n");
        execute_program(&cpu, &mem);
    }
    else if (config.vmx_file != NULL && config.vmi_file != NULL) {
        // MODO B: .vmx + .vmi (ejecución con breakpoints)
        printf("Cargando programa: %s\n", config.vmx_file);
        printf("Archivo de imagen: %s\n", config.vmi_file);
        printf("Memoria: %u KiB\n", config.memory_size_kib);
        
        if (config.param_count > 0) {
            printf("Parametros (%d): ", config.param_count);
            for (int i = 0; i < config.param_count; i++)
                printf("\"%s\" ", config.params[i]);
            
            printf("\n");
        }
        
        mem_load(&mem, config.vmx_file, &cpu, config.params, config.param_count);
        g_vmi_filename = config.vmi_file;

        if (config.disassem_mode) {
            printf("\n=== DISASSEMBLER ===\n");
            disassembler(cpu, mem);
        }
        
        printf("\n=== EJECUCION (modo debug) ===\n");
        printf("Los breakpoints guardaran el estado en: %s\n", config.vmi_file);
        execute_program(&cpu, &mem);
    }
    else if (config.vmx_file == NULL && config.vmi_file != NULL) {
        // MODO C: Solo .vmi (continuar desde imagen)
        printf("Cargando imagen: %s\n", config.vmi_file);
        
        load_vmi(&cpu, &mem, config.vmi_file);
        printf("\n=== EJECUCION (desde imagen) ===\n");
        execute_program(&cpu, &mem);
    }

    mem_free(&mem);
    config_free(&config);

    return 0;
}

void execute_program(cpu_t *cpu, mem_t *mem) {
    int result, CS = cpu->CS >> 16;
    do {
        //printf("IP: %08X | ", cpu->IP);
        operators_registers_load(cpu, *mem);
        //printf("OPC: %02X | OP1: %08X | OP2: %08X\n",cpu->OPC, cpu->OP1, cpu->OP2);
        result = execute_instruction(cpu, mem);
    } while ((cpu->IP & 0xFFFF) < mem->segments[CS].size);
}