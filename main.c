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
    /* 
    int CS = cpu.CS >> 16; //antes estaba 4
    
    while (cpu.IP < mem.segments[CS].size) { 
        
        Operators_Registers_Load(mem,&cpu);
        
        result = execute_instruction(&cpu,&mem);
    
        if (result != 0) {
            
            // Manejar errores
            break;
        }
    }
    */
    return 0;
}