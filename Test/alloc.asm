\\EXTRA 1024
SIZE_ES EQU 1024
NULL EQU -1
ALLOC:      PUSH BP 
            MOV BP,SP
            MOV EAX, NULL
            MOV EBX, W[ES+2]
            ADD EBX, [BP+8]
            CMP EBX, SIZE_ES
            JP ALLOC_END
            MOV EAX, [ES]
ALLOC_END:  MOV EDX,EAX
            MOV EAX,8
            LDH ECX,4
            LDL ECX,1
            SYS 2
            POP EBX
            MOV SP,BP
            POP BP
            RET
