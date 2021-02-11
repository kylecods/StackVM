#ifndef era_scanner_h
#define era_scanner_h

typedef enum tokenType{ //tag


}TokenType;

typedef struct ttoken{ //tag
    TokenType type;
    char *begin;
    int length;
    int line;//debug
}Token;

typedef struct lexer{ //tag
    char *begin;
    char *end;
    int line;//debug
}Lexer;



#endif