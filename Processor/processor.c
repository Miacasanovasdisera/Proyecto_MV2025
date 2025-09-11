#include "processor.h"
#include "../Memory/mem.h"

void cpu_init(cpu_t *cpu) {
    memset(cpu, 0, sizeof(cpu_t));
}

void cpu_update_IP(cpu_t *cpu, int8_t typeOP1, int8_t typeOP2) {
    cpu->IP = cpu->IP + typeOP1 + typeOP2 + 1;
}

int32_t cpu_logic_to_physic(mem_t mem, int32_t logic_address, int bytesToRead) {
    int16_t segment, offset, base;
    int32_t dir_physic;

    segment = logic_address >> 16;
    offset = logic_address & 0xFF;
    base = mem.segments[segment].base;

    dir_physic = (int32_t)base + offset;

    if (dir_physic + bytesToRead > mem.segments[segment].base && dir_physic < mem.segments[segment].size)
        return dir_physic;

    printf("Segmentation fault\n");
    exit(1);
}

void Operators_Registers_Load(mem_t mem, cpu_t *cpu) {
    int8_t i, increment, firstByte = mem.data[cpu->IP];
    uint32_t typeOP1, typeOP2;
    int32_t dataOP1, dataOP2, a, b;

    cpu->OPC = firstByte & 0x1F;
    
    typeOP2 = (firstByte & 0xc0) >> 6;
    typeOP1 = (firstByte >> 4) & 0x03;

    increment = cpu->IP + 1;
    a = mem.data[increment];

    for (i = 1; i < typeOP2; i++) {
        a = a << 8;
        b = mem.data[increment + i];
        a = a | b;
    }

    dataOP2 = a;
    increment += i;

    if (typeOP1 == 0) {
        typeOP1 = typeOP2;
        typeOP2 = 0;

        dataOP1 = dataOP2;
        dataOP2 = 0;
    }

    else {
        a = mem.data[increment];
        for (i = 1; i < typeOP1; i++) {
            a = a << 8;
            b = mem.data[increment + i];
            a = a | b;
        }

        dataOP1 = a;
    }

    cpu->OP1 = (typeOP1 << 24) | dataOP1;
    cpu->OP2 = (typeOP2 << 24) | dataOP2;

    cpu_update_IP(cpu, typeOP1, typeOP2);
}

int32_t Aritmetic_Shifter(int32_t K, int n) {
    int32_t X = K;
    int i;
    
    X = X >> 31;
    
    if (X != 0)
        for (i = 1; i<n ;i++) {
            K = K >> 1;
            K = K & 0x80000000;
        }
    
    else
        K >> n;
}

int8_t get_operand_type(int32_t OP_register) {
    return (OP_register >> 30) & 0x03;
}

int32_t get_operand_value(int32_t OP_register) {
    return OP_register & 0x3FFFFFFF;
}