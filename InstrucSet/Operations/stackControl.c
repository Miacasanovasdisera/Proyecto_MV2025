#include "stackControl.h"
#include "../../Stack/stack.h"
#include "../helpers.h"

int execute_PUSH(cpu_t *cpu, mem_t *mem) {
    int32_t value;
    get_value(cpu, mem, cpu->OP1, &value);
    
    stack_push(cpu, mem, value);
    return 0;
}

int execute_POP(cpu_t *cpu, mem_t *mem) {
    int32_t value = stack_pop(cpu, mem);

    int8_t type_dest = get_operand_type(cpu->OP1);
    uint32_t dest_addrss;
    get_destination_address(cpu, cpu->OP1, &dest_addrss);
    write_dest(cpu, mem, type_dest, dest_addrss, value, cpu->OP1);
    return 0;
}

int execute_CALL(cpu_t *cpu, mem_t *mem) {
    // Guardar la dirección de retorno en la pila
    printf("CALL Instruction Operand: %08X\n", cpu->OP1); // DEBUG
    stack_push(cpu, mem, cpu->IP);

    // Saltar a la dirección de la subrutina
    int32_t target_address;
    get_value(cpu, mem, cpu->OP1, &target_address);
    target_address |= cpu->CS;
    
    printf("CALL to address: %08X\n", target_address); // DEBUG
    cpu->IP = cpu_logic_to_physic(*mem,target_address,4);

    return 0;
}

int execute_RET(cpu_t *cpu, mem_t *mem) {
    // Recuperar la dirección de retorno de la pila
    int32_t return_address = stack_pop(cpu, mem);
    cpu->IP = return_address;
    return 0;
}