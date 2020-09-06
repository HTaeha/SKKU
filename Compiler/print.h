#ifndef PRINT_H
#define PRINT_H

#include "AST.h"

struct PROGRAM *head;

void Program ();
void Declaration (struct DECLARATION *);
void Block (struct BLOCK *);
void Stat (struct STAT *);
void Assign (struct ASSIGN *);
void Return (struct RETURN *);
void While (struct WHILE_S *);
void If (struct IF_S *);
void Expre (struct EXPRE *);
void Expr (struct EXPR *);
void Cond (struct COND *);
void Term (struct TERM *);
void Words (struct WORD_S *);
void Indent ();
void stack_init ();
void pop();
void push(char *);

#endif
