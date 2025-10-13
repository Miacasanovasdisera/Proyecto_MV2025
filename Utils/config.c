#include "config.h"

void config_init(vm_config_t *config) {
    memset(config, 0, sizeof(vm_config_t));
    
    // Valores por defecto
    config->vmx_file = NULL;
    config->vmi_file = NULL;
    config->memory_size_kib = 16;
    config->disassem_mode = 0;
    config->param_count = 0;
}

void config_parse(vm_config_t *config, int argc, char *argv[]) {
    // argv[0] es el nombre del programa (vmx)
    
    int i = 1, reading_params = 0;
    
    while (i < argc) {
        char *arg = argv[i];
        
        // Si ya estamos leyendo parámetros (-p), todo lo demás son parámetros
        if (reading_params) {
            if (config->param_count >= MAX_PARAMS) {
                    error_Output(ARGUMENT_ERROR);
                    return;
            }
            
            config->params[config->param_count++] = arg;
            i++;
            continue;
        }
        
        // Detectar flags
        if (arg[0] == '-') {
            // Flag -d
            if (strcmp(arg, "-d") == 0) {
                config->disassem_mode = 1;
                i++;
            }
            // Flag -p
            else if (strcmp(arg, "-p") == 0) {
                reading_params = 1;
                i++;
            }
            
            else {
                error_Output(ARGUMENT_ERROR);
                return;
            }
            
        }
        // Opción m=M
        else if (strncmp(arg, "m=", 2) == 0 || strncmp(arg, "M=", 2) == 0) {
            char *size_str = arg + 2;
            int size = atoi(size_str);
            if (size <= 0 || size > 65535) {
                error_Output(ARGUMENT_ERROR);
                return;
            }

            config->memory_size_kib = size;
            i++;
        }
        // Archivos (terminan en .vmx o .vmi)
        else {
            size_t len = strlen(arg);
            
            // Detectar archivo .vmx
            if (len > 4 && strcmp(arg + len - 4, ".vmx") == 0) {
                if (config->vmx_file != NULL) {
                    error_Output(ARGUMENT_ERROR);
                    return;
                }

                config->vmx_file = arg;
                i++;
            }
            // Detectar archivo .vmi
            else if (len > 4 && strcmp(arg + len - 4, ".vmi") == 0) {
                if (config->vmi_file != NULL) {
                    error_Output(ARGUMENT_ERROR);
                    return;
                }

                config->vmi_file = arg;
                i++;
            }
            else {
                error_Output(ARGUMENT_ERROR);
                return;
            }
            
        }
    }
}

void config_validate(vm_config_t *config) {
    // Al menos debe haber un archivo .vmx o .vmi
    if (config->vmx_file == NULL && config->vmi_file == NULL) {
        error_Output(ARGUMENT_ERROR);
        return;
    }
    
    // Si no hay .vmx, ignorar parámetros -p
    if (config->vmx_file == NULL && config->param_count > 0) {
        printf("Advertencia: Se ignorarán los parámetros -p (no hay archivo .vmx)\n");
        config->param_count = 0;
    }
}

void config_free(vm_config_t *config) {
    // Para limpiar la estructura
    memset(config, 0, sizeof(vm_config_t));
}