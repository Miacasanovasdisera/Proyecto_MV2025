# Máquina Virtual MVX 2025 — Versión 2

Implementación de una máquina virtual de 32 bits que ejecuta programas en lenguaje ensamblador según las especificaciones de la cátedra de Arquitectura de Computadoras (UNMDP).

Esta segunda versión incorpora soporte de segmentos múltiples (incluido Param Segment y Const Segment), direccionamiento lógico estable, subregistros (AL/AH/AX), tamaños de acceso a memoria por operando (b/w/l), pila y llamadas/retornos (CALL/RET), y herramientas de traducción de .asm a .vmx para Windows y Linux.

---

## Requisitos

- Compilador GCC
- make
- Windows o Linux. En Linux, para usar el traductor de .asm a .vmx provisto (versión .exe), se necesita Wine.

---

## Compilación

```bash
make              # Compilación estándar (Release)
make DEBUG=1      # Compilación con símbolos de depuración
make clean        # Limpia objetos y ejecutables
```

---

## Ejecución

La VM soporta tres modos:

- Modo A (solo .vmx): ejecución normal.
- Modo B (.vmx + .vmi): modo debug con breakpoints; `SYS 15` guarda un snapshot VMI.
- Modo C (solo .vmi): reanuda ejecución desde una imagen guardada.

Ejemplos:

```bash
# Linux
./vmx programa.vmx
./vmx programa.vmx -d                # con disassembler
./vmx programa.vmx m=64              # memoria de 64 KiB
./vmx programa.vmx -p arg1 arg2      # con parámetros para Param Segment
./vmx programa.vmx debug.vmi         # modo debug: SYS 15 guarda en debug.vmi
./vmx debug.vmi                      # reanudar desde imagen

# Windows (PowerShell/CMD)
vmx.exe programa.vmx
vmx.exe programa.vmx -d
vmx.exe programa.vmx m=64
vmx.exe programa.vmx -p arg1 arg2
vmx.exe programa.vmx debug.vmi
vmx.exe debug.vmi
```

Parámetros de línea de comandos:

- `-d`: imprime el desensamblado antes de ejecutar.
- `-p`: a partir de aquí, cada token se trata como parámetro del programa (se cargarán en el Param Segment).
- `m=NNNN`: tamaño de memoria en KiB (por defecto 16 Kib).
- Si se pasa un `.vmi` junto a un `.vmx`, `SYS 15` (breakpoint) guardará el estado en ese archivo. Si se pasa únicamente un `.vmi`, se reanuda desde esa imagen.

---

## Traducción de .asm a .vmx

Se incluyen dos utilidades en la raíz del repo que ejecutan el traductor ubicado en la carpeta `Test/`:

- Windows:
  ```bash
  .\translate nombre_base
  # Traduce Test\nombre_base.asm → Test\nombre_base.vmx
  ```
- Linux (requiere Wine):
  ```bash
  bash translate.sh nombre_base
  # Traduce Test/nombre_base.asm → Test/nombre_base.vmx usando wine vmtV2.exe
  ```

Notas:
- Ambos scripts cambian al directorio `./Test` de forma relativa y ejecutan el traductor (`vmtV2.exe`).
- Asegurate de ubicar tus `.asm` dentro de `Test/`.

---

## Arquitectura y Segmentación

- Segmentos:
  - `PS` (Param): guarda los parametros de la subrutina principal (punteros lógicos a los strings).
  - `KS` (Const): reservado para el uso de constantes (definidos en ensamblador).
  - `CS` (Code): contiene el codigo fuente.
  - `DS` (Data): se utiliza para los datos del proceso. 
  - `ES` (Extra): reservado para el uso de memoria dinamica.
  - `SS` (Stack): dedicado exclusivamente para la pila del proceso.
- La VM verifica límites al traducir lógico→físico para cada acceso.

Inicialización típica:
- `IP = CS | entry_point`
- `SP = SS + size` (pila vacía; crece hacia abajo)
- Si hay parámetros: la VM crea `PS` y coloca en la pila:
  - Dirección de retorno ficticia (0xFFFFFFFF)
  - `argc`
  - `argv` (puntero lógico al array dentro de PS)

---

## Registros

- Acceso a memoria: `LAR`, `MAR`, `MBR`
- Control de instrucciones: `IP`, `OPC`, `OP1`, `OP2`
- Generales: `EAX`, `EBX`, `ECX`, `EDX`, `EEX`, `EFX`
  - Subregistros mediante modificador de sector en el byte del registro:
    - `00`: entero (EAX)
    - `01`: AL
    - `10`: AH
    - `11`: AX
- Especiales: `AC` (acumulador de división), `CC` (condición: Z=0x40000000, N=0x80000000)
- Segmentos: `CS`, `DS`, `ES`, `SS`, `KS`, `PS`
- Punteros/otros: `SP`, `BP`

---

## Instrucciones implementadas

Dos operandos
- Aritméticas: `MOV`, `ADD`, `SUB`, `MUL`, `DIV`, `CMP`
- Lógicas: `AND`, `OR`, `XOR`
- Desplazamiento: `SHL`, `SHR`, `SAR`
- Especiales: `SWAP`, `LDL`, `LDH`, `RND`

Un operando
- Flujo: `JMP`, `JZ`, `JP`, `JN`, `JNZ`, `JNP`, `JNN`, `JN`
- Sistema: `SYS`
- Lógicas: `NOT`

Pila / Llamadas
- `PUSH`, `POP`, `CALL`, `RET`, `STOP`

Modificadores de operando
- Registro (GPR): sector en bits 7–6 del código de registro:
  - `00`=entero, `01`=AL, `10`=AH, `11`=AX
- Memoria: tamaño embebido:
  - `l[...]`=4 bytes, `w[...]`=2 bytes, `b[...]`=1 byte

---

## Llamadas al sistema (SYS)

- `SYS 1` (READ): lee desde teclado
  - `EAX`: flags de formato (bitfield)
    - `0x10` binario, `0x08` hexadecimal, `0x04` octal, `0x02` caracteres, `0x01` decimal
  - `ECX`: `ECXH` (alto 16 bits)=tamaño por elemento (1/2/4), `ECXL` (bajo 16 bits)=cantidad
  - `EDX`: dirección lógica de destino (inicio)
- `SYS 2` (WRITE): escribe a pantalla
  - Misma interpretación de `EAX`, `ECX`, `EDX` que READ; lee de memoria y muestra.
- `SYS 3` (STRING_READ): lee string a memoria (segmento tomado de `EDX>>16`)
- `SYS 4` (STRING_WRITE): imprime string hasta `'\0'` (segmento tomado de `EDX>>16`)
- `SYS 7` (CLEAR): limpiar pantalla (Windows)
- `SYS 15` (BREAKPOINT): guarda snapshot `.vmi` si se ejecuta en modo debug (.vmx + .vmi)

Notas:
- Los accesos se realizan en big-endian dentro de la VM (consistentemente al leer/escribir elementos de 1/2/4 bytes).

---

## Desensamblador y Snapshots

- `-d`: imprime el desensamblado del `.vmx` (códigos y operandos).
- Snapshots `.vmi`: restaura o guarda registros, tabla de segmentos y memoria completa.

---

## Errores reportados por la VM

- Operando invalido
- Sub-desbordamiento de pila
- Desbordamiento de pila
- Memoria insuficiente
- Semento no encontrado
- Instrucción inválida
- División por cero
- Tamaño de lectura/escritura inválido
- Error de segmento (acceso fuera de límites)
- Error de carga del programa
- Error de registro (código de registro inválido)
- Error de argumentos (entrada de datos desde terminal)
- Error de imagen

---

## Ejemplos rápidos

```bash
# Traducir y ejecutar (Windows)
.\translate ej3btp5
vmx.exe Test\ej3btp5.vmx

# Traducir y ejecutar (Linux, con Wine)
bash translate.sh ej3btp5
./vmx Test/ej3btp5.vmx -d

# Ejecutar con parámetros
./vmx Test/ejfactorialV2.vmx -p 5

# Modo debug (breakpoints con SYS 15)
./vmx Test/miProg.vmx debug.vmi
```

---

# Integrantes

- Octavio Laz
- Mia Casanovas Di Sera
- Agustin Zalazar

Última actualización: Octubre 2025