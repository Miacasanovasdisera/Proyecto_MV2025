INICIO:MOV EAX 0x10
	MOV EDX, DS
	LDL ECX, 1
	LDH ECX, 4
	SYS 1
	MOV EEX, [EAX]
LEO:MOV EAX 0x10
	MOV EDX, DS
	MOV EDX, 4
	LDL ECX, 1
	LDH ECX, 4
	SYS 1
	MOV EAX, [EDX]
	CMP EAX,0
	JN FIN
	AND EAX, EEX
	XOR AC, AC
OTRO:CMP EAX, 0
	JZ MUESTRA
	JNN SIGUE
	ADD AC, 1
SIGUE:SHL EAX, 1
	JMP OTRO
MUESTRA:MOV EAX 0x01
	ADD EDX,4
	MOV [EDX],AC
	LDH ECX, 0x04
	LDL ECX, 0x01
	SYS 2
	JMP LEO
FIN:STOP

\\Modificar el ejercicio anterior de modo que antes de ingresar la lista se lea un valor que
represente una máscara con la cual se debe realizar un AND a cada número de la lista antes
de calcular la cantidad de bits en 1.