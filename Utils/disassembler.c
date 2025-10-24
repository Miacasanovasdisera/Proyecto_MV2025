#include "disassembler.h"
#include "errors.h"
#include "../Processor/operands.h"

// Esta funcion imprime un operando segun su tipo

void print_operand(uint32_t op) {
    uint32_t typeOP = get_operand_type(op);
    int32_t dataOP = get_operand_value(op);
    int8_t reg;
    int16_t offset;

    if (g_version==2)
        switch (typeOP) {
            case NO_OPERAND:  // sin operando
            break;

            case IMMEDIATE_OPERAND:  // inmediato en decimal
                printf("%X", dataOP);
            break;

            case REGISTER_OPERAND:{  // registro 
                uint8_t reg_index = dataOP & 0x1F;      // bits 0-4 
                uint8_t pseudonimo = (dataOP >> 5) & 0x03; // bits 5-6 
                
                const char **name_array;
                switch(pseudonimo) {
                    case 0b01: name_array = register_name_16; break; 
                    case 0b10: name_array = register_name_8L; break;
                    case 0b11: name_array = register_name_8H; break;
                    default: name_array = register_name; break;
                }

                if (reg_index < 32 && strcmp(name_array[reg_index],"?") != 0) 
                    printf("%s", name_array[reg_index]);
                else
                    printf("?"); 
            } break;

            case MEMORY_OPERAND:{
                int16_t offset = dataOP & 0x0000FFFF;
                uint8_t modifiers = (dataOP >> 16) ;
                uint8_t reg_idx = dataOP >> 16;
             

                uint8_t size = (modifiers & 0xC0) >> 6;          // bits 8-7 
                
                printf("%s", size_prefix[size]); // Prefijo de tamaño (l , w , b)
                printf("[%s", register_name[reg_idx]);
                
                if (offset > 0) 
                    printf("+%d", offset);
                else if (offset < 0) 
                    printf("%d", offset);
                
                printf("]");
                
            } break;

            default: printf("?"); break;
        }             
    
    else 
        switch (typeOP) {
            case NO_OPERAND:  // sin operando
            break;
            
            case IMMEDIATE_OPERAND:  // inmediato en decimal
            printf("%X", dataOP);
            break;
            
            case REGISTER_OPERAND:  // registro
            if (dataOP < 32 && strcmp(register_name[dataOP],"?")!=0) { //menos por ahora pero
                printf("%s", register_name[dataOP]);
            } else {
                printf("?");
            }
            break;
            
            case MEMORY_OPERAND:{
                reg = dataOP >> 16;
                offset = dataOP & 0x0000FFFF;
                printf("[%s+", register_name[reg]);
                printf("%d]",offset);
            }     // memoria
            break;
            
            default: error_Output(INVALID_OPERAND);    
    
        }
    
}

void disassembler(cpu_t cpu, mem_t mem){
    int8_t i, instrucSize, typeOP1, typeOP2; // declaro lo que se que va a ser usado en ambas versiones
        //segmento de constantes
    if (cpu.KS != 0xFFFFFFFF && cpu.KS != 0 ) {
        uint32_t offset, ks_base = offset = mem.segments[cpu.KS >> 16].base;
        uint32_t ks_size = mem.segments[cpu.KS >> 16].size;
        uint32_t len;
        while (offset < ks_size) {
            printf("[%04X]", offset);
            uint32_t str_phys_addr = ks_base + offset;
            
            len = 0;
            
            while ((offset + len) < ks_size && mem.data[str_phys_addr + len] != '\0')
                len++;
            
            len++; // P incluir el \0

            int hex_len = (len > 7) ? 6 : len; //para formato, porq si la longitud es mayor a 7, se muestran 6 catacteres y desp .. 
            for (i = 0; i < hex_len; i++)
                printf("%02X ", mem.data[str_phys_addr + i]);
        
            if (len > 7) 
                printf(".. ");
            
            for (i = hex_len + (len > 7 ? 1 : 0); i < 8; i++)
                printf("   ");

            printf("| \"");
            for (i = 0; i < len - 1; i++) {
                char c = mem.data[str_phys_addr + i];
                printf("%c", c);
            }
            printf("\"\n");

                offset += len;
        }
    }
    //segmento de codigo
    uint32_t cs_base = mem.segments[cpu.CS >> 16].base;
    uint32_t cs_size = mem.segments[cpu.CS >> 16].size;
    uint32_t offset = cpu.IP;  // offset logico 
    
    printf(">");

    while (offset < cs_size) { 
        printf("[%04X] ", offset);

        uint32_t physical_addr = cs_base + offset;
        cpu.IP = physical_addr; // operators_registers_load lee desde cpu.IP

        operators_registers_load(&cpu, mem);
        // carga OPC, OP1, OP2 y actualiza IP
                
        typeOP1 = get_operand_type(cpu.OP1);
        typeOP2 = get_operand_type(cpu.OP2);
        instrucSize = 1 + typeOP1 + typeOP2; //tamaño de la instruccion
        
        // 2. instruccion en hex
        for (i = 0; i < instrucSize; i++)
            printf("%02X ", mem.data[offset + i]);

        // Formato
        for (i = instrucSize; i < 8; i++)
            printf("   ");

        printf("| ");

        const char *mnemonic = opcode_name[cpu.OPC];
        printf("%s ", mnemonic);

        if (typeOP1 != NO_OPERAND) 
            print_operand(cpu.OP1);
                
        if (typeOP2 != NO_OPERAND) {
            printf(", ");
            print_operand(cpu.OP2);
        }   

        printf("\n ");
        offset += instrucSize; // Avanzar offset lógico
    } 

}

/*
void print_operand(uint32_t op) {
    uint32_t typeOP = get_operand_type(op);
    int32_t dataOP = get_operand_value(op);
    int8_t reg;
    int16_t offset;
    
    switch (typeOP) {
        case NO_OPERAND:  // sin operando
            break;

        case IMMEDIATE_OPERAND:  // inmediato en decimal
            printf("%X", dataOP);
            break;

        case REGISTER_OPERAND:  // registro
            if (dataOP < 32 && strcmp(register_name[dataOP],"?")!=0) { //menos por ahora pero
                printf("%s", register_name[dataOP]);
            } else {
                printf("?");
            }
            break;

        case MEMORY_OPERAND:{
            reg = dataOP >> 16;
            offset = dataOP & 0x0000FFFF;
            printf("[%s+", register_name[reg]);
            printf("%d]",offset);
        }     // memoria
            break;

        default:
            error_Output(INVALID_OPERAND);
    }
}

void disassembler(cpu_t cpu, mem_t mem) {
    uint32_t codsize =  mem.segments[0].size, increment = cpu.IP;
    int8_t i, instrucSize, typeOP1, typeOP2;

    while (increment < codsize) {
        printf("[%04X] ", increment);        

        operators_registers_load(&cpu, mem);
         // carga OPC, OP1, OP2 y actualiza IP
        typeOP1 = get_operand_type(cpu.OP1);
        typeOP2 = get_operand_type(cpu.OP2);
        instrucSize = 1 + typeOP1 + typeOP2; //tamaño de la instruccion
        // 2. instruccion en hex
        for (i = 0; i < instrucSize; i++) {
            printf("%02X ", mem.data[increment + i]);
        }

        // Formato
        for (i = instrucSize; i < 8; i++) {
            printf("   ");
        }

        printf("| ");

        // 3 mnemonico
        const char *mnemonic = opcode_name[cpu.OPC];
            printf("%s ", mnemonic);

        // 4. Operando 1
        if (typeOP1 != NO_OPERAND) {
            print_operand(cpu.OP1);
        }

        // 5. Operando 2
        if (typeOP2 != NO_OPERAND) {
            printf(", ");
            print_operand(cpu.OP2);
        }   

        printf("\n");
        // Actualizo el incrementador
        increment += 1 + typeOP1 + typeOP2;
    }
}
*/