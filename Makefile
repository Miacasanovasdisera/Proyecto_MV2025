# ============================================================================
# MAKEFILE - MÁQUINA VIRTUAL VMX
# ============================================================================
# Descripción: Sistema de compilación robusto con detección automática de
#              dependencias, soporte multiplataforma y limpieza inteligente.
# ============================================================================

# ----------------------------------------------------------------------------
# CONFIGURACIÓN BÁSICA
# ----------------------------------------------------------------------------

# Nombre del ejecutable
TARGET = vmx

# Compilador
CC = gcc

# Flags base de compilación
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -MMD -MP

# Flags de optimización (cambiar según necesidad)
ifdef DEBUG
    CFLAGS += -g -O0 -DDEBUG
    $(info *** Modo DEBUG activado ***)
else
    CFLAGS += -O2
endif

# Flags de linker
LDFLAGS =

# ----------------------------------------------------------------------------
# DIRECTORIOS
# ----------------------------------------------------------------------------

SRC_DIRS = . \
           Images \
           InstrucSet \
           InstrucSet/Operations \
           Memory \
           Processor \
           Stack \
           Utils

# Directorios de includes (para encontrar headers)
INC_DIRS = $(SRC_DIRS)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

CFLAGS += $(INC_FLAGS)

# ----------------------------------------------------------------------------
# DETECCIÓN AUTOMÁTICA DE ARCHIVOS
# ----------------------------------------------------------------------------

# Buscar todos los archivos .c en los directorios especificados
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Generar lista de archivos objeto (.o)
OBJS = $(SRCS:.c=.o)

# Generar lista de archivos de dependencias (.d)
DEPS = $(SRCS:.c=.d)

# ----------------------------------------------------------------------------
# DETECCIÓN DE SISTEMA OPERATIVO
# ----------------------------------------------------------------------------

ifeq ($(OS),Windows_NT)
    DETECTED_OS = Windows
    RM = cmd /C del /F /Q
    RM_DIR = cmd /C rmdir /S /Q
    MKDIR = cmd /C mkdir
    EXE_EXT = .exe
    TARGET_FULL = $(TARGET)$(EXE_EXT)
    # Reemplazar / por \ en paths de Windows
    FIX_PATH = $(subst /,\,$(1))
else
    DETECTED_OS = $(shell uname -s)
    RM = rm -f
    RM_DIR = rm -rf
    MKDIR = mkdir -p
    EXE_EXT =
    TARGET_FULL = $(TARGET)
    FIX_PATH = $(1)
endif

# ----------------------------------------------------------------------------
# COLORES PARA OUTPUT (solo en sistemas Unix)
# ----------------------------------------------------------------------------

ifneq ($(DETECTED_OS),Windows)
    COLOR_RESET = \033[0m
    COLOR_GREEN = \033[32m
    COLOR_YELLOW = \033[33m
    COLOR_BLUE = \033[34m
    COLOR_RED = \033[31m
else
    COLOR_RESET =
    COLOR_GREEN =
    COLOR_YELLOW =
    COLOR_BLUE =
    COLOR_RED =
endif

# ----------------------------------------------------------------------------
# REGLAS PRINCIPALES
# ----------------------------------------------------------------------------

.PHONY: all clean rebuild run info help test

# Regla por defecto
all: banner $(TARGET_FULL)
	@echo $(COLOR_GREEN)Compilacion exitosa: $(TARGET_FULL)$(COLOR_RESET)

# Banner informativo
banner:
	@echo $(COLOR_BLUE)========================================$(COLOR_RESET)
	@echo $(COLOR_BLUE)  Compilando Maquina Virtual VMX$(COLOR_RESET)
	@echo $(COLOR_BLUE)========================================$(COLOR_RESET)
	@echo OS detectado: $(DETECTED_OS)
	@echo Archivos .c: $(words $(SRCS))
	@echo Modo: $(if $(DEBUG),DEBUG,RELEASE)

# Compilar el ejecutable
$(TARGET_FULL): $(OBJS)
	@echo "$(COLOR_YELLOW)→ Linkeando $(TARGET_FULL)...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Compilar archivos .c a .o (con generación automática de dependencias)
%.o: %.c
	@echo "$(COLOR_YELLOW)→ Compilando $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Incluir archivos de dependencias (si existen)
-include $(DEPS)

# ----------------------------------------------------------------------------
# LIMPIEZA
# ----------------------------------------------------------------------------

# Limpieza básica (solo .o y .d)
clean:
	@echo "$(COLOR_YELLOW)→ Limpiando archivos temporales...$(COLOR_RESET)"
ifeq ($(DETECTED_OS),Windows)
	@for %%d in ($(SRC_DIRS)) do @if exist %%d\*.o del /F /Q %%d\*.o 2>nul
	@for %%d in ($(SRC_DIRS)) do @if exist %%d\*.d del /F /Q %%d\*.d 2>nul
	@if exist $(TARGET_FULL) del /F /Q $(TARGET_FULL) 2>nul
else
	@$(RM) $(OBJS) $(DEPS) $(TARGET_FULL)
endif
	@echo "$(COLOR_GREEN)✓ Limpieza completada$(COLOR_RESET)"

# Limpieza profunda (incluye ejecutables y backups)
distclean: clean
	@echo "$(COLOR_YELLOW)→ Limpieza profunda...$(COLOR_RESET)"
ifeq ($(DETECTED_OS),Windows)
	@if exist *.exe del /F /Q *.exe 2>nul
	@if exist *~ del /F /Q *~ 2>nul
else
	@$(RM) $(TARGET) $(TARGET_FULL) *~ *.exe
endif
	@echo "$(COLOR_GREEN)✓ Limpieza profunda completada$(COLOR_RESET)"

# ----------------------------------------------------------------------------
# UTILIDADES
# ----------------------------------------------------------------------------

# Recompilar todo desde cero
rebuild: clean all

# Ejecutar el programa (con ejemplo de parámetros)
run: all
	@echo "$(COLOR_BLUE)→ Ejecutando $(TARGET_FULL)...$(COLOR_RESET)"
	@./$(TARGET_FULL)

# Mostrar información de configuración
info:
	@echo $(COLOR_BLUE)========================================$(COLOR_RESET)
	@echo $(COLOR_BLUE)  Información de Compilación$(COLOR_RESET)
	@echo $(COLOR_BLUE)========================================$(COLOR_RESET)
	@echo Sistema Operativo: $(DETECTED_OS)
	@echo Compilador: $(CC)
	@echo Flags: $(CFLAGS)
	@echo Target: $(TARGET_FULL)
	@echo ""
	@echo Directorios fuente:
	@$(foreach dir,$(SRC_DIRS),echo "  - $(dir)";)
	@echo ""
	@echo Archivos fuente detectados: $(words $(SRCS))
	@$(foreach src,$(SRCS),echo "  - $(src)";)
	@echo ""
	@echo Archivos objeto: $(words $(OBJS))
	@echo Archivos dependencias: $(words $(DEPS))

# Test de compilación (compila sin optimización para detectar warnings)
test:
	@echo $(COLOR_YELLOW)→ Compilando en modo test (todos los warnings)...$(COLOR_RESET)
	@$(MAKE) clean
	@$(CC) -Wall -Wextra -Wpedantic -Werror -std=c11 $(INC_FLAGS) $(SRCS) -o $(TARGET)_test
	@echo $(COLOR_GREEN)✓ Test de compilación pasado$(COLOR_RESET)
	@$(RM) $(TARGET)_test

# Verificar integridad del Makefile
check:
	@echo $(COLOR_YELLOW)→ Verificando configuración...$(COLOR_RESET)
	@echo Verificando que existen todos los directorios...
	@$(foreach dir,$(SRC_DIRS),[ -d "$(dir)" ] && echo "  ✓ $(dir)" || echo "  ✗ $(dir) NO EXISTE";)
	@echo ""
	@echo Verificando que existen archivos fuente...
	@if [ -z "$(SRCS)" ]; then \
		echo $(COLOR_RED)✗ No se encontraron archivos .c$(COLOR_RESET); \
		exit 1; \
	else \
		echo $(COLOR_GREEN)✓ $(words $(SRCS)) archivos .c encontrados$(COLOR_RESET); \
	fi

# Ayuda
help:
	@echo $(COLOR_BLUE)========================================$(COLOR_RESET)
	@echo $(COLOR_BLUE)  Makefile - Máquina Virtual VMX$(COLOR_RESET)"
	@echo $(COLOR_BLUE)========================================$(COLOR_RESET)
	@echo ""
	@echo "Uso: make [target]"
	@echo ""
	@echo "Targets disponibles:"
	@echo "  $(COLOR_GREEN)all$(COLOR_RESET)       - Compilar el proyecto (default)"
	@echo "  $(COLOR_GREEN)clean$(COLOR_RESET)     - Limpiar archivos temporales (.o, .d)"
	@echo "  $(COLOR_GREEN)distclean$(COLOR_RESET) - Limpieza profunda (incluye ejecutables)"
	@echo "  $(COLOR_GREEN)rebuild$(COLOR_RESET)   - Limpiar y recompilar todo"
	@echo "  $(COLOR_GREEN)run$(COLOR_RESET)       - Compilar y ejecutar"
	@echo "  $(COLOR_GREEN)info$(COLOR_RESET)      - Mostrar información de compilación"
	@echo "  $(COLOR_GREEN)test$(COLOR_RESET)      - Compilar en modo test (máximos warnings)"
	@echo "  $(COLOR_GREEN)check$(COLOR_RESET)     - Verificar integridad del proyecto"
	@echo "  $(COLOR_GREEN)help$(COLOR_RESET)      - Mostrar esta ayuda"
	@echo ""
	@echo "Ejemplos:"
	@echo "  make              # Compilar normalmente"
	@echo "  make DEBUG=1      # Compilar en modo debug"
	@echo "  make clean        # Limpiar temporales"
	@echo "  make rebuild      # Recompilar todo"
	@echo "  make run          # Compilar y ejecutar"
	@echo ""

# ----------------------------------------------------------------------------
# CONFIGURACIÓN ESPECIAL
# ----------------------------------------------------------------------------

# No borrar archivos intermedios
.PRECIOUS: %.o

# Usar el shell correcto en Windows
ifeq ($(DETECTED_OS),Windows)
    SHELL = cmd
endif