#include "Utils/common.h"
#include "Processor/processor.h"
#include "Memory/mem.h"
#include "InstrucSet/instruct.h"
#include "Utils/disassembler.h"
#include "Utils/errors.h"
#include "Utils/config.h"

int main(int argc, char *argv[])
{
    cpu_t cpu;
    mem_t mem;
    vm_config_t config;

    config_init(&config);
    config_parse(&config, argc, argv); 
    config_validate(&config);

    cpu_init(&cpu);
    mem_init(&mem);
    mem_load(&mem, config.vmx_file, &cpu);
    InstrucSet_init();

    printf("\n");

    if (config.disassem_mode)
        disassembler(cpu, mem);
    
    printf("\n");
    
    int result,CS = cpu.CS >> 16;

    do {
        operators_registers_load(&cpu,mem);
        result = execute_instruction(&cpu,&mem);
    } while ((uint32_t)cpu.IP < mem.segments[CS].size);

    mem_free(&mem);
    config_free(&config);

    return 0;
}