#include "../Utils/disassembler.h"
#include "errors.h"
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
        case NO_OPERAND:  // sin operando
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

// Decodifica la instruccion en OPC, OP1, OP2, typeOP1, typeOP2 y actualiza el IP
void Decode(cpu_t *cpu, mem_t *mem,int8_t *OP1, int8_t *OP2, int8_t *typeOP1, int8_t *typeOP2, int8_t *OPC) {
    int8_t i, increment, firstByte = mem->data[cpu->IP];
    int32_t dataOP1, dataOP2, a, b, typeOP1, typeOP2;

    *OPC = firstByte & 0x1F;

    *typeOP2 = (firstByte & 0xc0) >> 6;
    *typeOP1 = (firstByte >> 4) & 0x03;

    increment = cpu->IP + 1;
    a = mem->data[increment];

    for (i = 1; i < *typeOP2; i++) {
        a = a << 8;
        b = mem->data[increment + i];
        a = a | b;
    }
    dataOP2 = a;
    increment += i;

    if (*typeOP1 == 0) {
        *typeOP1 = typeOP2;
        *typeOP2 = 0;

        dataOP1 = dataOP2;
        dataOP2 = 0;
    }

    else {
        a = mem->data[increment];
        for (i = 1; i < *typeOP1; i++) {
            a = a << 8;
            b = mem->data[increment + i];
            a = a | b;
        }

        dataOP1 = a;
    }

    *OP1 = (typeOP1 << 24) | dataOP1;
    *OP2 = (typeOP2 << 24) | dataOP2;

    cpu_update_IP(cpu, typeOP1, typeOP2);
}

// Desensambla el codigo desde la posicion actual del IP hasta el final del segmento de codigo
void disassemble(cpu_t *cpu, mem_t *mem) { //se va a implementar antes de la ejecucion de cada instruccion
    uint32_t codsize = mem->segments[0].size; // tamaño del segmento de codigo
    int8_t i, instrucSize, typeOP1, typeOP2, increment = cpu->IP = cpu->CS; // inicio del segmento de codigo (preguntar si estaria bien volver a setear el ip)
    int32_t OP1, OP2, OPC;
    while (increment < codsize) {
        // 1. direc fisica
        printf("[%04X] ", increment);        

        Decode(cpu, &OP1, &OP2, &typeOP1, &typeOP2, &OPC);
        instrucSize = 1 + typeOP1 + typeOP2; //tamaño de la instruccion
        // 2. instruccion en hex
        for (int i = 0; i < instrucSize; i++) {
            printf("%02X ", mem->data[increment + i]);
        }

        // Formato
        for (int i = instrucSize; i < 6; i++) {
            printf("   ");
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
        // Actualizo el incrementador
        increment += 1 + typeOP1 + typeOP2;
    }
}