#include <stdio.h>
#include <stdlib.h>

#ifndef _STACK_H
#define _STACK_H

#define MAX_STACK	50

typedef enum{ false, true} bool;
typedef int Data;   //I will push decimal numbers in stack.

typedef struct {
	Data items[MAX_STACK];
	int top;
} Stack;

// Make stack empty.
void InitStack(Stack *pstack);

// Check whether stack is full.
bool IsFull(Stack *pstack);

// Check whether stack is empty
bool IsEmpty(Stack *pstack);

// Read the item at the top.
Data Peek(Stack *pstack);

// Insert an item at the top.
void Push(Stack *pstack, Data item);

// Remove the item at the top.
void Pop(Stack *pstack);

#endif

int truncation(int num);
void Calc(Stack *pstack, char operator);
int main(){
    char input_c;     //Save a character type input.
    int input_i;      //Save a integer type input.
    int input = 0;    //Save a decimal number changed four-digit octal number.
    int octal[4];     //Save four-digit octal numbers.
    int i,j;
    int result;
    Stack operand;    //Save operands.

    InitStack(&operand);

    while((input_c = getchar()) != '\n'){ //If input is '\n', stop.
        input = 0;
        if(input_c <='7' && '0'<=input_c){ //If input is a number, change it a decimal number. Then push it in the stack. 
            input_i = input_c-'0';
            for(i=0;i<4;i++){
                for(j=0;j<3-i;j++){
                    input_i *= 8;
                }
                input += input_i;
                input_c = getchar();
                input_i = input_c-'0';
            }
            Push(&operand, input);
        }else if(input_c == ' '){
            continue;
        }else{ //If input is a operator, execute a Calc function.
            Calc(&operand, input_c);
        }
    }

    result = Peek(&operand);
    for(i=0;i<4;i++){ //Change a decimal number to a four-digit octal number.
        octal[3-i] = result%8;
        result /= 8;
    }
    for(i=0;i<4;i++){ //Print a result value in a four-digit octal number form.
        printf("%d",octal[i]);
    }

    return 0;
}
void Calc(Stack *pstack, char operator){ //ope2 is a number top of the stack, ope1 is a number under ope2.
    int ope1;       //front operand.
    int ope2;       //rear operand.
    int result = 0;

    if(operator == '+'){   //Calculate '+' operation.
        ope2 = Peek(pstack);
        Pop(pstack);
        ope1 = Peek(pstack);
        Pop(pstack);
        result = ope1 + ope2;
        if(result >= 8*8*8*8){ //If the result is a 8*8*8*8 or more, 
            result -= 8*8*8*8; //it should be  truncated.
        }                      //But it cannot jump over a 2*8*8*8*8.
        Push(pstack, result);
    }else if(operator == '-'){ //Calculate '-' operation.
        ope2 = Peek(pstack);
        Pop(pstack);
        ope1 = Peek(pstack);
        Pop(pstack);
        result = ope1 - ope2;
        Push(pstack, result);
    }else if(operator == '*'){ //Calculate '*' operation.
        ope2 = Peek(pstack);
        Pop(pstack);
        ope1 = Peek(pstack);
        Pop(pstack);
        result = ope1 * ope2;
        Push(pstack, truncation(result)); //It can jump over a 2*8*8*8*8. So I use a truncation function.
    }else if(operator == '/'){ //Calculate '/' operation.
        ope2 = Peek(pstack);
        Pop(pstack);
        ope1 = Peek(pstack);
        Pop(pstack);
        result = ope1 / ope2;
        Push(pstack, result);
    }else if(operator == '%'){ //Calculate '%' operation.
        ope2 = Peek(pstack);
        Pop(pstack);
        ope1 = Peek(pstack);
        Pop(pstack);
        result = ope1 % ope2;
        Push(pstack, result);
    }
}
int truncation(int num){ //Truncate a 8*8*8*8 or more number.
    int oct[4];
    int i,j;
    int result = 0;

    for(i=0;i<4;i++){ //Save a four-digit octal number in array of oct.
        oct[3-i] = num%8;
        num /=8;
    }
    for(i=0;i<4;i++){ //Save a decimal number changed a octal number.
        for(j=0;j<i;j++){
            oct[3-i] *= 8;
        }
        result += oct[3-i];
    }
    return result;
}
// Make stack empty.
void InitStack(Stack *pstack)
{
	pstack->top = -1;
}

// Check whether stack is full.
bool IsFull(Stack *pstack)
{
	return pstack->top == MAX_STACK - 1;
}

// Check whether stack is empty
bool IsEmpty(Stack *pstack)
{
	return pstack->top == -1;
}

// Read the item at the top.
Data Peek(Stack *pstack)
{
	if (IsEmpty(pstack))
		exit(1); //error: empty stack
	return pstack->items[pstack->top];
}

// Insert an item at the top.
void Push(Stack *pstack, Data item)
{
	if (IsFull(pstack))
		exit(1); //error: stack full
	pstack->items[++(pstack->top)] = item;
}


// Remove the item at the top.
void Pop(Stack *pstack)
{
	if (IsEmpty(pstack))
		exit(1); //error: empty stack
	--(pstack->top);
}
