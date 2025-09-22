INICIO:MOV EAX 0x10
	MOV EDX, DS
	MOV EDX, 4
	LDL ECX, 1
	LDH ECX, 4
	SYS 1
	MOV [EAX], [EDX]
	CMP EAX,0
	JN FIN
	XOR AC, AC
OTRO:CMP EAX, 0
	JZ MUESTRA
	JNN SIGUE
	ADD AC, 1
SIGUE:SHL EAX, 1
	JMP OTRO
MUESTRA:MOV EAX 0x01
	ADD EDX,4
	MOV [EDX],[AC]
	LDH ECX, 0x04
	LDL ECX, 0x01
	SYS 2
	JMP INICIO
FIN:STOP
//Se ingresan una serie de números naturales, terminada con un número negativo. Mostrar por
cada número ingresado la cantidad de bits en 1 que contiene su representación binaria.
