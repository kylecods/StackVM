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
cmp                 {return CMP;}
add                 {return ADD;}
multi               {return MULTI;}
div                 {return DIVIDE;}
_and                {return BIT_AND;}
_or                 {return BIT_OR;}
jump                {return JUMP;}
jumpf               {return JUMPF;}
loop                {return LOOP;}
halt                {return HALT;}
[ \n\t]+            {;}
(\/\/)[^\n]*        {;}
.                   {yyerror("Unknown character");}
%%
int yywrap(){
    return 1;
}