#include "helpers.h"
#include "../Processor/operands.h"

int get_destination_address(cpu_t *cpu, int32_t OP, int32_t *dest_addrss) {     // obtiene la direccion del destino para luego sobreescribirlo
    int32_t dataOP;
    int8_t typeOP;

    typeOP = get_operand_type(OP);
    dataOP = get_operand_value(OP);

    if (typeOP == MEMORY_OPERAND) {
        *dest_addrss = calculate_logical_address(cpu, typeOP, dataOP);
        return 0;
    }

    else if (typeOP == REGISTER_OPERAND) {
        *dest_addrss = dataOP;
        return 0;
    }
    else {
        return 9;
    }
}

int get_value(cpu_t *cpu, int32_t OP, mem_t *mem, int32_t *content){        // obtiene el valor contenido en un registro o una direccion de memoria (si es un inmediato lo devuelve)
    int32_t dataOP,logic_addr;
    int8_t typeOP;

    dataOP = get_operand_value(OP);
    typeOP = get_operand_type(OP);
    
    switch (typeOP)
        {
        case REGISTER_OPERAND:
            *content = read_register(cpu, dataOP);      
            break;

        case IMMEDIATE_OPERAND:
            *content = dataOP;
            break;
        
        case MEMORY_OPERAND:{
            logic_addr = calculate_logical_address(cpu,typeOP,dataOP);
            *content = mem_read(mem,cpu,logic_addr,content,4);
        }break;
    
    }

    
}

int write_dest(cpu_t *cpu, mem_t *mem, int8_t type, int32_t dest_addrss, int32_t value){   // escribe en la direccion obtenida por la funcion (la de arriba) el valor 
    
    if (type == MEMORY_OPERAND) {
        return mem_write(mem, cpu, dest_addrss, value, 4);
    }

    else if (type == REGISTER_OPERAND) {
        write_register(cpu, dest_addrss, value);
        return 0;
    }

    else {
        return 9;
    }
}

void update_CC(cpu_t *cpu,int32_t result){      // funcion que me modifica el registro cc 

    if(result == 0)
        cpu->CC = 0x40000000;
    else{                                   
        if(result < 0)
            cpu->CC = 0x80000000;
    }

}





