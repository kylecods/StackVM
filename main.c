#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "vm/vm.h"


int main(){

    Instr prog[] = { 
        {OP_PUSH, {.UINT = 7}}, 
        // OP_PUSH,4,
        // {OP_GSTOREI, {.UINT = 0}},
        {OP_PUSH, {.UINT = 4}},
        // {OP_GSTOREI, {.UINT = 1}},
        // {OP_STOREI, {.UINT = 5}},
        // {OP_LOADI, {.UINT = 5}},
        // {OP_STOREI, {.UINT = 0}},
        // {OP_GSTOREI, {.UINT = 0}},
        // {OP_GLOADI, {.UINT = 1}},
        // {OP_GLOADI, {.UINT = 0}},
        // {OP_ILT, {.UINT = 0}},
        // {OP_JUMPF, {.UINT = 13}},

        // {OP_GLOADI, {.UINT = 1}},
        // {OP_PUSH, {.UINT = 1}},
        {OP_ADD, {.UINT = 0}},
        {OP_PUSH, {.UINT = 10}},
        {OP_GSTOREI, {.UINT = 1}},
        // {OP_JUMP, {.UINT = 4}},
        // {OP_BIT_AND, {.UINT = 0}},
        // OP_ADD,
        // OP_PUSH, 1,
        // OP_MINUS,
        {OP_HALT,{.UINT = 0}}
    };
    ERAVM *vm = init_vm();
    // const char *path = "assembler/test1.ebin";
    // FILE *rp = fopen(path, "rb");
    // if(rp == NULL){
    //     fprintf(stderr, "Could not open file");
    //     exit(74);
    // }
    // // fseek(rp, 0L, SEEK_END);
    // // size_t file_size = ftell(rp);
    // // rewind(rp);
    // // Instr *buffer = (Instr*)malloc(sizeof(Instr));
    // // if( == NULL){
    // //     fprintf(stderr, "Not enough memory");
    // //     exit(74);
    // // }
    // size_t bytes_read = fread(vm->program, sizeof(vm->program[0]), 4,rp);
    // if(bytes_read < 4){
    //     fprintf(stderr, "Could not read file\n");
    //     exit(74);
    // }
    // ERAVM *vm = init_vm();
    // int len = sizeof prog / sizeof prog[0];
    // for(int i = 0; i < len; i++){
    //     writeVal(vm, prog[i]);
    //     // vm->program_size +=1;
    // }
    load_file(vm, "assembler/test1.ebin");
    run(vm);

    // FILE *fp = fopen("test.ebin", "wb");

    // // size_t element_size = sizeof *program;
    // size_t element_size = sizeof vm->program[0];
    // size_t elements_to_write = vm->program_size;

    // //write to file
    // size_t written = fwrite(vm->program, element_size, elements_to_write, fp);

    // if(written != elements_to_write){
    //     fprintf(stderr, "failed wrote only %lu out of %lu elements.\n", 
    //     written, elements_to_write);
    //     exit(EXIT_FAILURE);
    // }
    // fclose(rp); 

    // const char *path = "test.ebin";
    // FILE *rp = fopen(path, "rb");
    // if(rp == NULL){
    //     fprintf(stderr, "Could not open file");
    //     exit(74);
    // }
    // fseek(rp, 0L, SEEK_END);
    // size_t file_size = ftell(rp);
    // rewind(rp);
    // // char *buffer = (char*)malloc(file_size  + 1);
    // // if( == NULL){
    // //     fprintf(stderr, "Not enough memory");
    // //     exit(74);
    // // }
    // size_t bytes_read = fread(buffer, sizeof(char), file_size,rp);
    // if(bytes_read < file_size){
    //     fprintf(stderr, "Could not read file");
    //     exit(74);
    // }
    // buffer[bytes_read] = '\0';
   
   
    
    // Dump stack
    printf("-------STACK-------\n");
    printf("  u32\tf32\tptr\n\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\t%.2f\t%p\n",i,vm->stack[i].UINT, vm->stack[i].FLOAT, vm->stack[i].ptr);
    }
    printf("stack top: %d\n", vm->stack[vm->sp-1].UINT);
    printf("program size: %d\n\n", vm->program_size);

    puts(_RED"-------MEM-------\n"_RESET);
    // printf("-------MEM-------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%03d:\t%02x\n",i,vm->mem[i]);
    }
    
    free_vm(vm);
     
    return 0;
}