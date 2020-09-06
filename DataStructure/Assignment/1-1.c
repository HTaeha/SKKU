#include <stdio.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_LEN 201
void init(int n, int * arr, int len);
void set_array(char * tmp, int * arr, int len);
int mul(int * n1, int * n2, int * res, int len1, int len2);
int sum(int * n1, int * n2, int * res, int len1, int len2);
void reverse(int * arr, int * res, int len);
int main() {
    char operand;
    int num1[MAX_INPUT_LEN];
    int num2[MAX_INPUT_LEN];
    int len1;
    int len2;
    char temp[MAX_INPUT_LEN];
    int idx;
    int i;
    int result[MAX_LEN];
    int result_len;
    int zero_flag = 0;

    init(-1, num1, MAX_INPUT_LEN);
    init(-1, num2, MAX_INPUT_LEN);
    init(0, result, MAX_LEN);

    scanf("%c",&operand);
    getchar();
    scanf("%s",temp);
    len1 = strlen(temp);
    set_array(temp, num1, len1);
    scanf("%s",temp);
    len2 = strlen(temp);
    set_array(temp, num2, len2);

    if(operand == '*'){
        if(num1[0] == 0 || num2[0] == 0){
            printf("0\n");
            return 0;
        }
        result_len = mul(num1, num2, result, len1, len2);
    }else if(operand == '+'){
        if(num1[0] == 0 && num2[0] == 0){
            printf("0\n");
            return 0;
        }
        result_len = sum(num1, num2, result, len1, len2);
    }
    
    for(i=0;i<result_len;i++){
        if(result[i] == 0 && zero_flag == 0){
            continue;
        }
        if(result[i] != 0){
            zero_flag = 1;
        }
        printf("%d", result[i]);
    }
    printf("\n");
    return 0;
}
void init(int n,int * arr, int len){
    int i;
    for(i = 0;i<len;i++){
        arr[i] = n;
    }
}
void set_array(char * tmp, int * arr, int len){
    int i;
    for(i=0;i<len;i++){
        arr[i] = tmp[i] - '0';
    }
}
int mul(int * n1, int * n2, int * res, int len1, int len2){
    int i,j;
    int num;
    int n = 0;
    int temp[MAX_LEN];
    int temp2[MAX_LEN];
    int res_temp[MAX_LEN];
    int temp_len, temp_len2, res_len = 0;
    int real_res_len = 0;

    init(-1, temp, MAX_LEN);
    for(i=0;i<len1;i++){
        temp[i] = n1[i];
    }
    temp_len = len1;
    while(1){
        if(len2-n-1 == -1){
            return res_len;
        }
        num = n2[len2-n-1];
        /*for(i=0;i<n;i++){
            n1[len1] = 0;
            len1++;
        }*/

        init(-1, temp2, MAX_LEN);
        temp2[0] = 0;
        temp_len2 = 1;
       
        for(i=0;i<num;i++){
            init(0, res_temp, MAX_LEN);
            res_len = sum(temp, temp2, res_temp, temp_len, temp_len2);
            //res_len = sum(n1, temp, temp2, len1, res_len);
            temp_len2 = res_len;
            for(j=0;j<res_len;j++){
                temp2[j] = res_temp[j];
            }
        }
        printf("res_len : %d\n", res_len);
        printf("temp2 : ");
        for(i=0;i<res_len;i++){
            printf("%d", temp2[i]);
        }
        printf("\n");
        for(i=res_len;i<MAX_LEN;i++){
            res_temp[i] = -1;
        }
        printf("res_temp : ");
        for(i=0;i<res_len;i++){
            printf("%d", res_temp[i]);
        }
        printf("\n");

        for(i=0;i<MAX_LEN;i++){
            if(i<real_res_len){
                temp2[i] = res[i];
            }else{
                temp2[i] = -1;
            }
        }
        init(0, res, MAX_LEN);
        real_res_len = sum(res_temp, temp2, res, res_len, real_res_len);
        temp[len1] = 0;
        temp_len++;
        n++;
    }
}
/*
int mul(int * n1, int * n2, int * res, int len1, int len2){
    int n = 0;
    int temp[MAX_LEN];
    int temp2[MAX_LEN];
    int temp_len;
    int i;
    int start;
    int num, mul_num;
    int res_len = 0;

    while(1){
        if(len2-n-1 == -1){
            return res_len;
        }
        init(0, temp, MAX_LEN);
        
        num = n2[len2-n-1];
        if(num == 0){
            n++;
            continue;
        }
        start = n;
        for(i=len1-1;i>=0;i--){
            mul_num = num*n1[i];
            if(mul_num >= 10){
                temp[start] += mul_num%10;
                temp[start+1] += mul_num/10;
            }else{
                if(temp[start] + mul_num >= 10){
                    temp[start] += mul_num - 10;
                    temp[start+1] = 1;
                }else{
                    temp[start] += mul_num;
                }
            }
            start++;
        }
        if(temp[start] != 0){
            temp_len = start+1;
        }else{
            temp_len = start;
        }
        reverse(temp, temp, temp_len);
        for(i=temp_len;i<MAX_LEN;i++){
            temp[i] = -1;
        }
        for(i=0;i<MAX_LEN;i++){
            if(i<res_len){
                temp2[i] = res[i];
            }else{
                temp2[i] = -1;
            }
        }
        init(0, res, MAX_LEN);
        printf("temp : ");
        for(i=0;i<temp_len;i++){
            printf("%d",temp[i]);
        }
        printf("\ntemp2 : ");
        for(i=0;i<res_len;i++){
            printf("%d",temp2[i]);
        }
        printf("\n");
        res_len = sum(temp, temp2, res, temp_len, res_len);
        n++;
    }
}*/
int sum(int * n1, int * n2, int * res, int len1, int len2){
    int idx = 0;
    int sum_num;
    int finish1 = 0;
    int finish2 = 0;
    while(1){
        len1--;
        len2--;
        if(len1 == -1){
            finish1 = 1;
        }
        if(len2 == -1){
            finish2 = 1;
        }
        if(!finish1 && !finish2){
            sum_num = n1[len1] + n2[len2];
            if(sum_num >= 10){
                res[idx] += sum_num - 10;
                res[idx+1] = 1;
            }else{
                if(res[idx] + sum_num >= 10){
                    res[idx] += sum_num - 10;
                    res[idx+1] = 1;
                }else{
                    res[idx] += sum_num;
                }
            }
        }else if(!finish1){
            if(res[idx] + n1[len1] >= 10){
                res[idx] += n1[len1] - 10;
                res[idx+1] = 1;
            }else{
                res[idx] += n1[len1];
            }
        }else if(!finish2){
            if(res[idx] + n2[len2] >= 10){
                res[idx] += n2[len2] - 10;
                res[idx+1] = 1;
            }else{
                res[idx] += n2[len2];
            }
        }else{
            if(res[idx] != 0){
                idx++;
            }
            reverse(res, res, idx);
            return idx;
        }
        idx++;
    }
}
void reverse(int * arr, int * res, int len){
    int temp[len];
    int i;
    for(i=0;i<len;i++){
        temp[len-i-1] = arr[i];
    }
    for(i=0;i<len;i++){
        res[i] = temp[i];
    }
}
