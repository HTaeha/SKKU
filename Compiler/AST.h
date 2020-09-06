#ifndef AST_H
#define AST_H

#define bool char 
#define true 1 
#define false 0

typedef enum
{eInt,eChar} Type_e;
typedef enum
{eAssign,eRet,eWhile,eIf,eBlock,eSemi} Stat_e;
typedef enum
{eExpr,eTerm,eCond,eNum,eWord,eExpre} Expre_e;
typedef enum
{ePlus} Expr_e;
typedef enum
{eMult} Term_e;
typedef enum
{eLT,eGT} Cond_e;

struct PROGRAM
{
    Type_e t;
    char * word;
    struct WORD_S * word_s;
    struct BLOCK * block;
};

struct DECLARATION
{
	Type_e t;
    struct WORD_S * word_s;
	struct DECLARATION *prev;
};

struct WORD_S
{
    char* word;
    struct WORD_S * prev;
};

struct BLOCK
{
	struct DECLARATION *decl;
	struct STAT *stat;
};

struct STAT 
{	
	Stat_e s;
	union {
		struct ASSIGN *assign_;
		struct RETURN *return_;
		struct WHILE_S *while_;
		struct IF_S *if_;
		struct BLOCK *block_;
	} stat; 
	struct STAT *prev;
};

struct ASSIGN
{
	char *word;
	struct EXPRE *expre;
};

struct RETURN
{
	struct EXPRE *expre;
};

struct WHILE_S 
{
	struct EXPRE *cond;
	struct STAT *stat;

};

struct IF_S
{
	struct EXPRE *cond;
	struct STAT *if_;
	struct STAT *else_;
};

struct EXPRE
{
	Expre_e e;
	union
	{
		int num;
        char* word;
		struct EXPR *expr_;
		struct TERM *term_;
		struct COND *cond_;
		struct EXPRE *bracket;
	} expression;
};

struct EXPR
{
	Expr_e a;
	struct EXPRE *lhs;
	struct EXPRE *rhs;
};

struct TERM
{
	Term_e m;
	struct EXPRE *lhs;
	struct EXPRE *rhs;
};

struct COND
{
	Cond_e r;
	struct EXPRE *lhs;
	struct EXPRE *rhs;
};

#endif
