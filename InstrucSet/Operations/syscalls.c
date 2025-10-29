#include "syscalls.h"
#include "../registers.h"
#include "../../Utils/errors.h"
#include "../helpers.h"
#include "../../Images/vmi.h"
#include "../instruct.h"

void clsBuffer(){
    char c;
    while((c = getchar()) != '\n' && c != EOF){}
    fflush(stdout);
}

int execute_SYS(cpu_t *cpu, mem_t *mem) {
    uint32_t ECXH,ECXL,physical_address,EDX;
    uint16_t index;                                  
    // ECXH me dice la cantidad de bytes que leo o escribo y ECXL me dice la cantidad de veces que leo o escribo 'x' cantidad de bytes
    // index me dice de donde empiezo a escribir o leer

    ECXH = (read_register(cpu,R_ECX) & 0xFFFF0000) >> 16;
    ECXL = read_register(cpu,R_ECX) & 0x0000FFFF;
    
    //leo la direccion logica del EDX y la paso a fisica para saber de donde arranco a leer (se lo asigno a la variable index que se va a ir moviendo por donde leo)
    physical_address = cpu_logic_to_physic(*mem,read_register(cpu,R_EDX),0);
    index = physical_address;
    
    switch(get_operand_value(cpu->OP1)) {
        case 1:sys_read(mem,cpu,ECXH,ECXL,index); break;
        case 2:sys_write(*mem,cpu,ECXH,ECXL,index); break;
        case 3: { 
            EDX = read_register(cpu,R_EDX);      //a las funciones nuevas las invoco con el numero del segmento,para saber en cual estoy
            sys_string_read(mem,cpu,ECXL,index,EDX >> 16); 
        } break;
        case 4:{ 
            EDX = read_register(cpu,R_EDX);      //a las funciones nuevas las invoco con el numero del segmento,para saber en cual estoy
            sys_string_write(*mem,index,EDX >> 16); 
        } break;
        case 7:sys_clear(); break;
        case 15:sys_breakpoint(cpu,mem); break;
        default:error_Output(INVALID_OPERAND); break;
    }
    
    return 0;
}

void sys_read(mem_t *mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index) {
    int32_t hexa,octal,binary,decimal,character,num,segment_end,segment;
    int16_t i,j;                                                 
    // hexa,octal,binary,decimal y character son booleanos para saber que numero va a ingresar por teclado
    // num es el numero que se ingresa por teclado
    
    segment = read_register(cpu,R_EDX) >> 16;
    
    segment_end = mem->segments[segment].base +  mem->segments[segment].size;
    
    //chequeo si lo voy a leer a partir de 'EDX' supera el segmento donde estoy
    if(index + ECXH * ECXL <= segment_end){
        // activa un booleano declarado mas arriba
        activate_booleans_syscall(read_register(cpu,R_EAX),&hexa,&octal,&binary,&decimal,&character);

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

void sys_write(mem_t mem,cpu_t *cpu,int32_t ECXH,int32_t ECXL,int16_t index) {
    int32_t hexa, octal, binary, decimal,character, aux, segment_end,segment;
    int16_t i, j;
    // hexa,octal,binary,decimal y character son booleanos para saber que mostrar en pantalla
    // el aux es donde voy a guardar los elementos que leo del segmento

    segment = read_register(cpu,R_EDX) >> 16;
    segment_end = mem.segments[segment].base +  mem.segments[segment].size;
    
    if(index + ECXH * ECXL <=segment_end){
        // activa los booleanos declarados mas arriba
        activate_booleans_syscall(read_register(cpu,R_EAX),&hexa,&octal,&binary,&decimal,&character);

        // itera la cantidad de veces que tengo que leer 'x' cantidad de bytes
        for(i = 0; i < ECXL ;i++){     
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
                print_binary(aux,ECXH);
            if(character)
                print_characters(aux,ECXH);
            if(decimal)
                printf("%d ",aux);
    
            index += ECXH;
    
            printf("\n");
        }
    }
    else       //no se puede excribir pq supero el tamaño del segmento
        error_Output(MEMORY_ERROR);
    
}

void activate_booleans_syscall(int32_t EAX,int32_t *hexadecimal,int32_t *octal,int32_t *binary,int32_t *decimal,int32_t *character) {

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

void print_binary(int32_t num,int32_t ECXH) {
    int32_t i,bits = 8 * ECXH;

    for (i = bits - 1; i >= 0; i--)     //comienza mostrando de mas significativos a menos significativo y se va decrementando la cantidad de shifts
        printf("%d", (num >> i) & 1);

    printf(" ");
}

void read_binary(int32_t *num) {
    char binary_number[32],length,i;
    
    scanf("%s",binary_number);
    *num = 0;
    length = strlen(binary_number);
    
    for(i=0; i<length ;i++)
        if(binary_number[i] == '1')
            *num += 1 << (length-1-i);      
}

void print_characters(int32_t num,int32_t ECXH) {
    char characters[4];     // characters contiene los caracteres a mostrar
    int32_t i,j,aux;        // aux contiene el numero a mostrar y j es el indice del vector 

    j = 0;
    for(i = ECXH - 1; i>=0; i--){
        aux = num;
        aux = (aux >> i*8) & 0x000000FF;
        if(aux >= 32 && aux < 127)
            characters[j] = aux;
        else
            characters[j]='.';
        j+=1;
    }
    
    printf("'");
    for(i = 0; i < ECXH; i++)
        printf("%c",characters[i]);
    
    printf(" ");    
}

void sys_string_read(mem_t *mem,cpu_t *cpu,int32_t CX,int16_t index,int16_t segment) {
    char characters[256];
    int32_t i = 0, segment_end;


    //obtengo el final del segmento para no superarlo
    segment_end = mem->segments[segment].size + mem->segments[segment].base;
    //? printf("[%.4X]:   ",index);
    scanf("%s", characters);
    clsBuffer();
    //tengo que leer hasta el final o no
    if(CX == 0xFFFF){
        //recorre hasta que la palabra termine o  hasta que me caiga del segmento
        while(index + i <= segment_end && i <= strlen(characters)){
            mem->data[index + i] = characters[i];
            i++;
        }
            
    }
    //verifica si lo que leo de la palabra indicado por 'CX' supero el segmento apuntado por 'EDX'
    else {
        if( index + CX <= segment_end){
        //leo la cantidad de veces indicada por 'CX'
        for(i = 0; i < CX; i++)
            mem->data[index + i] = characters[i];
        }
        else
            error_Output(MEMORY_ERROR);
    }

    //no se puede leer pq supero el tamaño del segmento
    
}

void sys_string_write(mem_t mem,int16_t index,int16_t segment) {
    int i = 0,aux;
    int32_t segment_end;
    // me dice la posicion del final del segmento asi no la supero leyendo
    segment_end = mem.segments[segment].size + mem.segments[segment].base;
    //me fijo si hay un '\0' o un me cai del segmento
    while(index + i <= segment_end && mem.data[index + i] != 0x00) {        
        aux = mem.data[index + i];

        if (aux >= 32 && aux < 127)
            printf("%c",mem.data[index + i]);
        else if (aux == 10)
            printf("\n");
        else
            printf(".");
        i++;
    }

    if(mem.data[index + i] != 0x00)
        error_Output(MEMORY_ERROR);      
}

void sys_clear(){
    system("cls");
    
}

void sys_breakpoint(cpu_t *cpu,mem_t *mem) {
    char character;
    int result;
    
    save_vmi(cpu,mem,g_vmi_filename);
    
    do {
        character = 0;

        printf("\nBreakpoint: ingrese accion para continuar... [g(go) / q(quit) / -Enter-]: ");
        scanf(" %c", &character);
        character = tolower(character);
        if (character == 'q')
            cpu->IP = -1;
        
        else if (character == 10)
            do {
                operators_registers_load(cpu, *mem);
                result = execute_instruction(cpu, mem);
                sys_breakpoint(cpu,mem);
            } while(character == 10);    

        else if (character != 'g')
            printf("Comando no reconocido, ingrese nuevamente.\n");
    } while(character != 'g' && character != 'q' && character != 10);
}