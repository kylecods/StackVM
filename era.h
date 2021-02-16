#ifndef era_h
#define era_h

#include <stdbool.h>

#define _RED "\x1b[31m"
#define _RESET "\x1b[0m"

#include <stdint.h>

#define u64 uint64_t 
#define u32 uint32_t 
#define u16 uint16_t 
#define u8  uint8_t 

#define i32 int32_t


#define PROG_SIZE 1024
#define STACK_SIZE 1024
#define MEM_SIZE (640 * 1000)



typedef union values{
    u32 UINT;
    i32 INT;
    double FLOAT;
    void *ptr;
}Values;

typedef enum opcodes{ 
    #define OPCODE(name) OP_##name,
    #include "vm/opcode.h"
    #undef OPCODE
}Opcodes;

typedef struct instr
{
    Opcodes type;
    Values operand;
}Instr;


#endif