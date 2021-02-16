#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "vm.h"
#if 0
(vm)->stack[(vm)->sp-1].type = (vm)->stack[(vm)->sp-2].type op (vm)->stack[(vm)->sp-1].type;
#endif


ERAVM* init_vm(){
    ERAVM *vm = (ERAVM *)malloc(sizeof(ERAVM));
    vm->pointer = vm->program;
    vm->program_size = 0;
    vm->stackTop = vm->stack;
    vm->pc = 0;
    vm->sp = 0;
    vm->fp = 0;
    vm->state = true;
    return vm;
}

void free_vm(ERAVM *vm){
    free(vm);
}

static void fetch(ERAVM *vm){
    vm->instr = *vm->pointer++;
}

//load instructions 
void writebyte(ERAVM *vm, u8 byte){
    vm->program[vm->program_size] = byte;
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
    vm->program_size = size;
    fclose(rp);
}

#define PUSH(vm, value) (*(vm)->stackTop++ = value)
#define POP(vm) (*(--(vm)->stackTop))
#define DROP(vm) ((vm)->stackTop--)
#define BINARY_OP(vm, op, valtype,type)\
          do {\
            if((vm)->sp < 2){\
                printf("Stack underflow by %d\n", (vm)->sp);\
            }\
            Values result;\
            type left = POP(vm).valtype;\
            type right = POP(vm).valtype;\
            result.UINT = left op right; \
            PUSH(vm, result);\
          } while(false);       \

#define READ_BYTE(vm)  (*(vm)->pointer++)
#define READ_SHORT(vm) ((vm)->pointer+=2, (u16)((vm)->pointer[-2] << 8)|((vm)->pointer[-1]))
static void op_push(ERAVM* vm){
    Values val;
    val.UINT = READ_BYTE(vm);
    PUSH(vm, val);

}
static void op_load(ERAVM* vm){
    u8 addr = READ_BYTE(vm);
    Values val =  vm->mem[addr];
    PUSH(vm, val);
}
static void op_store(ERAVM* vm){
    
    u8 addr = READ_BYTE(vm);
    vm->mem[addr] = POP(vm);
}

static void op_pop(ERAVM* vm){
    DROP(vm);
}
static void op_iadd(ERAVM* vm){
    BINARY_OP(vm,+,UINT,u32);
}
static void op_iminus(ERAVM* vm){
    BINARY_OP(vm,-,UINT,u32);
}
static void op_imulti(ERAVM* vm){
    BINARY_OP(vm,*,UINT,u32);
}
static void op_idiv(ERAVM* vm){
    BINARY_OP(vm,/,UINT,u32);
}

static void op_bit_and(ERAVM* vm){
    BINARY_OP(vm,&,UINT,u32);
}

static void op_bit_or(ERAVM* vm){
    BINARY_OP(vm,|,UINT,u32);
}

static void op_fadd(ERAVM* vm){
    BINARY_OP(vm,+,FLOAT,float);
}
static void op_fmulti(ERAVM* vm){
    BINARY_OP(vm,*,FLOAT,float);
}
static void op_fdiv(ERAVM* vm){
    BINARY_OP(vm,/,FLOAT,float);
}
static void op_fminus(ERAVM* vm){
    BINARY_OP(vm,-,FLOAT,float);
}
static void op_jump(ERAVM* vm){
     vm->pointer += READ_BYTE(vm);
}

static void op_loop(ERAVM *vm){
    u8 offset = READ_BYTE(vm);
    vm->pointer -= offset;
}

static void op_jumpf(ERAVM *vm){
    u32 jmp = vm->stack[vm->sp-1].UINT;
    if(jmp == 0){
        u8 offset = READ_BYTE(vm);
        vm->pointer += offset;
    }
}
static void op_cmp(ERAVM *vm){
    vm->stack[(vm)->sp-1].UINT = (vm->stack[vm->sp-2].UINT < vm->stack[vm->sp-1].UINT) ? 1 : 0;
}


static void op_call(ERAVM *vm){
         Values ip;
         Values fp;
        //  Values nargs; 
        //  nargs.UINT = vm->instr.operand.UINT;
         ip.UINT = vm->pc;
         fp.UINT = vm->fp;
        //  push(vm, nargs);
         PUSH(vm, fp);
         PUSH(vm, ip);
         vm->fp = vm->sp;
         vm->pc = vm->instr;
}

static void op_ret(ERAVM *vm){
    //  Values ret_value = pop(vm);
    //  printf("%d\n", ret_value.UINT);
    //  vm->fp = vm->sp;
    //  vm->pc = pop(vm).UINT;
    //  vm->fp = pop(vm).UINT;
    //  u32 nargs = pop(vm).UINT;
    //  vm->sp -= nargs;
    // push(vm,ret_value);
}
static void op_halt(ERAVM* vm){
    vm->state = false;
}

#undef READ_BYTE
#undef READ_SHORT
#undef BINARY_OP
#undef PUSH
#undef POP
#undef DROP

OPTINSTR opt[] = {
    {NULL, OP_NOP},
    {op_push, OP_PUSH},
    {op_pop, OP_POP},
    {op_iadd, OP_IADD},
    {op_iminus, OP_IMINUS},
    {op_imulti, OP_IMULTI},
    {op_idiv, OP_IDIVIDE},
    {op_fadd, OP_FADD},
    {op_fminus, OP_FMINUS},
    {op_fmulti, OP_FMULTI},
    {op_fdiv, OP_FDIVIDE},
    {op_bit_and, OP_BIT_AND},
    {op_bit_or, OP_BIT_OR},
    {op_load, OP_LOAD},
    {op_store, OP_STORE},
    {op_loop, OP_LOOP},
    {op_cmp, OP_CMP},
    // {op_jump, OP_JUMP},
    {op_jumpf, OP_JUMPF},
    // {NULL, OP_NOP},
    // {op_call, OP_CALL},
    // {op_ret, OP_RET},
    {op_halt, OP_HALT}

};
OPTINSTR* getOps(u8 codes){
    return &opt[codes];
}




static void execute(ERAVM *vm){
    // switch (vm->instr.type)
    // {
    //     case OP_PUSH:{
    //         if(vm->sp >= STACK_SIZE){
    //             printf("Stack overflow by %d\n", vm->sp);
    //         }
    //         push(vm,vm->instr.operand);
    //         break;
    //     }
    //     case OP_POP:{
    //         pop(vm);
    //         break;
    //     }
    //     case OP_ADD:{
            
    //         // u32 right = pop(vm).UINT;
    //         // u32 left = pop(vm).UINT;
    //         // Values result;
    //         // result.UINT = left + right;
    //         // printf("ADD: %d\n", result.UINT);
    //         BINARY_OP(vm,+,UINT);
    //         // push(vm, result);
    //         break;
    //     }
        
    //     case OP_MINUS:{
    //         BINARY_OP(vm,-,UINT);
    //         break;
    //     }
    //     case OP_DIVIDE:{
    //         BINARY_OP(vm, /, UINT);
    //         break;
    //     }
    //     case OP_MULTI:{
    //         BINARY_OP(vm, *, UINT);
    //         break;
    //     }
    //     case OP_BIT_OR:{
    //         BINARY_OP(vm, |, UINT);
    //         break;
    //     }
    //     case OP_BIT_AND:{
    //         BINARY_OP(vm, &, UINT);
    //         break;
    //     }
    //     case OP_GLOADI:{
            
    //         u32 addr = vm->instr.operand.UINT;
    //         Values val;
    //         val.UINT =  *(u32*)&vm->mem[addr];
    //         push(vm, val);
    //         break;
    //     }
    //     case OP_GSTOREI: {
    //         if(vm->sp < 1){
    //             printf("Stack underflow by %d\n", vm->sp);
    //         }
    //         // Values val = pop(vm);
    //         u32 addr = vm->instr.operand.UINT;
    //         *(u32*)&vm->mem[addr] = (u32)vm->stack[(vm)->sp-1].UINT;
    //         vm->sp -=1;
    //         break;
    //     }
    //     case OP_JUMP:{
    //         vm->pc = vm->instr.operand.UINT;
    //         // vm->pc++;
    //         break;
    //     }
    //     case OP_JUMPF:{
    //         u32 jmp = pop(vm).UINT;
    //         printf("%d\n", jmp);
    //         // assert(jmp == 0);
    //         if( jmp == 0){
    //             vm->pc = vm->instr.operand.UINT;
    //         }

    //         break;   
    //     }
    //     case OP_LLOADI:{
    //         u32 offset = vm->instr.operand.UINT;
    //         Values stack_lval;
    //         stack_lval.UINT = vm->stack[vm->fp+offset].UINT;
    //         push(vm, stack_lval);
    //         break;
    //     }
    //     case OP_CMP:{
    //         // Values right = pop(vm);
    //         // Values left = pop(vm);
    //         // Values result; 
    //         // // result.UINT = (left.UINT < right.UINT) ? 1 : 0;
    //         // if(left.UINT < right.UINT){
    //         //     result.UINT = 1;
    //         // }else{
    //         //     result.UINT = 0;
    //         // }
    //         vm->stack[(vm)->sp-1].UINT = (vm->stack[vm->sp-2].UINT < vm->stack[vm->sp-1].UINT) ? 1 : 0;
    //         // printf("stack top cmp: %d\n", vm->stack[vm->sp-2]);
    //         // vm->sp -=1;
    //         // push(vm, result);
    //         break;
    //     }
    //     case OP_LOOP:{
    //         break;
    //     }
    //     case OP_CALL:{
    //         Values ip;
    //         Values fp;
    //         Values nargs; 
    //         nargs.UINT = vm->instr.operand.UINT;
    //         ip.UINT = vm->pc;
    //         fp.UINT = vm->fp;
    //         push(vm, nargs);
    //         push(vm, fp);
    //         push(vm, ip);
    //         vm->fp = vm->sp;
    //         vm->pc = vm->instr.operand.UINT;
    //         break;
    //     }
    //     case OP_RET:{
    //         Values ret_value = pop(vm);
    //         vm->fp = vm->sp;
    //         vm->pc = pop(vm).UINT;
    //         vm->fp = pop(vm).UINT;
    //         u32 nargs = pop(vm).UINT;
    //         vm->sp -= nargs;
    //         push(vm,ret_value);
    //         break;
    //     }
    //     default:
    //         break;
         
    // }
    // u8 instr = *vm->pointer++;
    //  printf("Instr: %u\n", mv_>instr);
    Instructions exec = getOps(vm->instr)->instrFn;
    exec(vm);
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
    // printf("Instr: %u\n", vm->state);
    // for (int i = 0; i < 20 && !vm->state; i++)
    while (vm->state)
    {
        fetch(vm);

        // printf("Instr: %u\n", vm->instr);
        execute(vm);
        
    }
   
    
}