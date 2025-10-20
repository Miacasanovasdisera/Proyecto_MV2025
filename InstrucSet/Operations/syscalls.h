#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "../../Utils/common.h"
#include "../../Processor/processor.h"
#include "../../Memory/mem.h"

// Implementación de las llamadas al sistema READ y WRITE.
int execute_SYS(cpu_t *, mem_t *);
void sys_read(mem_t *,cpu_t *,int32_t ,int32_t ,int16_t );
void sys_write(mem_t ,cpu_t *,int32_t ,int32_t ,int16_t );
void activate_booleans_syscall(int32_t ,int32_t *,int32_t *,int32_t *,int32_t *,int32_t *);
void print_binary(int32_t ,int32_t);
void read_binary(int32_t *);
void print_characters(int32_t ,int32_t);
//la variable segment me dice en que segmento estoy parando, para no superarlo luego ya que leo hasta encontrar un \0
void sys_string_read(mem_t *mem,cpu_t *cpu,int32_t CX,int16_t index,int16_t segment);
void sys_string_write(mem_t mem,int16_t index,int16_t segment);
void sys_clear();
void sys_breakpoint(cpu_t *cpu,mem_t *mem);

#endif