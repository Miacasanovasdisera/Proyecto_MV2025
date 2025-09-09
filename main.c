#include "common.h"
#include "Processor/processor.h"
#include "Memory/mem.h"
#include "InstrucSet/instruct.h"
#include "Utils/disassembler.h"

int main(int argc, char *argv[])
{
    cpu_t cpu;
    mem_t mem;
    int result,disassembler,carga;

    cpu_init(&cpu);
    mem_init(&mem);

    carga = mem_load(&mem,argv[1],&cpu);
    if (carga != 0) {
        printf("Error %d: Fallo al cargar el programa '%s'.\n", carga,argv[1]);
        return 0;
    }
    
    if (disassembler) {
        
        return 0;
    }

    int CS = cpu.CS >> 4;
    
    while (cpu.IP < mem.segments[CS].size) { 
        
        result = cpu_step(&cpu, &mem);
    
        if (result != 0) {
            // Instrucción inválida, división por cero, fallo de segmento
            printf("Error durante la ejecución. Código: %d\n", result);
            return 0;
        }
    }

   
    return 0;
}