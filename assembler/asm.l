%{
#include "y.tab.h"
int yylex();
void yyerror(char *);
%}

%%

[-+]?[0-9]+           {   
                        yylval.ival = atoi(yytext); 
                        return INTEGER; 
                      }
[-+]?[0-9]*[.][0-9]+  {
                        yylval.fval = atof(yytext);
                        return _FLOAT;
                      }

push                {return PUSH;}
pop                 {return POP;}
less                 {return LESS;}
iadd                 {return IADD;}
iminus               {return IMINUS;}
imulti               {return IMULTI;}
idiv                 {return IDIVIDE;}
fadd                 {return FADD;}
fminus               {return FMINUS;}
fmulti               {return FMULTI;}
fdiv                 {return FDIVIDE;}
_and                {return BIT_AND;}
_or                 {return BIT_OR;}
load                {return LOAD;}
jump                {return JUMP;}
jumpf               {return JUMPF;}
args                {return ARGS;}
call                {return CALL;}
ret                 {return RET;}
halt                {return HALT;}
[ \n\t]+            {;}
(\/\/)[^\n]*        {;}
.                   {yyerror("Unknown character");}
%%
int yywrap(){
    return 1;
}