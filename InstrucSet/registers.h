#ifndef REGISTERS_H
#define REGISTERS_H

#define R_LAR 0
#define R_MAR 1
#define R_MBR 2
#define R_IP 3
#define R_OPC 4
#define R_OP1 5
#define R_OP2 6
#define R_EAX 10
#define R_EBX 11
#define R_ECX 12
#define R_EDX 13
#define R_EEX 14
#define R_EFX 15
#define R_AC 16
#define R_CC 17
#define R_CS 26
#define R_DS 27

const char *register_name[] = {
    "LAR", "MAR", "MBR", "IP", "OPC", "OP1", "OP2",
    "?", "?", "?",
    "EAX", "EBX", "ECX", "EDX", "EEX", "EFX",
    "AC", "CC",
    "?","?","?","?","?","?","?","?",
    "CS", "DS"
};

#endif