MOV EAX,0X01
MOV EDX, DS
LDL ECX, 1
LDH ECX, 4
SYS 0X1
MOV EAX, 0X10
MOV EDX, DS 
LDL ECX, 1
LDH ECX, 4
SYS 0X2
STOP
//Dado un valor decimal ingresado por teclado, imprimir su valor binario equivalente como una
secuencia de ceros y unos.
