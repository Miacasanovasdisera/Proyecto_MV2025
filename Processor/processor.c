#include "processor.h"
#include "../Memory/mem.h"

void cpu_init(cpu_t *cpu){
    
    // Setea todo en 0
    memset(cpu,0,sizeof(cpu_t));
    
}

void cpu_update_IP(cpu_t *cpu,int8_t typeOP1, int8_t typeOP2){
    cpu->IP =  cpu->IP + typeOP1 + typeOP2 + 1;
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

void Operators_Registers_Load(mem_t mem, cpu_t *cpu) {
    int8_t firstByte = mem.data[cpu->IP];
    int8_t typeOP1, typeOP2;

    cpu->OPC = firstByte & 0x1F;
    typeOP2 = firstByte >> 6;
    typeOP1 = (firstByte >> 4) & 0x30;

    switch (typeOP2) {
    case 0b01:
        cpu->OP1 = mem.data[cpu->IP+1];
        break;
    case 0b10:
        cpu->OP2 = mem.data[cpu->IP+1] << 16 & mem.data[cpu->IP+2];
        cpu->OP1 = mem.data[cpu->IP+3];
        break;
        
    default:
        break;
    }
}