#include <stdio.h>
#include "Processor/processor.h"
#include "Memory/mem.h"
#include "InstrucSet/instruct.h"
#include "Utils/disassembler.h"

int main(int argc, char *argv[])
{
    cpu_t cpu;
    mem_t mem;

    cpu_init(&cpu);
    mem_init(&mem);

    mem_load(&mem,argv[1],&cpu);
        

    return 0;
}

// main.c 3 ./vmx program.vmx -d