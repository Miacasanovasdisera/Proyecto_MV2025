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
    
    // Verificar que no se salga de la memoria física
    if ((uint32_t)physical_addr + bytesToRead > mem.size) {
        error_Output(MEMORY_ERROR);
        return 0;
    }

    return physical_addr;
}

void operators_registers_load(cpu_t *cpu, mem_t mem) { //*modificacion
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
        
    cpu->OP1 = (typeOP1 << 24) | ((int32_t)dataOP1 & 0x00FFFFFF);
    cpu->OP2 = (typeOP2 << 24) | ((int32_t)dataOP2 & 0x00FFFFFF);

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

void write_register(cpu_t *cpu, uint8_t reg_code, uint32_t value) { //*modificacion
    int8_t sector = (reg_code >> 6) & 0x03, base_reg = reg_code & 0x3F;
    int32_t *reg_ptr; // se usa el puntero para modificar el registro de proposito general, solo los bits necesarios

    if (base_reg >= R_EAX && base_reg <= R_EFX) {
        switch (base_reg) {
            case R_EAX: reg_ptr = &cpu->EAX; break;
            case R_EBX: reg_ptr = &cpu->EBX; break;
            case R_ECX: reg_ptr = &cpu->ECX; break;
            case R_EDX: reg_ptr = &cpu->EDX; break;
            case R_EEX: reg_ptr = &cpu->EEX; break;
            case R_EFX: reg_ptr = &cpu->EFX; break;
        }
        switch (sector) {
            case 0: *reg_ptr = value; break; // EAX
            case 1: *reg_ptr = (*reg_ptr & 0xFFFFFF00) | (value & 0xFF); break; // AL
            case 2: *reg_ptr = (*reg_ptr & 0xFFFF00FF) | (value << 8 & 0xFF00); break; // AH 
            case 3: *reg_ptr = (*reg_ptr & 0xFFFF0000) | (value & 0xFFFF); break; // AX
        }
        return;
    }

    switch (reg_code) {
        case R_LAR: cpu->LAR = value; break;
        case R_MAR: cpu->MAR = value; break;
        case R_MBR: cpu->MBR = value; break;
        case R_IP:  cpu->IP  = value; break;
        case R_SP:  cpu->SP  = value; break;
        case R_BP:  cpu->BP  = value; break;
        case R_CC:  cpu->CC  = value & 0xC0000000; break;
        case R_AC:  cpu->AC  = value; break;
        case R_CS:  cpu->CS  = value; break;
        case R_DS:  
            if ((value & 0xFFFF0000) <= 0x00070000) {
                cpu->DS = value;
            }    
        break;
        case R_ES:  cpu->ES  = value; break;
        case R_SS:  cpu->SS  = value; break;
        case R_KS:  cpu->KS  = value; break;
        case R_PS:  cpu->PS  = value; break;
        default: {
            //Error de registro
            error_Output(REGISTER_ERROR); 
            return;
        }
    }
}

int32_t read_register(cpu_t *cpu, uint8_t reg_code) { //*modificacion
    int8_t sector = (reg_code >> 6) & 0x03, base_reg = reg_code & 0x3F;
    int32_t value;
    switch (reg_code) {
        case R_EAX: value = cpu->EAX;break;
        case R_EBX: value = cpu->EBX;break;
        case R_ECX: value = cpu->ECX;break;
        case R_EDX: value = cpu->EDX;break;
        case R_EEX: value = cpu->EEX;break;
        case R_EFX: value = cpu->EFX;break;
        case R_SP : value = cpu->SP; break;
        case R_BP : value = cpu->BP; break;
        case R_CS : value = cpu->CS; break;
        case R_DS : value = cpu->DS; break;
        case R_ES : value = cpu->ES; break;
        case R_SS : value = cpu->SS; break;
        case R_KS : value = cpu->KS; break;
        case R_PS : value = cpu->PS; break;        
        case R_AC : value = cpu->AC; break;
        case R_CC : value = cpu->CC; break;        
        case R_IP : value = cpu->IP; break;        
        case R_MAR: value = cpu->MAR;break;
        case R_MBR: value = cpu->MBR;break;
        case R_LAR: value = cpu->LAR;break;
        case R_OPC: value = cpu->OPC;break;
        case R_OP1: value = cpu->OP1;break;
        case R_OP2: value = cpu->OP2;break;
        default: return error_Output(REGISTER_ERROR);
    }

    if (base_reg >= R_EAX && base_reg <= R_EFX) {
        switch (sector) {
            case 0: return value; // EAX
            case 1: return value & 0xFF; // AL
            case 2: return (value >> 8) & 0xFF; // AH 
            case 3: return value & 0xFFFF; // AX
        }
    }

    return value;
}