#ifndef era_vm_h
#define era_vm_h

#include "../era.h"

typedef struct eravm{
    //TODO: change later to maybe 64-bit
    Instr program[PROG_SIZE];
    u32 program_size;
    Instr *pointer;

    //stack
    Values stack[STACK_SIZE];
    // Values *stackTop;
    u32 sp;
    u32 fp;

    //memory
    uint8_t mem[MEM_SIZE];

    //instructions
    Instr instr;
    u32 pc;
}ERAVM;
typedef void (*Instructions)(ERAVM *vm);
typedef struct opt_instr{
    Instructions instrFn;
    Opcodes type;
}OPTINSTR;

ERAVM *init_vm();
void writeVal(ERAVM *vm, Instr instr);
void load_file(ERAVM *vm, const char *path);
void run(ERAVM *vm);
void free_vm(ERAVM *vm);

#endif