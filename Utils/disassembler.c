#include "disassembler.h"
#include "errors.h"
#include "../Processor/operands.h"

// Esta funcion imprime un operando segun su tipo

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
    uint32_t codsize =  mem.segments[0].size, increment = cpu.IP = cpu.CS;
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