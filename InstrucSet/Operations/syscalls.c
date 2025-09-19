#include "syscalls.h"

void execute_sys(cpu_t *cpu, mem_t *mem){
    uint32_t ECXH,ECXL,physical_addres;              // ECXH == me dice la cantidad de bytes que leo o escribo y ECXL == me dice la cantidad de veces que leo o escribo 'x' cantidad de bytes
    uint16_t index;                                  //el index me dice de donde empiezo a escribir o leer


    //leo la informacion del ECX
    ECXH = read_register(cpu,R_ECX) & 0xFFFF0000;
    ECXH = ECXH >> 16;
    ECXL= read_register(cpu,R_ECX) & 0x0000FFFF;


    //leo la direccion logica del EDX y la paso a fisica para saber de donde arranco a leer (se lo asigno a la variable index que se va a ir moviendo por donde leo)
    pyshical_addres = cpu_logic_to_physic(*mem,read_register(cpu,R_EDX),4);
    index = physycal_addres & 0xFFFF;


    // verificar si el ecx y el edx sumados superan al tamaño del segmento

    if(index+ECXH*ECXL<16384){
        if(get_operand_value((*cpu)->OP1)==1)
        sys_read(mem,cpu,ECXH,ECXL,index);
        else{
            if(get_operand_value((*cpu)->OP1)==2){
                sys_write(*mem,cpu,ECXH,ECXL,index);
                }
            else
                //error
            }
    }
    else
        //no se puede leer o escribir pq supero el tamaño del segmento

}


void sys_read(mem_t mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index){
    int32_t hexa,octal,binary,decimal,character,num;            // hexa,octal,binary,decimal y character son booleanos para saber que numero va a ingresar por teclado
    int16_t i,j;                                                 // num es el numero que se ingresa por teclado

    activate_booleans_syscall(read_register(cpu,R_EAX),&hexa,&octal,&binary,&decimal,&characters);  // activa un booleano declarado mas arriba

    for(i=0; i <= ECXL ;i++){   // itera la cantidad de veces q tenga que leer por pantalla
        
        if(hexa)
            scanf("%x",&num)
        else if(octal)
            scanf("%o",&num)
        else if(binary)
            read_binary(&num)
        else if(decimal)
            scanf("%d",&num)
        else if(character)
            scanf("%c",&num)
        
        k=0;

        for(j = ECXH-1; j>=0 ;j--){                             // itera hasta guardar el numero en la cantidad de celdas que me diga ECXH
            mem->data[index+j] = (num >> k*8) & 0xFF;       // comienzo a guardar el numero en el segmento del final hacia el inicio y k me va moviendo el num para guardar los 8 bit menos significativos
            k++;
        }
        index = index + ECXH;

    }


}


void sys_write(mem_t mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index){
    int32_t hexa,octal,binary,decimal,character,aux;        // hexa,octal,binary,decimal y character son booleanos para saber que mostrar en pantalla
    int16_t i,j;                                            // el aux es donde voy a guardar los elementos que leo del segmento

    activate_booleans_syscall(read_register(cpu,R_EAX),&hexa,&octal,&binary,&decimal,&characters);  // activa los booleanos declarados mas arriba

    for(i=0; i<=ECXL-1 ;i++){     // itera la cantidad de veces que tengo que leer 'x' cantidad de bytes
        aux = 0;
        
        for(j=0; j<=ECXH-1 ;j++){
            aux = aux | mem->data[index+j];
            aux = aux << 8;
        }

        // muestra segun el booleano
        if(hexa)
            printf("[%x]: %x ",index,aux);
        if(octal)
            printf("[%x]: %o ",index,aux);
        if(binary)
            print_binary(aux,ECXH,index);  //llama a la funcion que me muestra el numero en binario
        if(decimal)
            printf("[%x]: %d ",index,aux);
        if(character)
            printf("[%x]: %c ",index,aux);

        index = index + j;

        printf("\n") ;
        }

}

void activate_booleans_syscall(int32_t EAX,int32_t *hexadecimal,int32_t *octal,int32_t *binary,int32_t *decimal,int32_t *character){

    if(eax & 0x00000010)
        *hexadecimal = 1;
    else
        *hexadecimal = 0;

    if(eax & 0x00000008)
        *hexadecimal = 1;
    else
        *hexadecimal = 0;

    if(eax & 0x00000004)
        *octal = 1;
    else
        *octal = 0;

    if(eax & 0x00000002)
        *character = 1;
    else
        *character = 0;

    if(eax & 0x00000001)
        *decimal = 1;
    else
        *decimal = 0;

}


void print_binary(int32_t num,int32_t ECXH,int16_t index){
    int32_t i,bits = 8 * ECXH;

    printf("[%x]: 0b",index);
    for (i = bits - 1; i >= 0; i--)     //comienza mostrando de mas significativos a menos significativo y se va decrementando la cantidad de shifts
        printf("%d", (num >> i) & 1);

    printf(" ");
}


void read_binary(int32_t *num){
    char binary_number[32],lenght,i;
    scanf("%s",binary_number);
    *num = 0;
    lenght = strlen(binary_number);
    
    for(i=0; i<lenght ;i++)
        if(binary_number[i]=='1')
            *num += 1 << (lenght-1-i);
        
}
