%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "print.h"
%}

%union {
    struct PROGRAM *ptr_program;
	struct DECLARATION *ptr_decl;
    struct WORD_S *ptr_word;
	struct BLOCK *ptr_block;
	struct STAT *ptr_stat;
	struct ASSIGN *ptr_assi;
	struct RETURN *ptr_ret;
	struct WHILE_S *ptr_while;
	struct IF_S *ptr_if;
	struct EXPRE *ptr_expre;
	struct EXPR *ptr_expr;
	struct TERM *ptr_term;
	struct COND *ptr_cond;
	int num;
	char* word;
	int type;
}

%token <num>NUM <word>WORD <type>TYPE <ptr_while>WHILE <ptr_if>IF <ptr_if>THEN <ptr_if>ELSE <ptr_ret>RETURN
%right '='
%left '+'
%left '*'
%left '(' ')'

%type <ptr_program> Program;
%type <ptr_decl> Decllist;
%type <ptr_decl> Decl;
%type <ptr_word> Words;
%type <ptr_word> Word;
%type <ptr_block> Block;
%type <ptr_stat> Statlist;
%type <ptr_stat> Stat;
%type <ptr_assi> Assi;
%type <ptr_while> While;
%type <ptr_if> If;
%type <ptr_expre> Expre;
%type <ptr_expr> Expr;
%type <ptr_term> Term;
%type <ptr_cond> Cond;
%type <ptr_ret> Ret;

%%
Program :
	TYPE WORD '(' Words ')' Block {
		struct PROGRAM *program = (struct PROGRAM *) malloc (sizeof (struct PROGRAM));
        program->word = $2;
		program->word_s = $4;
        program->block = $6;
		head = program;
        if($1){
            program->t = eInt;
        }else{
            program->t = eChar;
        }
		$$ = program;
	}
	;

Decllist :
    Decllist Decl {
		struct DECLARATION *decl = $2;
		decl->prev = $1;
		$$ = decl;
	}
	| Decl {
		struct DECLARATION *decl = $1;
		decl->prev = NULL;
		$$ = decl;
	}
	;

Decl :
	TYPE Words ';' {
		struct DECLARATION *decl = (struct DECLARATION *) malloc (sizeof (struct DECLARATION));
        decl->word_s = $2;
		if($1)
			decl->t = eInt;
		else
			decl->t = eChar;
		$$ = decl;
	}
	;

Block :
	'{' Decllist Statlist '}' {
		struct BLOCK *block = (struct BLOCK *) malloc (sizeof (struct BLOCK));
		block->decl = $2;
		block->stat = $3;
		$$ = block;
	}
	;

Statlist :
	Statlist Stat {
		struct STAT *stat = $2;
		stat->prev = $1;
		$$ = stat;
	}
	| Stat {
		struct STAT *stat = $1;
		stat->prev = NULL;
		$$ = stat;
	}
	;

Stat :
	Assi {
		struct STAT *stat = (struct STAT *) malloc (sizeof (struct STAT));
		stat->s = eAssign;
		stat->stat.assign_ = $1;
		$$ = stat;
	}
    	| Ret {
		struct STAT *stat = (struct STAT *) malloc (sizeof (struct STAT));
		stat->s = eRet;
		stat->stat.return_ = $1;
		$$ = stat;
	}
	| While {
		struct STAT *stat = (struct STAT *) malloc (sizeof (struct STAT));
		stat->s = eWhile;
		stat->stat.while_ = $1;
		$$ = stat;
	}
	| If {
		struct STAT *stat = (struct STAT *) malloc (sizeof (struct STAT));
		stat->s = eIf;
		stat->stat.if_ = $1;
		$$ = stat;
	}
	| Block {
		struct STAT *stat = (struct STAT *) malloc (sizeof (struct STAT));
		stat->s = eBlock;
		stat->stat.block_ = $1;
		$$ = stat;
	}
	| ';' {
		struct STAT *stat = (struct STAT *) malloc (sizeof (struct STAT));
		stat->s = eSemi;
		$$ = stat;
	}
	;

Assi :
	WORD '=' Expre {
		struct ASSIGN *assi = (struct ASSIGN *) malloc (sizeof (struct ASSIGN));
		assi->word = $1;
		assi->expre = $3;
		$$ = assi;
	}
	;

While :
	WHILE '(' Expre ')' Stat {
		struct WHILE_S *while_s = (struct WHILE_S *) malloc (sizeof (struct WHILE_S));
		while_s->cond = $3;
		while_s->stat = $5;
		$$ = while_s;
	}
	;

If :
	IF '(' Expre ')' THEN Stat ELSE Stat {
		struct IF_S *if_s = (struct IF_S *) malloc (sizeof (struct IF_S));
		if_s->cond = $3;
		if_s->if_ = $6;
		if_s->else_ = $8;
		$$ = if_s;
	}
	| IF '(' Expre ')' THEN Stat {
		struct IF_S *if_s = (struct IF_S *) malloc (sizeof (struct IF_S));
		if_s->cond = $3;
		if_s->if_ = $6;
		if_s->else_ = NULL;
		$$ = if_s;
	}
	;

Expre :
	Expr {
		struct EXPRE *expre = (struct EXPRE *) malloc (sizeof (struct EXPRE));
		expre->e = eExpr;
		expre->expression.expr_ = $1;
		$$ = expre;
	}
	| Term {
		struct EXPRE *expre = (struct EXPRE *) malloc (sizeof (struct EXPRE));
		expre->e = eTerm;
		expre->expression.term_ = $1;
		$$ = expre;
	}
	| Cond {
		struct EXPRE *expre = (struct EXPRE *) malloc (sizeof (struct EXPRE));
		expre->e = eCond;
		expre->expression.cond_ = $1;
		$$ = expre;
	}
	| NUM {
		struct EXPRE *expre = (struct EXPRE *) malloc (sizeof (struct EXPRE));
		expre->e = eNum;
		expre->expression.num = $1;
		$$ = expre;
	}
	| WORD {
		struct EXPRE *expre = (struct EXPRE *) malloc (sizeof (struct EXPRE));
		expre->e = eWord;
		expre->expression.word = $1;
		$$ = expre;
	}
	| '(' Expre ')' {
		struct EXPRE *expre = (struct EXPRE *) malloc (sizeof (struct EXPRE));
		expre->e = eExpre;
		expre->expression.bracket = $2;
		$$ = expre;
	}
	;

Words : 
    Words ',' Word {
        struct WORD_S *word = $3;
        word->prev = $1;
        $$ = word;
    }
    | Word {
        struct WORD_S *word = $1;
        word->prev = NULL;
        $$ = word;
    }
    ;

Word :
    WORD {
        struct WORD_S * word = (struct WORD_S *) malloc (sizeof (struct WORD_S));
        word->word = $1;
        $$ = word;
    }
    ;

Expr :
	Expre '+' Expre {
		struct EXPR *expr = (struct EXPR *) malloc (sizeof (struct EXPR));
		expr->a = ePlus;
		expr->lhs = $1;
		expr->rhs = $3;
		$$ = expr;
	}
	;

Term :
	Expre '*' Expre {
		struct TERM *term = (struct TERM *) malloc (sizeof (struct TERM));
		term->m = eMult;
		term->lhs = $1;
		term->rhs = $3;
		$$ = term;
	}
	;

Cond :
	Expre '<' Expre {
		struct COND *cond = (struct COND *) malloc (sizeof (struct COND));
		cond->r = eLT;
		cond->lhs = $1;
		cond->rhs = $3;
		$$ = cond;
	}
	| Expre '>' Expre {
		struct COND *cond = (struct COND *) malloc (sizeof (struct COND));
		cond->r = eGT;
		cond->lhs = $1;
		cond->rhs = $3;
		$$ = cond;
	}
	;

Ret :
	RETURN Expre {
		struct RETURN *ret = (struct RETURN *) malloc (sizeof (struct RETURN));
		ret->expre = $2;
		$$ = ret;
	}
	;

%%

int yyerror (char *s) {
    return fprintf (stderr, "%s\n", s);
}
