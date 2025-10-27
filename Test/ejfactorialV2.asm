\\EXTRA 3000

FACT:   PUSH BP
        MOV BP,SP
        SUB SP,4
        PUSH EDX
        PUSH ECX
        MOV [BP-4],1
        MOV EDX, [BP+8]
        CMP EDX, 1
        JNP FIN
        SUB EDX, 1
        PUSH EDX
        CALL FACT
        ADD SP,4
        MOV ECX, EAX
        MUL ECX, [BP+8]
        MOV [BP-4], ECX
FIN:    MOV EAX, [BP-4]
        POP ECX
        POP EDX
        ADD SP,4
        MOV SP,BP
        POP BP
        RET

MAIN:   PUSH BP
        MOV BP, SP
        PUSH EAX
        PUSH ECX
        PUSH EDX
        MOV EAX, [BP+12]
        MOV EAX, [EAX]
        MOV EAX, b[EAX]
        SUB EAX, '0'
        PUSH EAX
        CALL FACT
        ADD SP, 4
        
        ; ===== NUEVO: guardar el resultado en memoria antes de imprimir =====
        ; Usamos DS:0 como buffer de salida
        MOV [DS+0], EAX      ; guardar factorial (4 bytes) en el inicio del DS
        MOV EDX, DS          ; EDX = puntero lógico al DS:0

        ; Formato de impresión: 4 bytes, 1 elemento, decimal
        LDH ECX, 4
        LDL ECX, 1
        MOV EAX, 1           ; decimal
        SYS 15
        SYS 2                ; imprime el entero almacenado en [EDX]

        POP EDX
        POP ECX
        POP EAX
        MOV SP, BP
        POP BP
        RET