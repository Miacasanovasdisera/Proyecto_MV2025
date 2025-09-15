# Máquina Virtual MV1 2025

Implementación de una máquina virtual que ejecuta programas en lenguaje ensamblador según las especificaciones de la cátedra de Arquitectura de Computadoras - UNMDP.

# Descripción
Esta máquina virtual emula un procesador de 32 bits que puede ejecutar programas compilados en lenguaje máquina (archivos .vmx). El proyecto implementa una arquitectura segmentada con registros especializados y un conjunto completo de instrucciones aritméticas, lógicas y de control.
Características principales

# Memoria principal: 16 KiB (16,384 bytes)
- Registros: 32 registros de 4 bytes (17 utilizados en esta versión)
- Segmentación: Código y datos en segmentos separados
- Instrucciones: 26 instrucciones implementadas
- Llamadas al sistema: READ y WRITE para E/S

# Ejecucion basica
``` bash
./vmx programa.vmx
```

# Ejecucion con disassembler
``` bash
./vmx programa.vmx -d
```

# Requisitos

- Compilador GCC
- Sistema operativo Unix/Linux o Windows con herramientas de desarrollo
- Herramienta 'make', para mas facilidad a la hora de compilar

# Compilacion
```bash
make
```

# Arquitectura implementada
Registros principales
- LAR, MAR, MBR -> Acceso a memoria
- IP, OPC, OP1, OP2 -> Control de instrucciones
- EAX-EFX -> Propósito general
- AC, CC -> Acumulador y códigos de condición
- CS, DS -> Segmentos de código y datos

# Instrucciones implementadas

Dos operandos

- Aritméticas: MOV, ADD, SUB, MUL, DIV, CMP
- Lógicas: AND, OR, XOR
- Desplazamiento: SHL, SHR, SAR
- Especiales: SWAP, LDL, LDH, RND

Un operando

- Control: JMP, JZ, JP, JN, JNZ, JNP, JNN
- Sistema: SYS
- Lógicas: NOT

Sin operandos

- Control: STOP

Llamadas al sistema

- SYS 1 (READ): Lectura desde teclado
- SYS 2 (WRITE): Escritura a pantalla

# Manejo de errores
La máquina virtual detecta y reporta los siguientes errores:

- Instrucción inválida: Código de operación no reconocido
- División por cero: División o módulo por cero
- Fallo de segmento: Acceso fuera de límites de memoria
- Fallo de carga del programa
- Error de registro: registro invalido
- Tamaño de lectura o escritura inválido


Última actualización: Septiembre 2025

