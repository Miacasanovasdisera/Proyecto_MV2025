#include "processor.h"

void cpu_init(cpu_t *cpu){
    
    // Setea todo en 0
    memset(cpu,0,sizeof(cpu_t));
    
}
void cpu_update_IP(cpu_t *cpu){
    cpu->IP =  cpu->OP1|cpu->OP2|0x01;
}