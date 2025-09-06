#include <stdio.h>
#include "Processor/processor.h"
#include "Memory/mem.h"
#include "InstrucSet/instruct.h"
#include "Utils/disassembler.h"

int main(int argc, char *argv[])
{
    cpu_t cpu;
    mem_t mem;
    int result;

    cpu_init(&cpu);
    mem_init(&mem);

    if (mem_load(&mem,argv[1],&cpu) != 0) {
        fprintf(stderr, "Error: Fallo al cargar el programa '%s'.\n", argv[1]);
        return 0;
    }
    
    // Mientras no se ejecute STOP
    while (cpu.IP != 0xFFFFFFFF) { 
        
        result = cpu_step(&cpu, &mem);
        
        if (result != 0) {
            // Instrucción inválida, división por cero, fallo de segmento
            fprintf(stderr, "Error durante la ejecución. Código: %d\n", result);
            return 0;
        }
    }

    return 0;
}