#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "print.h"

FILE *fp;
FILE *symtab;
int indent;
char stack_func[100][100];
int stack_table[100][4]; // while for if order
int index_;
char isWritten;
char type;
int count;
int f_ptr = 0;
int flag = 0;

int main () {
	fp = fopen("testfile.code", "w");
	symtab = fopen("testfile.symbol", "w");
	indent = 0;
	stack_init();
	isWritten = 0;
	count = 1;

	if(!yyparse())
		Program();
	
	close(fp);
    close(symtab);

	return 0;
}

void Program () {
    int i;
    struct WORD_S *words;

	if (head == NULL) {
		fprintf(stderr, "program does not exist.\n");
		return;
	}

    stack_init();

    if(head->t == eInt){
        type = 0;
        fprintf(fp, "INT ");
    }else{
        type = 1;
        fprintf(fp, "CHAR ");
    }

    push(head->word);

    if(isWritten == 0){
        isWritten = 1;
        fprintf(symtab, "\n");
        fprintf(symtab, "Function name : %s\n", stack_func[0]);
        fprintf(fp, "%s",stack_func[0]);
		fprintf(symtab, "\tcount\t\ttype\t\tname\t\trole\n");
        words = head->word_s;
        fprintf(fp, "(");
        flag = 1;
        while(words){
            fprintf(symtab, "\t%d",count++);
            fprintf(symtab, "\t\t\tnone");
            fprintf(symtab, "\t\t%s",words->word);
            fprintf(symtab, "\t\t\tparameter\n");
            words = words->prev;
        }
        Words(head->word_s);
        flag = 0;
        fprintf(fp, ")");
    }
	if(head->block) {
		Block(head->block);
		fprintf(symtab, "\n");
	}
}

void Declaration (struct DECLARATION *decl) {

	int i = 0;

	if(decl->prev){
		Declaration(decl->prev);
    }
	if(isWritten == 0) {
		isWritten = 1;
		fprintf(symtab, "\n");
		fprintf(symtab, "Function name : %s", stack_func[0]);

		for(i = 1; i <= index_; i++) {
			fprintf(symtab, " - %s(", stack_func[i]);
			if(!strcmp(stack_func[i], "WHILE"))
				fprintf(symtab, "%d)", stack_table[i][0]);
			else if(!strcmp(stack_func[i], "IF"))
				fprintf(symtab, "%d)", stack_table[i][1]);
			else if(!strcmp(stack_func[i], "block"))
				fprintf(symtab, "%d)", stack_table[i][2]);
		}
		fprintf(symtab, "\n");

		fprintf(symtab, "\tcount\t\ttype\t\tname\t\trole\n");
	}

	if(decl->t == eInt){
		type = 0;
		fprintf(fp, "INT ");
    }else if(decl->t == eChar){
        type = 1;
		fprintf(fp, "CHAR ");
    }

    Words(decl->word_s);
    fprintf(fp, ";\n");

	Indent();
}

void Block (struct BLOCK *block) {

	fprintf(fp, "{\n");

	indent++;
        Indent();
	if(block->decl){
		Declaration(block->decl);
        }
	fprintf(fp, "\n");
	if(block->stat){
            Stat(block->stat);
        }
	indent--;
	Indent();
	fprintf(fp, "}\n");
}

void Stat (struct STAT *stat) {
	if(stat->prev)
		Stat(stat->prev);
	switch(stat->s) {
		case eAssign:
			Indent();
			Assign(stat->stat.assign_);
			break;
		case eRet:
			Indent();
			Return(stat->stat.return_);
			break;
		case eWhile:
			if(f_ptr > 0)
				f_ptr--;
			Indent();
			While(stat->stat.while_);
			break;
		case eIf:
			if(f_ptr > 0)
				f_ptr--;
			Indent();
			If(stat->stat.if_);
			break;
		case eBlock:
			f_ptr++;
			if(f_ptr >= 2) {
				push("block");
			}
			Indent();
			Block(stat->stat.block_);
			if(f_ptr >= 2) {
				pop();
				f_ptr--;
			}
			break;
		case eSemi:
			fprintf(fp, ";\n");
			break;
	}
}

void Assign (struct ASSIGN *assi_) {

	fprintf(fp, "%s", assi_->word);

	fprintf(fp, " = ");

	Expre(assi_->expre);
}

void Return (struct RETURN *ret_) {

	fprintf(fp, "RETURN");

	if(ret_->expre) {
		fprintf(fp, " ");
		Expre(ret_->expre);
	}

	return;
}

void While (struct WHILE_S *while_s){

	push("WHILE");

    fprintf(fp, "WHILE (");
    Expre(while_s->cond);
    fprintf(fp, ")\n");
    Stat(while_s->stat);

	pop();
}

void If (struct IF_S *if_s) {

	push("IF");

	if (if_s->else_) {
		fprintf(fp, "IF (");
		Expre(if_s->cond);
		fprintf(fp, ") THEN\n");
		Stat(if_s->if_);
		Indent();
		fprintf(fp, "ELSE\n");
		Stat(if_s->else_);
	}
	else {
		fprintf(fp, "IF (");
		Expre(if_s->cond);
		fprintf(fp, ") THEN\n");
		Stat(if_s->if_);
	}

	pop();
}

void Expre (struct EXPRE *expre) {

	switch(expre->e) {
		case eExpr:
			Expr(expre->expression.expr_);
			break;
		case eTerm:
			Term(expre->expression.term_);
			break;
		case eCond:
			Cond(expre->expression.cond_);
			break;
		case eNum:
			fprintf(fp, "%d", expre->expression.num);
			break;
		case eWord:
			fprintf(fp, "%s", expre->expression.word);
			break;
		case eExpre:
			fprintf(fp, "(");
			Expre(expre->expression.bracket);
			fprintf(fp, ")");
			break;
	}
}

void Term (struct TERM *term) {

	Expre(term->lhs);

	if(term->m == eMult)
		fprintf(fp, " * ");

	Expre(term->rhs);
}

void Cond (struct COND *cond) {

	Expre(cond->lhs);

	if(cond->r == eLT) {
		fprintf(fp, " < ");
	}
	else if(cond->r == eGT) {
		fprintf(fp, " > ");
	}

	Expre(cond->rhs);
}

void Expr (struct EXPR *expr) {

	if(expr->lhs) {
		Expre(expr->lhs);
	}

	if(expr->a == ePlus)
		fprintf(fp, " + ");

	Expre(expr->rhs);

	return;
}

void Words(struct WORD_S *word){
    if(word->prev){
        Words(word->prev);
    }
    if(!flag){
        fprintf(symtab, "\t%d",count++);
        if(type){
            fprintf(symtab, "\t\t\tCHAR");
        }else{
            fprintf(symtab, "\t\t\tINT\t");
        }
        fprintf(symtab, "\t\t%s", word->word);
        fprintf(symtab, "\t\t\tvariable\n");
    }
   // fprintf(fp, ";\n");
    if(word->prev){
        fprintf(fp, ", %s", word->word);
    }else{
        fprintf(fp, "%s", word->word);
    }

}

void Indent () {
	int i;
	for(i = 0; i < indent; i++)
		fprintf(fp, "\t");
}

void stack_init () {
	int i,j;

	for(i = 0; i < 100; i++) {
		for(j = 0; j < 100; j++)
			stack_func[i][j] = 0;
	}

	for(i = 0; i < 100; i++) {
		for(j = 0; j < 4; j++) {
			stack_table[i][j] = 0;
		}
	}

	index_ = -1;
	isWritten = 0;
	count = 1;
}

void pop() {

	int i;

	for (i = 0; i < 100; i++)
		stack_func[index_][i] = 0;

	index_--;

	isWritten = 0;
}

void push(char *name) {

	index_++;
	count = 1;

	strcpy(stack_func[index_], name);

	isWritten = 0;

	if(!strcmp(name, "WHILE"))
		stack_table[index_][0]++;
	else if(!strcmp(name, "IF"))
		stack_table[index_][1]++;
	else if(!strcmp(name, "block"))
		stack_table[index_][2]++;
}
