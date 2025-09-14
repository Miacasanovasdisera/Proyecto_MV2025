#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "../../Utils/common.h"
#include "../../Processor/processor.h"
#include "../../Memory/mem.h"

// Implementación de las llamadas al sistema READ y WRITE.
int execute_SYS(cpu_t *, mem_t *);

#endif