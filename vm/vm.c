#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "vm.h"



ERAVM* init_vm(){
    ERAVM *vm = (ERAVM *)malloc(sizeof(ERAVM));
    vm->program_size = 0;
    vm->pc = 0;
    vm->sp = 0;
    vm->frame.return_point = NULL;
    vm->frame.fp = 0;
    vm->frame.locals = (Values *)malloc(sizeof(Values));
    vm->state = true;
    return vm;
}

void free_vm(ERAVM *vm){
    free(vm->frame.locals);
    free(vm);
}

ObjClass *newClass(ERAVM *vm,ObjClass *superClass,int numFields,const char *name){
    ObjString *sname;
    ObjClass *klass = ALLOCATE_OBJ(vm,ObjClass,OBJ_CLASS);
    // klass->superClass = superClass;
    klass->numFields = numFields;
    klass->name = sname;
    return klass;
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


static void fetch(ERAVM *vm){
    // vm->instr = *vm->pointer++;
    vm->instr = READ_BYTE(vm);
}

static void op_cpush(ERAVM* vm){
    PUSH(vm, CHAR_VAL(READ_BYTE(vm)));
}
static void op_spush(ERAVM* vm){
    PUSH(vm, SHORT_VAL(READ_SHORT(vm)));
}
static void op_cnst(ERAVM *vm){
    Values cnst = READ_CONST(vm);
    PUSH(vm, cnst);
}
static void op_load(ERAVM* vm){
    u8 addr = READ_BYTE(vm);
    Values val =  vm->frame.locals[addr];
    PUSH(vm, val);
}
static void op_store(ERAVM* vm){
    // LOADFRAME(vm);
    u8 addr = READ_BYTE(vm);
    //maybe we have different memory type locations for different data types
    vm->frame.locals[addr] = POP(vm);
}

static void op_pop(ERAVM* vm){
    POP(vm);
}
static void op_iadd(ERAVM* vm){
    BINARY_OP(vm,+,INT_VAL,u32,AS_INT);
}
static void op_iminus(ERAVM* vm){
    BINARY_OP(vm,-,INT_VAL,u32,AS_INT);
}
static void op_imulti(ERAVM* vm){
    BINARY_OP(vm,*,INT_VAL,u32,AS_INT);
}
static void op_idiv(ERAVM* vm){
    BINARY_OP(vm,/,INT_VAL,u32,AS_INT);
}

static void op_bit_and(ERAVM* vm){
    BINARY_OP(vm,&,INT_VAL,u32,AS_INT);
}

static void op_bit_or(ERAVM* vm){
    BINARY_OP(vm,|,INT_VAL,u32,AS_INT);
}

static void op_fadd(ERAVM* vm){
    BINARY_OP(vm,+,FLOAT_VAL,float,AS_FLOAT);
}
static void op_fmulti(ERAVM* vm){
    BINARY_OP(vm,*,FLOAT_VAL,float,AS_FLOAT);
}
static void op_fdiv(ERAVM* vm){
    BINARY_OP(vm,/,FLOAT_VAL,float,AS_FLOAT);
}
static void op_fminus(ERAVM* vm){
    BINARY_OP(vm,-,FLOAT_VAL,float,AS_FLOAT);
}
static void op_jump(ERAVM* vm){
     vm->pc = (int) READ_BYTE(vm);
}

static void op_loop(ERAVM *vm){
    u8 offset = READ_BYTE(vm);
    vm->pointer -= offset;
}

static void op_jumpf(ERAVM *vm){
    u32 jmp = AS_INT(POP(vm));
    if(jmp == 0){
        u8 offset = READ_BYTE(vm);
        vm->pointer += offset;
    }
}
static void op_cmp(ERAVM *vm){
    u32 left = AS_INT(POP(vm));
    u32 right = AS_INT(POP(vm));
    PUSH(vm, INT_VAL(left < right ? 1 : 0));
}


static void op_call(ERAVM *vm){
        u32 args = (u32)READ_BYTE(vm);
        u32 offset = (u32)READ_BYTE(vm);
        printf("ARGS: %d\n", args);
        PUSH(vm, INT_VAL(args));
        PUSH(vm, INT_VAL(vm->frame.fp));
        PUSH(vm,INT_VAL(vm->pc));
        vm->frame.fp = vm->sp;
        vm->pc = offset;
}
static void op_string(ERAVM *vm){

}

static void op_ret(ERAVM *vm){
     Values ret_value = POP(vm);
     vm->frame.fp = vm->sp;
     vm->pc = AS_INT(POP(vm));
     vm->frame.fp = AS_INT(POP(vm));
     vm->sp -= AS_INT(POP(vm));//nargs
     PUSH(vm,ret_value);
}
static void op_halt(ERAVM* vm){
    vm->state = false;
}
OPTINSTR opt[] = {
    {NULL, OP_NOP},
    {op_cpush, OP_CPUSH},
    {op_spush, OP_SPUSH},
    {op_cnst, OP_CONST},
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
    {op_jump, OP_JUMP},
    {op_jumpf, OP_JUMPF},
    {op_call, OP_CALL},
    // {op_string, OP_STRING},
    {op_ret, OP_RET},
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
            
    //         // u32 right = pop(vm).INT;
    //         // u32 left = pop(vm).INT;
    //         // Values result;
    //         // result.INT = left + right;
    //         // printf("ADD: %d\n", result.INT);
    //         BINARY_OP(vm,+,INT);
    //         // push(vm, result);
    //         break;
    //     }
        
    //     case OP_MINUS:{
    //         BINARY_OP(vm,-,INT);
    //         break;
    //     }
    //     case OP_DIVIDE:{
    //         BINARY_OP(vm, /, INT);
    //         break;
    //     }
    //     case OP_MULTI:{
    //         BINARY_OP(vm, *, INT);
    //         break;
    //     }
    //     case OP_BIT_OR:{
    //         BINARY_OP(vm, |, INT);
    //         break;
    //     }
    //     case OP_BIT_AND:{
    //         BINARY_OP(vm, &, INT);
    //         break;
    //     }
    //     case OP_GLOADI:{
            
    //         u32 addr = vm->instr.operand.INT;
    //         Values val;
    //         val.INT =  *(u32*)&vm->mem[addr];
    //         push(vm, val);
    //         break;
    //     }
    //     case OP_GSTOREI: {
    //         if(vm->sp < 1){
    //             printf("Stack underflow by %d\n", vm->sp);
    //         }
    //         // Values val = pop(vm);
    //         u32 addr = vm->instr.operand.INT;
    //         *(u32*)&vm->mem[addr] = (u32)vm->stack[(vm)->sp-1].INT;
    //         vm->sp -=1;
    //         break;
    //     }
    //     case OP_JUMP:{
    //         vm->pc = vm->instr.operand.INT;
    //         // vm->pc++;
    //         break;
    //     }
    //     case OP_JUMPF:{
    //         u32 jmp = pop(vm).INT;
    //         printf("%d\n", jmp);
    //         // assert(jmp == 0);
    //         if( jmp == 0){
    //             vm->pc = vm->instr.operand.INT;
    //         }

    //         break;   
    //     }
    //     case OP_LLOADI:{
    //         u32 offset = vm->instr.operand.INT;
    //         Values stack_lval;
    //         stack_lval.INT = vm->stack[vm->fp+offset].INT;
    //         push(vm, stack_lval);
    //         break;
    //     }
    //     case OP_CMP:{
    //         // Values right = pop(vm);
    //         // Values left = pop(vm);
    //         // Values result; 
    //         // // result.INT = (left.INT < right.INT) ? 1 : 0;
    //         // if(left.INT < right.INT){
    //         //     result.INT = 1;
    //         // }else{
    //         //     result.INT = 0;
    //         // }
    //         vm->stack[(vm)->sp-1].INT = (vm->stack[vm->sp-2].INT < vm->stack[vm->sp-1].INT) ? 1 : 0;
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
    //         nargs.INT = vm->instr.operand.INT;
    //         ip.INT = vm->pc;
    //         fp.INT = vm->fp;
    //         push(vm, nargs);
    //         push(vm, fp);
    //         push(vm, ip);
    //         vm->fp = vm->sp;
    //         vm->pc = vm->instr.operand.INT;
    //         break;
    //     }
    //     case OP_RET:{
    //         Values ret_value = pop(vm);
    //         vm->fp = vm->sp;
    //         vm->pc = pop(vm).INT;
    //         vm->fp = pop(vm).INT;
    //         u32 nargs = pop(vm).INT;
    //         vm->sp -= nargs;
    //         push(vm,ret_value);
    //         break;
    //     }
    //     default:
    //         break;
         
    // }
    Instructions exec = getOps(vm->instr)->instrFn;
  
    exec(vm);
  

}
#undef BINARY_OP
void run(ERAVM *vm){

    while (vm->state)
    {
        fetch(vm);
        execute(vm);
        
    }
    for (int i = 0; i < 10; i++)
    {
          printf("%d => LOCALS STORE: %c\n", i, AS_CHAR(vm->frame.locals[i]));
    }
    
   
    
}