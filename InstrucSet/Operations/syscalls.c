#include "syscalls.h"
#include "../registers.h"
#include "../../Utils/errors.h"
#include "../helpers.h"
#include "../../Images/vmi.h"

//! Para el breakpoint, usar save_vmi(cpu, mem, g_vmi_filename); g_vmi_filename (var global)
// porque a execute_SYS no le paso el nombre del archivo y en main tengo el nombre del archivo .vmi

int execute_SYS(cpu_t *cpu, mem_t *mem){
    uint32_t ECXH,ECXL,physical_address,EDX;              // ECXH == me dice la cantidad de bytes que leo o escribo y ECXL == me dice la cantidad de veces que leo o escribo 'x' cantidad de bytes
    uint16_t index;                                  //el index me dice de donde empiezo a escribir o leer

    //leo la informacion del ECX y EDX
    ECXH = (read_register(cpu,R_ECX) & 0xFFFF0000) >> 16;
    ECXL = read_register(cpu,R_ECX) & 0x0000FFFF;
    EDX= read_register(cpu,R_EDX);
    //leo la direccion logica del EDX y la paso a fisica para saber de donde arranco a leer (se lo asigno a la variable index que se va a ir moviendo por donde leo)
    physical_address = cpu_logic_to_physic(*mem,EDX,4);
    index = physical_address;
        
    if(  ==2){            // verifica en que version esta
     switch(get_operand_value){
        case 1:sys_read(mem,cpu,ECXH,ECXL,index);
                break;
        case 2:sys_write(mem,cpu,ECXH,ECXL,index);
                break;
        case 3:sys_string_read(mem,CPU,ECXL,index);
                break;
        case 4:sys_string_write(mem,CPU,index,EDX >> 16);
                break;
        case 7:sys_clear();
                break;
        case 15:sys_breackpoint();
                break;

        default:error_Output(INVALID_OPERAND); //no es ninguno de las opciones 
                break;
        } 
       }
    else{
        switch(get_operand_value){
            case 1:sys_read(mem,cpu,ECXH,ECXL,index);
                    break;
            case 2:sys_write(mem,cpu,ECXH,ECXL,index);
                    break;
            default:error_Output(INVALID_OPERAND); //no es ninguno de las opciones
                break;  
            }
        }


    return 0;
}

void sys_read(mem_t *mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index){
    int32_t hexa,octal,binary,decimal,character,num;            // hexa,octal,binary,decimal y character son booleanos para saber que numero va a ingresar por teclado
    int16_t i,j;                                                 // num es el numero que se ingresa por teclado
    
    //chequeo si lo voy a leer a partir de 'EDX' supera el segmento donde estoy
    if(index+ECXH*ECXL>=mem->segments[read_register(cpu,R_EDX) >> 16].base && index+ECXH*ECXL<=mem->segments[read_register(cpu,R_EDX); >> 16].size){
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
    else   //no se puede leer pq supero el tamaño del segmento
        error_Output(MEMORY_ERROR);

}

void sys_write(mem_t mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index){
    int32_t hexa,octal,binary,decimal,character,aux;        // hexa,octal,binary,decimal y character son booleanos para saber que mostrar en pantalla
    int16_t i,j;                                            // el aux es donde voy a guardar los elementos que leo del segmento

    if(index+ECXH*ECXL>=mem->segments[read_register(cpu,R_EDX) >> 16].base && index+ECXH*ECXL<=mem->segments[read_register(cpu,R_EDX); >> 16].size){
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
    else   //no se puede excribir pq supero el tamaño del segmento
        error_Output(MEMORY_ERROR);

    

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





void sys_string_read(mem_t *mem,cpu_t *cpu,int32_t CX,int16_t index){
    char characters[];      //cadena ingresada por teclado
    int32_t i;

    if(index+ECXH*ECXL>=mem->segments[read_register(cpu,R_EDX) >> 16].base && index+ECXH*ECXL<=mem->segments[read_register(cpu,R_EDX); >> 16].size){
    scanf("%s",cad);
    if(CX==-1)
        for(i=0;i<strlen(cad);i++)
            mem.data[index+i]=characters[i];

    else
        for(i=0;i<=CX;i++)
            mem.data[index+i]=characters[i];


}


void sys_string_write(mem_t *mem,cpu_t *cpu,int16_t index,int16_t segment){     //la variable segment me dice en que segmento estoy parando, para no superarlo luego ya que leo hasta encontrar un \0
    int i=0;
    int32_t segment_end;

    segment_end=mem->segments[segment].size+mem->segments[segment].base // me dice la posicion del final del segmento asi no la supero leyendo

    while(index+i <= segment_end && mem.data[index+i] != 0x00){   //me fijo si hay un '\o' o un me cai del segmento
        print("%c",mem.data[index+i])
        i++
    }

}


void sys_clear(){


}

void sys_breackpoint(){




}
