#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"
#include "errors.h"

#define MAX_PARAMS 100

typedef struct {
    // Archivos
    char *vmx_file;         
    char *vmi_file;          
    
    // Opciones
    uint32_t memory_size_kib;
    int disassem_mode;          
    
    // Parámetros del programa
    char *params[MAX_PARAMS];
    int param_count;          
    
} vm_config_t;

// Inicializa la configuración con valores por defecto
void config_init(vm_config_t *config);

// Parsea los argumentos de línea de comandos
void config_parse(vm_config_t *config, int argc, char *argv[]);

// Libera recursos de la configuración
void config_free(vm_config_t *config);

// Valida la configuración
void config_validate(vm_config_t *config);

#endif