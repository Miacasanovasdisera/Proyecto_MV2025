\\DATA 2000
\\EXTRA 10

SLEN:       PUSH BP
            MOV BP,SP
            MOV EAX,[BP+8]
BUCLE:      CMP [EAX],0
            JZ SLEN_FIN
            ADD EAX,1
            JMP BUCLE
SLEN_FIN:   SUB EAX, [BP+8]
            MOV SP,BP
            POP BP
            RET

MAIN:       PUSH BP
            MOV BP,SP
            PUSH EAX
            PUSH EBX
            PUSH ECX
            PUSH EDX
            MOV EDX, DS
            MOV ECX, -1
            SYS 3
            PUSH DS
            CALL SLEN
            ADD SP,4
            MOV [DS+100], EAX
            MOV EDX,DS
            ADD EDX,100
            MOV EAX,1
            LDH ECX,4
            LDL ECX,1
            SYS 2
            POP EDX
            POP ECX
            POP EBX
            POP EAX
            MOV SP,BP
            POP BP
            RET