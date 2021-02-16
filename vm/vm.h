#ifndef era_vm_h
#define era_vm_h

#include "../era.h"




typedef struct eravm{
    //TODO: change later to maybe 64-bit
    u8 program[PROG_SIZE];
    u32 program_size;
    u8 *pointer;

    //constant pool,jvm: where large numbers and strings live
    Values constant_pool[STACK_SIZE];

    //stack
    Values stack[STACK_SIZE];
    Values *stackTop;
    u32 sp;
    u32 fp;

    //memory
    Values mem[MEM_SIZE];

    //instructions
    u8 instr;
    u8 pc;
    bool state;
}ERAVM;
typedef void (*Instructions)(ERAVM *vm);
typedef struct opt_instr{
    Instructions instrFn;
    Opcodes type;
}OPTINSTR;

ERAVM *init_vm();
void writebyte(ERAVM *vm, u8 byte);
void load_file(ERAVM *vm, const char *path);
void run(ERAVM *vm);
void free_vm(ERAVM *vm);

#endif