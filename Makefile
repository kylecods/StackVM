CFLAGS = -o
CC = gcc -std=c99 ${CFLAGS}
LEX = flex
YACC = bison

# easm: assembler/asm.l assembler/easm.y 
# 	${LEX} assembler/asm.l
# 	${YACC} -d -y -v assembler/easm.y
# 	${CC} era_asm y.tab.c lex.yy.c

all:
	gcc -o eravm main.c vm/vm.c