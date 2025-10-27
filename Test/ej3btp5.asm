toString:       PUSH BP
                MOV BP, SP
                PUSH EAX
                PUSH ECX
                PUSH EDX
                ; EBX no se preserva a propósito: lo usamos como cursor que avanza

                MOV EAX, [BP+8]      ; n
                MOV EBX, [BP+12]     ; ptr

                ; ¿n < 0?
                CMP EAX, 0
                JN  negativo

                ; ¿n < 10?  (EAX - 10 < 0)
                CMP EAX, 10
                JN  base

recur:          MOV ECX, EAX
                DIV ECX, 10          ; ECX = n/10, AC = n%10

                ; Guardar resto antes del llamado recursivo (porque AC se pisa)
                PUSH AC

                ; Llamada recursiva con (n/10, ptr)
                PUSH EBX
                PUSH ECX
                CALL toString
                ADD SP, 8

                ; Recuperar el resto guardado
                POP EDX              ; EDX = n%10

emit:           SUB EDX, 30          ; '0'
                MOV b[EBX], EDX
                MOV b[EBX+1], 0      ; mantener terminación
                ADD EBX, 1           ; avanzar cursor
                JMP fin

base:           MOV EDX, EAX
                ADD EDX, 30
                MOV b[EBX], EDX
                MOV b[EBX+1], 0
                ADD EBX, 1
                JMP fin

negativo:       MOV b[EBX], 45       ; '-'
                ADD EBX, 1
                NOT EAX
                ADD EAX, 1           ; EAX = -EAX
                ; ahora tratar igual que arriba
                CMP EAX, 10
                JN  base
                JMP recur

fin:            POP EDX
                POP ECX
                POP EAX
                MOV SP, BP
                POP BP
                RET

 main:          MOV EAX, 1           ; formato decimal para read
                MOV EDX, DS          ; leer un entero en DS:0
                LDL ECX, 1
                LDH ECX, 4
                SYS 3

                MOV EBX, DS
                ADD EBX, 100         ; buffer destino de string

                ; Llamada: toString(n, ptr)
                PUSH EBX             ; ptr
                PUSH [EDX]           ; n (en DS:0)
                CALL toString
                ADD SP, 8

                ; Imprimir desde el INICIO del buffer (no usar EBX tras la llamada)
                MOV EDX, DS
                ADD EDX, 100
                SYS 2
                RET