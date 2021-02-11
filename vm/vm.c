#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "vm.h"

// uint8_t program[100];
// uint8_t *pointer = program;

// void emit(uint8_t value){
//     *pointer = value;
//     pointer++;
// }

static void push(ERAVM *vm, Values val){
    //to read the stack
    vm->stack[vm->sp++] = val;
    // vm->stackTop++;
}
static Values pop(ERAVM *vm){
    // vm->sp--;
    // vm->stackTop--;
    return vm->stack[--vm->sp];
}

ERAVM* init_vm(){
    ERAVM *vm = (ERAVM *)malloc(sizeof(ERAVM));
    vm->pointer = vm->program;
    vm->program_size = 0;
    // vm->stackTop = vm->stack;
    vm->pc = 0;
    vm->sp = 0;
    vm->fp = 0;
    return vm;
}
void free_vm(ERAVM *vm){
    free(vm);
}

static void fetch(ERAVM *vm){
    vm->instr = vm->program[vm->pc++];
}

//load instructions 
void writeVal(ERAVM *vm, Instr value){
    *vm->pointer = value;
    vm->pointer++;
    vm->program_size +=1;
}

void load_file(ERAVM *vm, const char *path){
    FILE *rp = fopen(path, "rb");
    if(rp == NULL){
        fprintf(stderr, "Could not open file\n");
        exit(74);
    }
    u32 size = 0;
    u32 meta_prog_size = fread(&size, sizeof(size), 1,rp);
    if (meta_prog_size > PROG_SIZE) {
        puts(_RED "Sorry your program is too large" _RESET);
        exit(1);
    }
    size_t bytes_read = fread(vm->program, sizeof(vm->program[0]),size,rp);
    if(bytes_read < size){
        fprintf(stderr, "Could not read file\n");
        exit(74);
    }
    fclose(rp);
}

#define BINARY_OP(vm, op, type)\
          do {\
            if((vm)->sp < 2){\
                printf("Stack underflow by %d\n", (vm)->sp);\
            }\
            (vm)->stack[(vm)->sp-1].type = (vm)->stack[(vm)->sp-2].type op (vm)->stack[(vm)->sp-1].type;\
          } while(false);       \


static void execute(ERAVM *vm){
    switch (vm->instr.type)
    {
        case OP_PUSH:{
            if(vm->sp >= STACK_SIZE){
                printf("Stack overflow by %d\n", vm->sp);
            }
            push(vm,vm->instr.operand);
            break;
        }
        case OP_POP:{
            pop(vm);
            break;
        }
        case OP_ADD:{
            
            // u32 right = pop(vm).UINT;
            // u32 left = pop(vm).UINT;
            // Values result;
            // result.UINT = left + right;
            // printf("ADD: %d\n", result.UINT);
            BINARY_OP(vm,+,UINT);
            // push(vm, result);
            break;
        }
        
        case OP_MINUS:{
            BINARY_OP(vm,-,UINT);
            break;
        }
        case OP_DIVIDE:{
            BINARY_OP(vm, /, UINT);
            break;
        }
        case OP_MULTI:{
            BINARY_OP(vm, *, UINT);
            break;
        }
        case OP_BIT_OR:{
            BINARY_OP(vm, |, UINT);
            break;
        }
        case OP_BIT_AND:{
            BINARY_OP(vm, &, UINT);
            break;
        }
        case OP_GLOADI:{
            
            u32 addr = vm->instr.operand.UINT;
            Values val;
            val.UINT =  *(u32*)&vm->mem[addr];
            push(vm, val);
            break;
        }
        case OP_GSTOREI: {
            if(vm->sp < 1){
                printf("Stack underflow by %d\n", vm->sp);
            }
            // Values val = pop(vm);
            u32 addr = vm->instr.operand.UINT;
            *(u32*)&vm->mem[addr] = (u32)vm->stack[(vm)->sp-1].UINT;
            vm->sp -=1;
            break;
        }
        case OP_JUMP:{
            vm->pc = vm->instr.operand.UINT;
            // vm->pc++;
            break;
        }
        case OP_JUMPF:{
            u32 jmp = pop(vm).UINT;
            printf("%d\n", jmp);
            // assert(jmp == 0);
            if( jmp == 0){
                vm->pc = vm->instr.operand.UINT;
            }

            break;   
        }
        case OP_LLOADI:{
            u32 offset = vm->instr.operand.UINT;
            Values stack_lval;
            stack_lval.UINT = vm->stack[vm->fp+offset].UINT;
            push(vm, stack_lval);
            break;
        }
        case OP_CMP:{
            // Values right = pop(vm);
            // Values left = pop(vm);
            // Values result; 
            // // result.UINT = (left.UINT < right.UINT) ? 1 : 0;
            // if(left.UINT < right.UINT){
            //     result.UINT = 1;
            // }else{
            //     result.UINT = 0;
            // }
            vm->stack[(vm)->sp-1].UINT = (vm->stack[vm->sp-2].UINT < vm->stack[vm->sp-1].UINT) ? 1 : 0;
            // printf("stack top cmp: %d\n", vm->stack[vm->sp-2]);
            // vm->sp -=1;
            // push(vm, result);
            break;
        }
        case OP_LOOP:{
            break;
        }
        case OP_CALL:{
            Values ip;
            Values fp;
            Values nargs; 
            nargs.UINT = vm->instr.operand.UINT;
            ip.UINT = vm->pc;
            fp.UINT = vm->fp;
            push(vm, nargs);
            push(vm, fp);
            push(vm, ip);
            vm->fp = vm->sp;
            vm->pc = vm->instr.operand.UINT;
            break;
        }
        case OP_RET:{
            Values ret_value = pop(vm);
            vm->fp = vm->sp;
            vm->pc = pop(vm).UINT;
            vm->fp = pop(vm).UINT;
            u32 nargs = pop(vm).UINT;
            vm->sp -= nargs;
            push(vm,ret_value);
            break;
        }
        default:
            break;
         
    }
}
#undef BINARY_OP
//TESTING
void run(ERAVM *vm){
   //write binary file
    // FILE *fp = fopen("test.ebin", "wb");

    // // size_t element_size = sizeof *program;
    // size_t element_size = vm->pointer - vm->program;
    // size_t elements_to_write = 1;

    // //write to file
    // size_t written = fwrite(vm->program, element_size, elements_to_write, fp);

    // if(written != elements_to_write){
    //     fprintf(stderr, "failed wrote only %lu out of %lu elements.\n", 
    //     written, elements_to_write);
    //     exit(EXIT_FAILURE);
    // }
    // fclose(fp); 
   
    while (vm->instr.type != OP_HALT)
    {
        fetch(vm);
        // printf("Instr: %d\n", vm->instr);
        execute(vm);
        
    }
   
    
}