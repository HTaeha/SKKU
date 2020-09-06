%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "taeha.tab.h"
#include "AST.h"
%}

word [a-zA-Z]*
num [0-9]*

space [ \t\r\n]
type INT|CHAR
special [;|\,|\(|\)|\{|\}|\<|\>|\=|\+|\*]

%%

{special} {
            return yytext[0];
}

{space} {
            break;
}

{type} {
    if(!strcmp(yytext, "INT")){
        yylval.type = 1;
    }else{
        yylval.type = 0;
    }
    return TYPE;
}

{word} {
    if(!strcmp(yytext, "WHILE")){
        return WHILE;
    }else if(!strcmp(yytext, "IF")){
        return IF;
    }else if(!strcmp(yytext, "THEN")){
        return THEN;
    }else if(!strcmp(yytext, "ELSE")){
        return ELSE;
    }else if(!strcmp(yytext, "RETURN")){
        return RETURN;
    }else{
        char *tmpc = (char *)malloc(strlen(yytext));
        strcpy(tmpc, yytext);
        yylval.word = tmpc;
        return WORD;
    }
}

{num} {
    yylval.num = atoi(yytext);
    return NUM;
}
%%
