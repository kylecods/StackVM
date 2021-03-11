#ifndef era_h
#define era_h

#include <stdbool.h>

#define _RED "\x1b[31m"
#define _GREEN "\x1b[32m"
#define _YELLOW "\x1b[33m"
#define _BLUE "\x1b[34m"
#define _MAGENTA "\x1b[35m"
#define _CYAN "\x1b[36m"
#define _RESET "\x1b[0m"

#include <stdint.h>

#define u64 uint64_t 
#define u32 uint32_t 
#define u16 uint16_t 
#define u8  uint8_t 

#define i32 int32_t




#define PROG_SIZE 1024
#define STACK_SIZE 1024
#define POOL_SIZE (1024 * 1024)
#define FRAME_SIZE 64
#define METHOD_COUNT 100

typedef enum valtype {
    VAL_CHAR,
    VAL_SHORT,
    VAL_INT,
    VAL_FLOAT,
    VAL_STRING
}ValType;

typedef struct values{
    ValType type;
    union{
        u8  CHAR;
        u16 SHORT;
        u32 INT;
        double FLOAT;
        void *ptr;
    }as;
}Values;

#define IS_CHAR(value) ((value).type == VAL_CHAR)
#define IS_SHORT(value) ((value).type == VAL_SHORT)
#define IS_INT(value) ((value).type == VAL_INT)
#define IS_FLOAT(value) ((value).type == VAL_FLOAT)




#define AS_CHAR(value) ((value).as.CHAR)
#define AS_SHORT(value) ((value).as.SHORT)
#define AS_INT(value) ((value).as.INT)
#define AS_FLOAT(value) ((value).as.FLOAT)


#define CHAR_VAL(value)   ((Values){ VAL_CHAR, {.CHAR = (value) } })
#define SHORT_VAL(value)  ((Values){ VAL_SHORT, {.SHORT = (value) } })
#define INT_VAL(value)    ((Values){ VAL_INT,  {.INT = (value) } })
#define FLOAT_VAL(value)  ((Values){ VAL_FLOAT, {.FLOAT = (value) } })
#define OBJ_VAL(value)  ((Values){ VAL_STRING, {.ptr = (value) } })

#define PRINT_CHAR(value)\
    printf("%c", AS_CHAR((value)))\

#define PRINT_SHORT(value)\
    printf("%u", AS_SHORT((value)))\

#define PRINT_INT(value)\
    printf("%d", AS_INT((value)))\

#define PRINT_FLOAT(value)\
    printf("%g", AS_FLOAT((value)))\


#define PRINT_OBJ()\
    printf("%s", "<main>")\

static inline void PRINTVAL(Values value){
    switch (value.type){
        case VAL_CHAR:
            PRINT_CHAR(value);
            break;
        case VAL_SHORT:
            PRINT_SHORT(value);
            break;
        case VAL_INT:
            PRINT_INT(value);
            break;
        case VAL_FLOAT:
            PRINT_FLOAT(value);
            break;  
        case VAL_STRING:
           printf("%s", "<main>");
           break;
    }
}


typedef enum opcodes{ 
    #define OPCODE(name) OP_##name,
    #include "./vm/opcode.h"
    #undef OPCODE
}Opcodes;

typedef struct instr
{
    Opcodes type;
    Values operand;
}Instr;


#endif