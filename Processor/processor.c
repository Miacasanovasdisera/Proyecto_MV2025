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
    int8_t i, increment, firstByte = mem.data[cpu->IP];
    int32_t typeOP1, typeOP2, dataOP1, dataOP2, a, b;

    printf("IP1->%x \n",cpu->IP);

    cpu->OPC = firstByte & 0x1F;
    typeOP2 = firstByte >> 6;
    typeOP1 = (firstByte >> 4) & 0x03;
    printf("tipo de op1 %d",typeOP1,"\n tipo de op2 %d \n",typeOP2);
    increment = cpu->IP+1;
    a = mem.data[increment];
    
    for(i = 1; i < typeOP2 ;i++){
        a = a << 8;
        b = mem.data[increment+i];
        a = a | b;
    }

    dataOP2 = a;
    increment += i;

    if(typeOP1 == 0){
        typeOP1 = typeOP2;
        typeOP2 = 0;

        dataOP1 = dataOP2;
        dataOP2 = 0;
    }

    else {
        a = mem.data[increment];
        for(i = 1; i < typeOP1 ;i++){
            a = a << 8;
            b = mem.data[increment+i];
            a = a | b;
        }

        dataOP1 = a;
    }

    cpu->OP1 = (typeOP1 << 24) | dataOP1;
    cpu->OP2 = (typeOP2 << 24) | dataOP2;

    cpu_update_IP(cpu,typeOP1,typeOP2);
    
    printf("OPC->%x \n",cpu->OPC);
    printf("OP1->%x \n",cpu->OP1);
    printf("OP2->%x \n",cpu->OP2);
    
    printf("IP2->%x \n",cpu->IP);

}