# Nombre del ejecutable final
TARGET = vmx

# Directorios
SRC_DIR = .
IMG_DIR = Images
IS_DIR = InstrucSet
MEM_DIR = Memory
OPERATIONS_DIR = InstrucSet/Operations
PROC_DIR = Processor
UTILS_DIR = Utils
STACK_DIR = Stack

# Archivos fuente
SRCS = $(SRC_DIR)/main.c \
       $(UTILS_DIR)/errors.c \
       $(PROC_DIR)/processor.c \
       $(MEM_DIR)/mem.c \
       $(IS_DIR)/instruct.c \
       $(OPERATIONS_DIR)/alu.c \
       $(OPERATIONS_DIR)/dataTransfer.c \
       $(IS_DIR)/helpers.c \
       $(OPERATIONS_DIR)/fluxControl.c \
       $(OPERATIONS_DIR)/ccModifiers.c \
       $(OPERATIONS_DIR)/shiftModule.c \
       $(OPERATIONS_DIR)/syscalls.c \
       $(OPERATIONS_DIR)/stackControl.c \
       $(UTILS_DIR)/disassembler.c \
       $(UTILS_DIR)/config.c \
       $(STACK_DIR)/stack.c \
       $(IMG_DIR)/vmi.c

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