CFLAGS = -o
CC = gcc -std=c99 ${CFLAGS}
LEX = flex
YACC = bison

era.easm: asm.l easm.y 
	${LEX} asm.l
	${YACC} -d -y -v easm.y
	${CC} era_asm y.tab.c lex.yy.c