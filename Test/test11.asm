    MOV EAX,0X01
	MOV EDX, DS
	LDL ECX, 1
	LDH ECX, 4
	SYS 0X1 //hay q construir el vector, aca empieza la lectura me falta hacer lo demas
    MOV ECX, 0 //i=0, base del vector, donde empieza
	MOV EBX, DS
	ADD EBX, 1
	CMP ECX, [DS] ??? //i<N
	JNN FIN
	MOV EBX, 4 // hasta aca: BASE + * tamcelda; x eso use EBX (EBaseX) todo esto p acceder al vector 
INICIA:MOV EFX, [EBX]
	MOV EAX, 0 //inicializo cant aparencias
SUMA:ADD EAX, 1 
OTRO:ADD ECX, 1
	CMP ECX, [DS]
	JNN FIN  
	MOV EBX, 4
	CMP [EFX], [EBX]
	JN INICIA
	JZ SUMA
	JUMP OTRO
FIN:MOV EDX, DS
	LDH ECX, 4
	LDL ECX, 1
	MOV EAX, 0x01
	SYS2

// Desarrollar un algoritmo que permita hallar el valor máximo de un vector de enteros y su
cantidad de apariciones.
