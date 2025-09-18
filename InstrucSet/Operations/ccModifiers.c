#include "ccModifiers.h"
#include "../helpers.h"
#include "alu.h"
#include "shiftModule.h"
#include "../../Utils/errors.h"
#include "operands.h"

int execute_ADD(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr, result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);

    result = alu_ADD(cpu, dest_value, src_value);
    
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_SUB(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr,result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);

    result = alu_SUB(cpu, dest_value, src_value);
    
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_MUL(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr,result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);

    result = alu_MUL(cpu, dest_value, src_value);
    
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_DIV(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr, result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);

    if (src_value == 0)
        return error_Output(DIVISION_BY_ZERO);
    
    alu_DIV(cpu, dest_value, src_value, &result);
    
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_CMP(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr;
    int8_t dest_type = get_operand_type(cpu->OP1);
    
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);
    
    alu_CMP(cpu, dest_value, src_value);
    return 0;
}

int execute_OR(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr, result;
    int8_t dest_type = get_operand_type(cpu->OP1);
    
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);
    
    result = alu_OR(cpu, dest_value, src_value);
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_XOR(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr, result;
    int8_t dest_type = get_operand_type(cpu->OP1);
    
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);
    
    result = alu_XOR(cpu, dest_value, src_value);
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_AND(cpu_t *cpu, mem_t *mem) {
    int32_t dest_value, src_value, dest_addr,result;
    int8_t dest_type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP2, &src_value);
    get_value(cpu, mem, cpu->OP1, &dest_value);
    get_destination_address(cpu, cpu->OP1, &dest_addr);

    result = alu_AND(cpu, dest_value, src_value);
    
    write_dest(cpu, mem, dest_type, dest_addr, result);
    return 0;
}

int execute_NOT(cpu_t *cpu, mem_t *mem) {
     int32_t value, addr,result;
    int8_t type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP1, &value);
    get_destination_address(cpu, cpu->OP1, &addr);

    result = alu_NOT(cpu, value);
    
    write_dest(cpu, mem, type, addr, result);
    return 0;
}

int execute_SHL(cpu_t *cpu, mem_t *mem) {
    int32_t value, addr,result;
    int8_t type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP1, &value);
    get_destination_address(cpu, cpu->OP1, &addr);

    result = shift_SHL(cpu, value);
    
    write_dest(cpu, mem, type, addr, result);
    return 0;
}

int execute_SHR(cpu_t *cpu, mem_t *mem) {
    int32_t value, addr,result;
    int8_t type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP1, &value);
    get_destination_address(cpu, cpu->OP1, &addr);

    result = shift_SHR(cpu, value);
    
    write_dest(cpu, mem, type, addr, result);
    return 0;
}

int execute_SAR(cpu_t *cpu, mem_t *mem) {
    int32_t value, addr,result;
    int8_t type = get_operand_type(cpu->OP1);
        
    get_value(cpu, mem, cpu->OP1, &value);
    get_destination_address(cpu, cpu->OP1, &addr);

    result = shift_SAR(cpu, value);
    
    write_dest(cpu, mem, type, addr, result);
    return 0;
    
}