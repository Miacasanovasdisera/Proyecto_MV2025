#include "alu.h"
#include "../Processor/operands.h"
#include "helpers.h"


// funciones aritmeticas

int alu_ADD(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result = dest_value + origin_value;
    update_CC(cpu,result);

    return result;
}

int alu_SUB(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result = dest_value - origin_value;
    update_CC(cpu,result);
}
    

int alu_MUL(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result = dest_value * origin_value;
    update_CC(cpu,result);
}


int alu_DIV(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result;

    if(origin_value != 0){
        result = dest_value / origin_value;
        cpu->AC =  dest_value % origin_value;
        update_CC(cpu,result);
        return result;
    }
    else
        return 0xFFFFFFFF; // Indica error de división por cero
    
}


void alu_CMP(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result = dest_value - origin_value;
    update_CC(cpu,result);
}


int alu_AND(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result = dest_value & origin_value;
    update_CC(cpu,result);
    
    return result;
}



int alu_OR(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{
    int32_t result = dest_value | origin_value;
    update_CC(cpu,result);

    return result;
}



int alu_XOR(cpu_t *cpu, int32_t dest_value, int32_t origin_value)
{   
    int32_t result = dest_value ^ origin_value;
    update_CC(cpu,result);

    return result;
}
