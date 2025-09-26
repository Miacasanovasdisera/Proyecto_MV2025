#include "processor.h"
#include "../Memory/mem.h"
#include "registers.h"
#include "operands.h"
#include "../Utils/errors.h"
#include "../InstrucSet/Operations/shiftModule.h"

void cpu_init(cpu_t *cpu) {
    memset(cpu, 0, sizeof(cpu_t));
}

void cpu_update_IP(cpu_t *cpu, int8_t typeOP1, int8_t typeOP2) {
    cpu->IP = cpu->IP + typeOP1 + typeOP2 + 1;
}

uint16_t cpu_logic_to_physic(mem_t mem, uint32_t logic_address, int bytesToRead) {
    uint16_t segment = (uint16_t)(logic_address >> 16);
    uint16_t offset = (uint16_t)(logic_address & 0xFFFF);
    
    // Verificar que el segmento sea válido (0-7)
    if (segment >= 8) {
        error_Output(MEMORY_ERROR);
        return 0;
    }
    
    uint16_t base = mem.segments[segment].base;
    uint16_t size = mem.segments[segment].size;

    // Verificar que el acceso completo esté dentro del segmento
    if ((uint32_t)offset + bytesToRead > size) {
        error_Output(MEMORY_ERROR);
        return 0;
    }
    
    // Calcular dirección física
    uint32_t physical_addr = base + offset;
    
    // Verificar que no se salga de la memoria física (16KB = 0x4000)
    if ((uint32_t)physical_addr + bytesToRead > MEM_SIZE) {
        error_Output(MEMORY_ERROR);
        return 0;
    }
    

    return physical_addr;
}

void operators_registers_load(cpu_t *cpu, mem_t mem) {
    uint8_t i, increment,  typeOP1, typeOP2, firstByte = mem.data[cpu->IP];
    uint32_t dataOP1, dataOP2, a, b;

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
    
    if(typeOP1 == IMMEDIATE_OPERAND)
        if(dataOP1 & 0x8000){
            dataOP1 = dataOP1 << 8;
            dataOP1 = shift_SAR(cpu,dataOP1,8);
        }
                                                       // caso particular de inmediatos negativos      
    if(typeOP2 == IMMEDIATE_OPERAND)
        if(dataOP2 & 0x8000){
            dataOP2 <<= 8;
            dataOP2 = shift_SAR(cpu,dataOP2,8);            
        }
        
    cpu->OP1 = (typeOP1 << 24) | (int32_t)dataOP1;
    cpu->OP2 = (typeOP2 << 24) | (int32_t)dataOP2;

    cpu_update_IP(cpu, typeOP1, typeOP2);
}

int8_t get_operand_type(uint32_t OP_register) {
    return (OP_register >> 24) & 0x03;
}

int32_t get_operand_value(uint32_t OP_register) {
    return (int32_t)((OP_register & 0x00FFFFFF) << 8) >> 8;
}

uint32_t calculate_logical_address(cpu_t *cpu, uint8_t OP_type, uint32_t OP_value) {
    // Solo los operandos de memoria necesitan dirección lógica
    if (OP_type != MEMORY_OPERAND) {
        error_Output(INVALID_OPERAND);
        return 0;
    }
    
    uint32_t value_reg, reg_code = OP_value & 0x00FF0000;
    uint16_t offset = OP_value & 0xFFFF;

    value_reg = read_register(cpu, (uint8_t)(reg_code >> 16));
    
    return value_reg + offset;
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
        case R_OPC: return cpu->OPC;
        case R_OP1: return cpu->OP1;
        case R_OP2: return cpu->OP2;
        default: return error_Output(REGISTER_ERROR);
    }
}