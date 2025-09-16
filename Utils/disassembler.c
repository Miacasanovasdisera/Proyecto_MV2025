#include "../Utils/disassembler.h"
#include "../errors.h"
#include "../InstrucSet/opcod.h"
#include "../InstrucSet/registers.h"
#include "../Memory/mem.h"
#include "../Processor/operands.h"


// Se usan las tablas definidas en opcod.h y registers.h:
//    const char *opcode_name[];
//    const char *register_name[];

// Esta funcion imprime un operando segun su tipo

 void print_operand(uint32_t op, int type, char *register_name[]) {
    switch (type) {
        case OP_NONE:
            break;

        case IMMEDIATE_OPERAND:  // inmediato en decimal
            printf("%d", op);
            break;

        case REGISTER_OPERAND:  // registro
            if (op < 32) { //menos por ahora pero
                printf("%s", register_name[op]);
            } else {
                printf("R?");
            }
            break;

        case MEMORY_OPERAND:  // memoria
            printf("[DS+%d]", op); // considero que la base es ds
            break;

        default:
            printf("?");
            break;
    }
}



void disassemble(int8_t instr_len,int32_t physical_addr, int8_t OP1, int8_t OP2, int8_t typeOP1, int8_t typeOP2, int8_t OPC, mem_t mem, char *register_name[],char *opcode_name[]) { //instr_len = 1 + typeOP1 + typeOP2;
    
    // 1. direc fisica
    printf("[%04X] ", physical_addr);

    // 2. instruccion en hex
   for (int i = 0; i < instr_len; i++) {
        printf("%02X ", mem->data[i]);
    }

    printf("| ");

    // 3 mnemonico
    const char *mnemonic = opcode_name[OPC];
    printf("%s ", mnemonic);

    // 4. Operando 1
    if (typeOP1 != NO_OPERAND) {
        print_operand(OP1, typeOP1, register_name);
    }

    // 5. Operando 2
    if (typeOP2 != NO_OPERAND) {
        printf(", ");
        print_operand(OP2, typeOP2 , register_name);
    }

    printf("\n");
}