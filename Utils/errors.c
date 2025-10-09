#include "errors.h"

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
        case ARGUMENT_ERROR:
            printf("Error %d: Numero de argumentos invalido.\n", error_code);
            break;
        case IMAGE_ERROR:
            printf("Error %d: Problema de carga de imagen.\n", error_code);
            break;
        case STACK_OVERFLOW:
            printf("Error %d: Desbordamiento de pila.\n", error_code);
            break;
        case STACK_UNDERFLOW:
            printf("Error %d: Subdesbordamiento de pila.\n", error_code);
            break;
        case INSUFFICIENT_MEMORY:
            printf("Error %d: Memoria insuficiente.\n", error_code);
        break;
        default:
            printf("Error de ejecucion.\n");
            break;
    }
    printf("Se aborta la ejecucion de la maquina virtual.\n");
    exit(error_code);
}