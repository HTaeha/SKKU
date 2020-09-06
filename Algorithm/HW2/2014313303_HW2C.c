#include <stdio.h>

void PrintAll(int arr[][10], int len);
void Counting_Sort(int arr[][10], int res[][10], int len, int radix);
void Radix_Sort(int arr[][10], int res[][10], int len);
int main(){
    int hex[20][10];  //Store input values.
    int result[20][10];  //Store result values.
    int r=0,c=0;  //row, column.
    char input_c;  //Store character type input.
    int input_i;  //Store integer type input.
    int length;  //Store the number of input.

    while(1){  //Get inputs.
        input_c = getchar();
        if(input_c <= 'F' && 'A' <= input_c){
            input_i = input_c - 'A' + 10;
        }else if(input_c == ' '){
            c = 0;
            r++;
            continue;
        }else if(input_c == '\n'){
            break;
        }else{
            input_i = input_c - '0';
        }
        hex[r][c++] = input_i;
    }
   
    length = r+1; //The number of ten-digit hexadecimal numbers.

    Radix_Sort(hex, result, length);

    return 0;
}
void PrintAll(int arr[][10], int len){  //Print all value of array.
    int i,j;

    for(i=0;i<len;i++){
        for(j=0;j<10;j++){
            if(arr[i][j] >= 10){  //If a value is over 10, print alphabet(hexadecimal).
                printf("%c", arr[i][j]-10+'A');
            }else{
                printf("%d",arr[i][j]);
            }
        }
        printf(" ");
    }
    printf("\n");
}
void Counting_Sort(int arr[][10], int res[][10], int len, int radix){
    int i,j;
    int value;
    int count[16];  //Count hexadecimal numbers and store.

    for(i=0;i<16;i++){  //Initialize array count.
        count[i] = 0;
    }
    for(i=0;i<len;i++){  //Counting numbers.
        count[arr[i][radix]]++;
    }
    for(i=1;i<16;i++){  //Determine position.
        count[i] += count[i-1];
    }
    for(i=len-1;i>=0;i--){  //Store sorted numbers in res array.
        value = arr[i][radix];
        for(j=9;j>=0;j--){
            res[count[value]-1][j] = arr[i][j];
        }
        count[value]--;
    }
}
void Radix_Sort(int arr[][10], int res[][10], int len){
    int j;
    int r,c;  //row, column.

    for(j=9;j>=0;j--){ //Sort by radix.
        Counting_Sort(arr,res,len,j);
        
        for(r=0;r<len;r++){  //Copy from res to arr.
            for(c=0;c<10;c++){
                arr[r][c] = res[r][c];
            }
        }
        PrintAll(res,len); //Print.
    }
}
