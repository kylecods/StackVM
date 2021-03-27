#ifndef era_vm_h
#define era_vm_h

#include "../era.h"


typedef struct callframe
{
    Values *fpointer;
    u8 *ip;
}CallFrame;

typedef struct eravm{
    CallFrame frames[FRAME_SIZE];
    u32 frameCount;
    //cant have globals store here
    CallFrame *gframe;
   
    u8 program[PROG_SIZE];
    u32 program_size;
    u8 *pointer;

    //like constant pool in jvm: where large numbers and strings live
    //NB: maybe make this dynamically allocated?
    Values constant_pool[POOL_SIZE];

    //stack
    Values stack[STACK_SIZE];
    Values *stackTop;

    //instructions
    u8 instr;
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