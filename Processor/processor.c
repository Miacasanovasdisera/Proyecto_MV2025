#include "processor.h"
#include "../Memory/mem.h"
#include "registers.h"
#include "operands.h"
#include "../Utils/errors.h"

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

    return error_Output(MEMORY_ERROR); //Fallo de segmento
}

void operators_registers_load(cpu_t *cpu, mem_t mem) {
    int8_t i, increment,  typeOP1, typeOP2, firstByte = mem.data[cpu->IP];
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

int8_t get_operand_type(int32_t OP_register) {
    return (OP_register >> 30) & 0x03;
}

int32_t get_operand_value(int32_t OP_register) {
    return OP_register & 0x3FFFFFFF;
}

int32_t calculate_logical_address(cpu_t *cpu, uint8_t OP_type, uint32_t OP_value) {
    int16_t segment_selector;
    int16_t offset;
    
    //Determina segmento
    if (OP_type == MEMORY_OPERAND) 
        segment_selector = (cpu->DS >> 16) & 0xFFFF;
    
    else 
        segment_selector = (cpu->CS >> 16) & 0xFFFF;
    
    //Extrae EL OFFSET del valor del operando
    if (OP_type == IMMEDIATE_OPERAND) 
        offset = OP_value & 0xFFFF;

    else if (OP_type == MEMORY_OPERAND) 
        offset = OP_value & 0xFFFF;

    else if (OP_type == REGISTER_OPERAND) 
        offset = read_register(cpu, OP_value) & 0xFFFF;
    
    //Ensambla dir logica
    return (segment_selector << 16) | offset;
}

void write_register(cpu_t *cpu, uint8_t reg_code, uint32_t value) {
    int verif = 0;
    switch (reg_code) {
        case R_LAR: cpu->LAR = value; break;
        case R_MAR: cpu->MAR = value; break;
        case R_MBR: cpu->MBR = value; break;
        case R_IP:  cpu->IP  = value; break;
        case R_EAX: cpu->EAX = value; break;
        case R_EBX: cpu->EBX = value; break;
        case R_ECX: cpu->ECX = value; break;
        case R_EDX: cpu->EDX = value; break;
        case R_EEX: cpu->EEX = value; break;
        case R_EFX: cpu->EFX = value; break;
        case R_CC:  cpu->CC  = value & 0xC0000000; break;
        case R_AC:  cpu->AC  = value; break;
        case R_CS:  cpu->CS  = value; break;
        case R_DS:  
            if ((value & 0xFFFF0000) <= 0x00070000) {
                cpu->DS = value;
            }    
        break;
        default: {
            //Error de registro
            error_Output(REGISTER_ERROR); 
            return;
        }
    }
}

int32_t read_register(cpu_t *cpu, uint8_t reg_code) {
    switch (reg_code) {
        case R_EAX: return cpu->EAX;
        case R_EBX: return cpu->EBX;
        case R_ECX: return cpu->ECX;
        case R_EDX: return cpu->EDX;
        case R_EEX: return cpu->EEX;
        case R_EFX: return cpu->EFX;
        case R_CS : return cpu->CS;
        case R_DS : return cpu->DS;        
        case R_AC : return cpu->AC;
        case R_CC : return cpu->CC;        
        case R_IP : return cpu->IP;        
        case R_MAR: return cpu->MAR;
        case R_MBR: return cpu->MBR;
        case R_LAR: return cpu->LAR;
        default:
            return error_Output(REGISTER_ERROR);
        break; //Error de registro
    }
}