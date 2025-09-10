#include "disassembler.h"
#include "../Processor/processor.h"

void decoder(int8_t byte1, int8_t byte2, cpu_t *cpu) {
    int8_t alto = byte1 >> 2;   // primeros 2 bits
    int8_t bajo  = byte1 & 0x03; // últimos 2 bits

    cpu->OP2 = alto;
    cpu->OP1 = bajo;
    if (cpu->OP2 == 0x00){ //si solo hay un tipo de registro en la instruccion
                            // utilizamos el operando 1
        cpu->OP1=cpu->OP2;
        cpu->OP2=0x00;
    }
    cpu->OPC = ((byte1 & 0x01) << 4) | (byte2 & 0x0F); // ultimo bit + nibble de b2 (en total tendria 5 bits)
}

//void disassembler(int8_t *code, int size) { //mas que nada para verificar, no sirve de mucho
//    int i = 0;
//    while (i < size) {
//        int8_t byte1 = code[i++];
//        int8_t byte2 = code[i++];
//
//        Decoded d = decoder(byte1, byte2);

//        printf("Opcode: %d, Op1Type: %d, Op2Type: %d\n", d.OPC, d.Top1, d.Top2);

        // Segun el cod de operacion, leer mas bytes
        // y mapear registros o inmediatos
//    }
//}