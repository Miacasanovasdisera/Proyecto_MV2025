# Nombre del ejecutable final
TARGET = vmx

# Directorios
SRC_DIR = .
PROC_DIR = Processor
MEM_DIR = mem
IS_DIR = InstrucSet
UTILS_DIR = utils

# Archivos fuente
SRCS = $(SRC_DIR)/main.c \
       $(PROC_DIR)/Processor.c \
       $(MEM_DIR)/mem.c \
       $(IS_DIR)/instruc.c \
       $(IS_DIR)/alu.c \
       $(IS_DIR)/syscalls.c \
       $(UTILS_DIR)/disassembler.c \
       $(UTILS_DIR)/debug.c

# Archivos objeto (reemplaza .c por .o)
OBJS = $(SRCS:.c=.o)

# Flags de compilación (puedes agregar -g para debuggear)
CFLAGS = -Wall -Wextra -std=c99 -g

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos temporales
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean