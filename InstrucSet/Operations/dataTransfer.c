#include "dataTransfer.h"
#include "../helpers.h"

int execute_MOV(cpu_t *cpu, mem_t *mem) {
    int32_t src_value, result;
    uint32_t dest_addr;
    int8_t dest_type = get_operand_type(cpu->OP1);

    get_value(cpu, mem, cpu->OP2, &src_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);
    result = src_value;
    write_dest(cpu, mem, dest_type, dest_addr, result, cpu->OP1);
    return 0;
}

int execute_SWAP(cpu_t *cpu, mem_t *mem) {
    int32_t A_value, B_value, A_addr, B_addr;
    int8_t A_type = get_operand_type(cpu->OP1);
    int8_t B_type = get_operand_type(cpu->OP2);
        
    get_value(cpu, mem, cpu->OP2, &B_value);
    get_value(cpu, mem, cpu->OP1, &A_value);
    get_destination_address(cpu, cpu->OP1, &A_addr);
    get_destination_address(cpu, cpu->OP2, &B_addr);
    write_dest(cpu, mem, A_type, A_addr, B_value, cpu->OP2);
    write_dest(cpu, mem, B_type, B_addr, A_value, cpu->OP1);

    return 0; 
}


int execute_LDL(cpu_t *cpu, mem_t *mem) {
    int32_t src_valueOP1, src_valueOP2, dest_addr, result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_valueOP2);
    get_value(cpu, mem, cpu->OP1, &src_valueOP1);


    get_destination_address(cpu, cpu->OP1, &dest_addr);

    result = (src_valueOP1 & 0xFFFF0000) | (src_valueOP2 & 0x0000FFFF);
    write_dest(cpu, mem, dest_type, dest_addr, result, cpu->OP1);
    return 0; 
    
}

int execute_LDH(cpu_t *cpu, mem_t *mem) {
    int32_t src_valueOP1, src_valueOP2, dest_addr, result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_valueOP2);
    get_value(cpu, mem, cpu->OP1, &src_valueOP1);

    get_destination_address(cpu, cpu->OP1, &dest_addr);

    result = (src_valueOP1 & 0x0000FFFF) | ((src_valueOP2 & 0x0000FFFF) << 16);
    write_dest(cpu, mem, dest_type, dest_addr, result, cpu->OP1);
    return 0;
}

int execute_RND(cpu_t *cpu, mem_t *mem) {
    int32_t src_valueOP2, dest_addr, src_valueOP1;
    int8_t dest_type = get_operand_type(cpu->OP1);
    
    srand(time(NULL));
    get_value(cpu, mem, cpu->OP1, &src_valueOP2);
    get_destination_address(cpu, cpu->OP1, &dest_addr);
    
    src_valueOP1 = rand() % (src_valueOP2 + 1);
    
    write_dest(cpu, mem, dest_type, dest_addr, src_valueOP1, cpu->OP1);
    return 0;
}