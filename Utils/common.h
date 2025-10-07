// .h para librerias en general

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>

#define INVALID_OPERAND 9
#define WRONG_SIZE 6
#define REGISTER_ERROR 5
#define MEMORY_ERROR 4
#define DIVISION_BY_ZERO 3
#define INVALID_INSTRUCTION 2
#define LOAD_PROGRAM_ERROR 1
#define ARGUMENT_ERROR 8

// variable global para el nombre del archivo .vmi
extern const char *g_vmi_filename;