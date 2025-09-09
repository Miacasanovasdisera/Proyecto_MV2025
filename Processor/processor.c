#include "processor.h"
#include "../Memory/mem.h"

void cpu_init(cpu_t *cpu){
    
    // Setea todo en 0
    memset(cpu,0,sizeof(cpu_t));
    
}
void cpu_update_IP(cpu_t *cpu){
    cpu->IP =  cpu->OP1|cpu->OP2|0x01;
}

int32_t cpu_logic_to_physic(mem_t mem,int32_t logic_address,int bytesToRead) {
    int16_t segment, offset,base;
    int32_t dir_physic;

    segment = logic_address >> 16;
    offset = logic_address & 0xFF;
    base = mem.segments[segment].base;

    dir_physic = (int32_t) base + offset;

    if (dir_physic + bytesToRead > mem.segments[segment].base && dir_physic < mem.segments[segment].size)
        return dir_physic;
    
    printf("Segmentation fault\n");
    exit(1);
}

