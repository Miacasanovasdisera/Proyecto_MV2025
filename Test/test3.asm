MOV EAX,0x01
MOV EDX, DS
LDL ECX, 1
LDH ECX, 4
SYS 0x1
MOV EAX, 0x10
MOV EDX, DS 
LDL ECX, 1
LDH ECX, 4
SYS 0x2
STOP

//Dado un valor decimal ingresado por teclado, imprimir su valor binario equivalente como una
secuencia de ceros y unos.