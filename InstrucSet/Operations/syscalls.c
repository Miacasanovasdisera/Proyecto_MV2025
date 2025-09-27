#include "syscalls.h"
#include "../registers.h"
#include "../../Utils/errors.h"
#include "../helpers.h"

int execute_SYS(cpu_t *cpu, mem_t *mem){
    uint32_t ECXH,ECXL,physical_address;              // ECXH == me dice la cantidad de bytes que leo o escribo y ECXL == me dice la cantidad de veces que leo o escribo 'x' cantidad de bytes
    uint16_t index;                                  //el index me dice de donde empiezo a escribir o leer

    //leo la informacion del ECX
    ECXH = (read_register(cpu,R_ECX) & 0xFFFF0000) >> 16;
    ECXL = read_register(cpu,R_ECX) & 0x0000FFFF;
 
    //leo la direccion logica del EDX y la paso a fisica para saber de donde arranco a leer (se lo asigno a la variable index que se va a ir moviendo por donde leo)
    physical_address = cpu_logic_to_physic(*mem,read_register(cpu,R_EDX),4);
    index = physical_address;

    // verificar si el ecx y el edx sumados superan al tamaño del segmento
    if(index+ECXH*ECXL <= MEM_SIZE)
        
        if(get_operand_value(cpu->OP1) == 1)
            sys_read(mem,cpu,ECXH,ECXL,index);
        
        else if(get_operand_value(cpu->OP1) == 2)
            sys_write(*mem,cpu,ECXH,ECXL,index);
                
        else
            error_Output(INVALID_OPERAND); //no es ni read ni write    
            
    else   //no se puede leer o escribir pq supero el tamaño del segmento
        error_Output(MEMORY_ERROR);

    return 0;
}

void sys_read(mem_t *mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index){
    int32_t hexa,octal,binary,decimal,character,num;            // hexa,octal,binary,decimal y character son booleanos para saber que numero va a ingresar por teclado
    int16_t i,j;                                                 // num es el numero que se ingresa por teclado

    activate_booleans_syscall(read_register(cpu,R_EAX),&hexa,&octal,&binary,&decimal,&character);  // activa un booleano declarado mas arriba

    for(i = 0; i < ECXL ;i++){   // itera la cantidad de veces q tenga que leer por pantalla
        
        printf("[%.4X]: ",index);
        if(hexa) {
            printf("0x");
            scanf("%X",&num);
        }
        else if(octal){
            printf("0o");
            scanf("0o%o",&num);
        }
        else if(binary) {
            printf("0b");
            read_binary(&num);
        }
        else if(decimal) 
            scanf("%d",&num);
        
        else if(character)
            scanf("%c",&num);

        // Guardar el número en memoria (big-endian)
        for(j = 0; j < ECXH; j++)                            // itera hasta guardar el numero en la cantidad de celdas que me diga ECXH
            mem->data[index + j] = (uint8_t)((num >> (8 * (ECXH - 1 - j))) & 0xFF);
        
        index += ECXH;
    }
}

void sys_write(mem_t mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index){
    int32_t hexa,octal,binary,decimal,character,aux;        // hexa,octal,binary,decimal y character son booleanos para saber que mostrar en pantalla
    int16_t i,j;                                            // el aux es donde voy a guardar los elementos que leo del segmento

    activate_booleans_syscall(read_register(cpu,R_EAX),&hexa,&octal,&binary,&decimal,&character);  // activa los booleanos declarados mas arriba
    for(i = 0; i < ECXL ;i++){     // itera la cantidad de veces que tengo que leer 'x' cantidad de bytes
        aux = 0;
        for(j = 0; j < ECXH ;j++)
            aux = (aux << 8) | (uint8_t)mem.data[index + j];
            
        switch (ECXH) {
            case 1: aux &= 0xFF; break;
            case 2: aux &= 0xFFFF; break;
            case 4: aux &= 0xFFFFFFFF; break;
        }
        
        // muestra segun el booleano
        printf("[%.4X]:   ",index);
        if(hexa)
            printf("0x%X ",aux);
        if(octal)
            printf("0o%o ",aux);
        if(binary)
            print_binary(aux,ECXH);  //llama a la funcion que me muestra el numero en binario
        if(character)
            print_characters(aux,ECXH);
        if(decimal)
            printf("%d ",aux);

        index += ECXH;

        printf("\n") ;
        }

}

void activate_booleans_syscall(int32_t EAX,int32_t *hexadecimal,int32_t *octal,int32_t *binary,int32_t *decimal,int32_t *character){

    if(EAX & 0x00000010)
        *binary = 1;
    else
        *binary = 0;

    if(EAX & 0x00000008)
        *hexadecimal = 1;
    else
        *hexadecimal = 0;

    if(EAX & 0x00000004)
        *octal = 1;
    else
        *octal = 0;

    if(EAX & 0x00000002)
        *character = 1;
    else
        *character = 0;

    if(EAX & 0x00000001)
        *decimal = 1;
    else
        *decimal = 0;

}

void print_binary(int32_t num,int32_t ECXH){
    int32_t i,bits = 8 * ECXH;

    for (i = bits - 1; i >= 0; i--)     //comienza mostrando de mas significativos a menos significativo y se va decrementando la cantidad de shifts
        printf("%d", (num >> i) & 1);

    printf(" ");
}

void read_binary(int32_t *num){
    char binary_number[32],length,i;
    scanf("%s",binary_number);
    *num = 0;
    length = strlen(binary_number);
    
    for(i=0; i<length ;i++)
        if(binary_number[i] == '1')
            *num += 1 << (length-1-i);
        
}

void print_characters(int32_t num,int32_t ECXH){
    char characters[4];     //vector q contiene los caracteres a mostrar
    int32_t i,j,aux;        // auxiliar contiene el numero a mostrar y j es el indice del vector 

    j=0;
    for(i=ECXH-1; i>=0; i--){
        aux = num;
        aux = (aux >> i*8) & 0x000000FF;
        if(aux>=32 && aux<127)
            characters[j]=aux;
        else
            characters[j]='.';
        j+=1;
    }
    printf("'");
    for(i=0;i<ECXH;i++)
        printf("%c",characters[i]);
    printf(" ");    
}