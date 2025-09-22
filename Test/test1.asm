MOV   EDX,DS
MOV   [EDX], 0x41 
SHL   [EDX], 8 
OR    [EDX], 'a' 
MOV   EDX, DS 
ADD   EDX, 12 
LDL   ECX, 1 
LDH   ECX, 2 
MOV   EAX, 0x0F 
SYS   0x2       