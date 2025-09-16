#include "Utils/common.h"
#include "Processor/processor.h"
#include "Memory/mem.h"
#include "InstrucSet/instruct.h"
#include "Utils/disassembler.h"
#include "Utils/errors.h"

int main(int argc, char *argv[])
{
    cpu_t cpu;
    mem_t mem;
    char *disassembler = argc > 2 ? argv[2] : "";

    cpu_init(&cpu);
    mem_init(&mem);
    mem_load(&mem, argv[1], &cpu); 
    
    /*
    int result,CS = cpu.CS >> 16; //antes estaba 4
    
    while (cpu.IP < mem.segments[CS].size) { 
        
        operators_registers_load(&cpu,mem);
        
        result = execute_instruction(&cpu,&mem);
    }   
    */
    return 0;
}