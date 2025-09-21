#include "errors.h"

/*
Fallo de carga de programa: 1
Instrucción inválida: 2
División por cero: 3
Fallo de segmento: 4
Error de registro: 5
Tamaño de lectura o escritura inválido: 6
*/

int error_Output(int error_code) {
    switch (error_code) {
        case LOAD_PROGRAM_ERROR:
            printf("Error %d: Fallo al cargar el programa.\n", error_code);
            break;
        case INVALID_INSTRUCTION:
            printf("Error %d: Instruccion invalida.\n", error_code);
            break;
        case DIVISION_BY_ZERO:
            printf("Error %d: Division por cero.\n", error_code);
            break;
        case MEMORY_ERROR:
            printf("Error %d: Fallo de segmento.\n", error_code);
            break;
        case REGISTER_ERROR:
            printf("Error %d: Error de registro.\n", error_code);
            break;
        case WRONG_SIZE:
            printf("Error %d: Tamanio de lectura o escritura invalido.\n", error_code);
            break;
        case INVALID_OPERAND:
            printf("Error %d: Operando invalido.\n", error_code);
            break;
        default:
            printf("Error de ejecucion.\n");
            break;
    }
    printf("Se aborta la ejecucion de la maquina virtual.\n");
    exit(error_code);
}