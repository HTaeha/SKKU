#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int * operMul(char *, char *);
int main(void){
    char operand = ' ';
    char * num1 = (char *)malloc(sizeof(char)*100);
    char * num2 = (char *)malloc(sizeof(char)*100);
    int * mul_result = (int *)malloc(sizeof(int)*201);

    int result = 0;
    scanf("%c",&operand); scanf("%s",num1); scanf("%s", num2);

    if(operand == '*'){
        mul_result = operMul(num1, num2);
        for(int i = 0 ;i<201;i++){
            if(mul_result[i] == -1){
                printf("\n");
                break;
            }
            printf("%d", mul_result[i]);
        }
    }

    return 0;
}
int * operMul(char * num1, char * num2){
    char * str1 = (char*)malloc(sizeof(char)*100);
    char * str2 = (char*)malloc(sizeof(char)*100);
    int * result = (int*)malloc(sizeof(int)*201);
    int * result1 = (int*)malloc(sizeof(int)*201);

    int len1 = 0, len2 = 0, carry = 0, digit = 0, remainder = 0, temp=0;
    int resultLen = 0;
    len1 = strlen(num1); len2 = strlen(num2);

    memset(result, 0, 201);

    for(int i = 0; i < len1; i++){
        str1[i] = num1[len1-i-1];
    }
    for(int i = 0; i < len2; i++){
        str2[i] = num2[len2-i-1];
    }

    for(int i = 0; i < len2; i++){
        for(int j = 0; j < len1; j++){
            digit = (str2[i] - '0') * (str1[j] - '0');
        //    printf("i : %d, j : %d\n", i, j);
          //  printf("digit : %d * %d = %d\n", str2[i]-'0', str1[j]-'0',digit);
            carry = digit/10;
            remainder = digit%10;
            temp = result[i+j] + remainder;
            result[i+j] = temp%10;
            carry += temp/10;
    //        printf("carry : %d\n", carry);
      //      printf("%d\n", result[i+j+1]);
            result[i+j+1] = result[i+j+1] + carry;
        }
    }
            
    for(int i = 200; i>=0;i--){
        if (result[i] != 0 || i == 0){
            resultLen = i+1;
            break;
        }
    }
    for(int i = 0; i<resultLen;i++){//실제 order로 바꿔주는 과정.
        result1[i] = result[resultLen-i-1];
    }
    
    result1[resultLen] = -1;

    return result1;
}

