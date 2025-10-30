#include "helpers.h"
#include "../Processor/operands.h"
#include "../Utils/errors.h"

void get_destination_address(cpu_t *cpu, int32_t OP, uint32_t *dest_addrss) {
    int32_t dataOP;
    int8_t typeOP;

    typeOP = get_operand_type(OP);
    dataOP = get_operand_value(OP);

    if (typeOP == MEMORY_OPERAND) 
        *dest_addrss = calculate_logical_address(cpu, typeOP, dataOP);
    
    else if (typeOP == REGISTER_OPERAND) 
        *dest_addrss = dataOP;
    
    else {
        error_Output(INVALID_OPERAND); 
        return;
    }
}

int auxiliar_SAR(cpu_t *cpu, int32_t value, int32_t num_bits) {
    // shift aritmetico a la derecha
    int32_t result = value;
    
    if(result & 0x00800000)
        result = result | 0xFF000000;
    
    if (num_bits >= 32)
        result = (value < 0) ? -1 : 0; 
    
    else
        for (int i = 0; i < num_bits; i++)
            if(result & 0x80000000){
                result = (result >> 1);
                result = result | 0x80000000;
            }
            else
                result = (result >> 1);
    
    if(result & 0x80000000)
        result = result & 0x00FFFFFF;
   
    return result;
}

void get_value(cpu_t *cpu, mem_t *mem, int32_t OP, int32_t *content) {
    int32_t dataOP;
    uint32_t logic_addr;
    uint8_t typeOP,verif = 0;

    dataOP = get_operand_value(OP);
    typeOP = get_operand_type(OP);
    
    if (dataOP & 0x800000) {
        verif = 1;
        dataOP = abs(dataOP); //Extiende signo
    }


    switch (typeOP) {
        case REGISTER_OPERAND: 
            *content = read_register(cpu, dataOP);    
        break;

        case IMMEDIATE_OPERAND: {
            if (dataOP < -32768 || dataOP > 32767)
                error_Output(WRONG_SIZE);
            
            *content = verif ? (-1)*dataOP : dataOP;
        }
        break;
        
        case MEMORY_OPERAND: { //*modificacion
            logic_addr = calculate_logical_address(cpu, typeOP, dataOP);
            
            // Obtener tamaño del modificador
            int size = get_memory_size(OP);
           
            // Leer con el tamaño especificado
            mem_read(mem, cpu, logic_addr, content, size);
            
            // Extensión de signo si es necesario
            if (size == 1 && (*content & 0x80))
                *content |= 0xFFFFFF00; // Extender signo de 8 bits
            else if (size == 2 && (*content & 0x8000))
                *content |= 0xFFFF0000; // Extender signo de 16 bits  
        } 
        break;

        default: {
            error_Output(INVALID_OPERAND);
            return;
        }
    }
}

void write_dest(cpu_t *cpu, mem_t *mem, uint8_t type, uint32_t dest_addrss, int32_t value, int32_t OP) { //*modificacion
    if (type == MEMORY_OPERAND)
        mem_write(mem, cpu, dest_addrss, value, get_memory_size(OP));
    
    else if (type == REGISTER_OPERAND) 
        write_register(cpu, dest_addrss, value);

    else {
        error_Output(INVALID_OPERAND);
        return;
    }
}

void update_CC(cpu_t *cpu,int32_t result) { 
    cpu->CC &= ~(NMask| ZMask); //Limpia los bits N y Z
    if(result == 0)
        cpu->CC |= ZMask;
    
    else if(result < 0)
        cpu->CC |= NMask;
}

int get_memory_size(uint32_t OP_register) { //*modificacion
    uint8_t size_code = (OP_register >> 22) & 0x03; // Bits 23-24

    switch (size_code) {
        case 0: return 4; // 00 - long (4 bytes)
        case 2: return 2; // 10 - word (2 bytes)
        case 3: return 1; // 11 - byte (1 byte)
        default: return 4; // Por defecto, 4 bytes
    }
}