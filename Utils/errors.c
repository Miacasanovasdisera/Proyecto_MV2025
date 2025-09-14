#include "errors.h"

/*
Fallo de carga de programa: 1
Instrucción inválida: 2
División por cero: 3
Fallo de segmento: 4
Error de registro: 5
Tamaño de lectura o escritura inválido: 6
*/

void error_Output(int error_code) {
    switch (error_code) {
        case LOAD_PROGRAM_ERROR:
            printf("Error %d: Fallo al cargar el programa.\n", error_code);
            break;
        case INVALID_INSTRUCTION:
            printf("Error %d: Instrucción inválida.\n", error_code);
            break;
        case DIVISION_BY_ZERO:
            printf("Error %d: División por cero.\n", error_code);
            break;
        case MEMORY_ERROR:
            printf("Error %d: Fallo de segmento.\n", error_code);
            break;
        case REGISTER_ERROR:
            printf("Error %d: Error de registro.\n", error_code);
            break;
        case WRONG_SIZE:
            printf("Error %d: Tamaño de lectura o escritura inválido.\n", error_code);
            break;
        default:
            printf("Error desconocido.\n");
            break;
    }
}