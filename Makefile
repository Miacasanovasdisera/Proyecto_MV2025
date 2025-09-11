# Nombre del ejecutable final
TARGET = vmx

# Directorios
SRC_DIR = .
PROC_DIR = Processor
MEM_DIR = Memory
IS_DIR = InstrucSet
UTILS_DIR = Utils

# Archivos fuente
SRCS = $(SRC_DIR)/main.c \
       $(PROC_DIR)/processor.c \
       $(MEM_DIR)/mem.c \
       $(IS_DIR)/instruct.c \
       $(IS_DIR)/alu.c \
       $(IS_DIR)/dataTransfer.c \
       $(IS_DIR)/fluxControl.c \
       $(IS_DIR)/syscalls.c \
       $(UTILS_DIR)/disassembler.c \
       $(UTILS_DIR)/debug.c

# Archivos objeto (reemplaza .c por .o)
OBJS = $(SRCS:.c=.o)

# Flags de compilación (puedes agregar -g para debuggear)
CFLAGS = -Wall -Wextra -std=c99 -g -IInstrucSet -IProcessor -IMemory -IUtils

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilar archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos temporales
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean