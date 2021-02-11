#ifndef era_parse_asm_h
#define era_parse_asm_h

#include <stdio.h>
#include <stdlib.h>
#include "../era.h"

int yylex();
int yyparse();
void yyerror(char *);
void prog_dump(Instr instr[]);
extern FILE *yyin, *yyout;

extern Instr instr[PROG_SIZE];
extern u32 count;

#define INSTR(value)\
    instr[count].type = value;\

#define _uint(value)\
    instr[count].operand.UINT = value;\

#define _float(value)\
    instr[count].operand.FLOAT = value;\

#endif
