%{
#include "easm.h"

Instr instr[PROG_SIZE];
u32 count = 0;
%}
%union{
    int ival;
    float fval;
};

%token <ival> INTEGER 
%token <fval> _FLOAT

%token PUSH ADD MINUS MULTI DIVIDE BIT_OR BIT_AND JUMP JUMPF HALT POP CMP LOOP


%%
file:
        file instruction
        |
        ;   

instruction:
         push_stmt             
         |  POP                 {INSTR(OP_POP); count++; }
         |  ADD                {INSTR(OP_ADD); count++; }
         |  HALT               {INSTR(OP_HALT); count++; }
         |  MULTI              {INSTR(OP_MULTI); count++; }
         |  DIVIDE             {INSTR(OP_DIVIDE); count++; }
         |  BIT_AND            {INSTR(OP_BIT_AND); count++; }
         |  BIT_OR             {INSTR(OP_BIT_OR); count++; }
         |  JUMP INTEGER       {INSTR(OP_JUMP); _uint($2); count++; }
         |  JUMPF INTEGER      {INSTR(OP_JUMPF); _uint($2); count++; }
         |  LOOP                {INSTR(OP_LOOP); count++; }
         |  CMP                {INSTR(OP_CMP); count++; }
         ;

push_stmt:
    PUSH INTEGER       {INSTR(OP_PUSH); _uint($2); count++; } 
    | PUSH _FLOAT      {INSTR(OP_PUSH); _float($2); count++; }   
    ;
%%

void yyerror(char *s){
    // fprintf(stdout, "%s\n", s);
    printf("Parse Error! Message: %s\n", s);
    exit(-1);
}

void prog_dump(Instr instr[]){
    // Instr ins[20];
    // FILE *rp = fopen("test1.txt", "rb");
    // if(rp == NULL){
    //     fprintf(stderr, "Could not open file");
    //     exit(74);
    // }
    // fseek(rp, 0L, SEEK_END);
    // size_t file_size = ftell(rp);
    // rewind(rp);
    // int prog_size = fread(ins, sizeof(ins[0]), file_size, rp);
    // if(prog_size < file_size){
    //     fprintf(stderr, "Could not read file\n");
    //     exit(74);
    // }
    printf("OP      UINT      FLOAT\n");
    printf("------------------------\n");
    for(int i = 0; i < count; i++){
        printf("OP=%d, UINT=%d, FLOAT=%f\n", 
                instr[i].type, instr[i].operand.UINT, instr[i].operand.FLOAT
              );
    }
}
void era_bin(Instr program[]){
    // yyout = fopen("test.ebin", "wb");
    yyout = fopen("test1.ebin", "wb");


    // size_t element_size = sizeof *program;
    size_t element_size = sizeof program[0];
    size_t elements_to_write = count;
    size_t prog_count = fwrite(&count, sizeof(u32), 1, yyout);
    //write to file
    size_t written = fwrite(program, element_size, elements_to_write, yyout);

    if(written != elements_to_write){
        fprintf(stderr, "failed wrote only %lu out of %lu elements.\n", 
        written, elements_to_write);
        exit(EXIT_FAILURE);
    }
}
int main(int argc, char *args[]){
    ++args; --argc;
    yyin = fopen(args[0], "r");
    // disassemble(instr);
    yyparse();
    prog_dump(instr);
    era_bin(instr);
    return 0;
}
