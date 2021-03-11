#ifndef era_vm_h
#define era_vm_h

#include "../era.h"



typedef enum objType{
    OBJ_CLASS,
}ObjType;
typedef struct obj
{
    ObjType type;
    struct obj *next;
}Obj;


typedef struct objstring
{
    Obj obj;
    u32 length;
    u32 hash;
    char *value;
}ObjString;

typedef enum methodType{
    METHOD_NATIVE,
    METHOD_NORMAL,
}MethodType;

typedef struct method{
    MethodType type;
    const char *name;
    union
    {
    }as;
}Method;


typedef struct objclass
{
    Obj obj;
    const char *name;
    u32 numFields;
    Method methods[METHOD_COUNT];
}ObjClass;

typedef struct frame
{
    u8 *return_point;
    int fp;
//    u32 nargs;
    Values *locals;
}Frame;

typedef struct eravm{
    ObjClass *objClass;

     Obj *objects;

    Frame frame;
    u8 program[PROG_SIZE];
    u32 program_size;
    int pc;
    u8 *pointer;

    //like constant pool in jvm: where large numbers and strings live
    //NB: maybe make this dynamically allocated? heap?
    Values constant_pool[POOL_SIZE];

    //stack
    Values stack[STACK_SIZE];
    int sp;
    Values *stackTop;

    //instructions
    u8 instr;
    bool state;
}ERAVM;

#define ALLOCATE_OBJ(vm, type, objType)\
        (type*)allocate_object(vm, sizeof(type), objType)

static Obj* allocate_object(ERAVM *vm, size_t size, ObjType type){
  Obj* object = (Obj*)malloc(size);
  object->type = type;
  object->next = vm->objects;
  vm->objects = object;
  return object;
}



#define PUSH(vm, value) ((vm)->stack[(vm)->sp++] = (value))
#define POP(vm) ((vm)->stack[--(vm)->sp])
#define PEEK(vm) (((vm)->stack[(vm)->sp-1]))
#define BINARY_OP(vm, op, valtype,type,more)\
          do {\
            type left = more(POP((vm)));\
            type right = more(POP((vm)));\
            PUSH((vm), valtype(left op right));\
          } while(false);       \

#define READ_BYTE(vm)  ((vm)->program[(vm)->pc++])
#define READ_SHORT(vm) ((vm)->pc +=2, (u16)((vm)->program[(vm)->pc-2] << 8)|((vm)->program[(vm)->pc-1]))
#define READ_CONST(vm) ((vm)->constant_pool[READ_BYTE((vm))])


#define TEMPLDCNST(vm,value,position)\
    (vm)->constant_pool[(position)] = FLOAT_VAL((value));\



typedef void (*Instructions)(ERAVM *vm);
typedef struct opt_instr{
    Instructions instrFn;
    Opcodes type;
}OPTINSTR;

ERAVM* init_vm();
Method* newMethod(ERAVM *vm, ObjClass *klass);
ObjClass* newClass(ERAVM *vm,ObjClass *superClass,int numFields,const char *name);
void writebyte(ERAVM *vm, u8 byte);
void load_file(ERAVM *vm, const char *path);
void run(ERAVM *vm);
void free_vm(ERAVM *vm);




#endif