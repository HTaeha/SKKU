#include <stdio.h>
#include <stdlib.h>

void dtoh(char * hexa,int num);
void Insertion_Sort(int * arr, int index_arr);
void Merge_Sort(int * arr, int p, int r);
void Merge(int * arr, int p, int q, int r);
int main(){
    int sort;         //Determine what kind of sorting.
    char input_c;     //Save a character type input.
    int input_i;      //Save a integer type input.
    int input = 0;    //Save a decimal number changed a four-digit hexadecimal number.
    char hex[4];      //Save a four-digit hexadecimal number.
    int dec[50];      //Save decimal numbers.
    int index = 0;    //Save index of dec array.
    int i,j;

    scanf("%d",&sort);  //1: insertion sort, 2: merge sort

    while((input_c = getchar()) != '\n'){  //If input is '\n', stop.
        input = 0;
        for(i=0;i<4;i++){
            input_c = getchar();
            if(input_c <='F' && 'A'<=input_c){ //If input is a alphabet, change it a number then save it in a input_i.
                input_i = input_c-'A'+10;
            }else{ //If input is a number, save it in a input_i.
                input_i = input_c-'0';
            }
            for(j=0;j<3-i;j++){
                input_i *= 16;
            }
            input += input_i; //Change hexadecimal number to decimal number.
        }
        dec[index++] = input; //Save decimal numbers.
    }

    if(sort == 1){  //If sort is 1, sort decimal numbers by insertion sort. index is a length of dec array.
        Insertion_Sort(dec,index); 
    }else if(sort ==2){ //If sort is 2, sort decimal numbers by merge sort.
        Merge_Sort(dec,0,index-1);
    }
    for(i=0;i<index;i++){ //Change decimal numbers to four-digit hexadecimal numbers and print.
        dtoh(hex,dec[i]);
        for(j=0;j<4;j++){
            printf("%c",hex[j]);
        }
        if(i != index-1){ //Insert space between numbers.
            printf(" ");
        }
    }
}
void dtoh(char * hexa,int num){ //Change decimal number to hexadecimal number.
    int i;
    int value;
    for(i=0;i<4;i++){
        value = num%16;
        if(value == 10){
            hexa[3-i] = 'A';
        }else if(value == 11){
            hexa[3-i] = 'B';
        }else if(value == 12){
            hexa[3-i] = 'C';
        }else if(value == 13){
            hexa[3-i] = 'D';
        }else if(value == 14){
            hexa[3-i] = 'E';
        }else if(value == 15){
            hexa[3-i] = 'F';
        }else{
            hexa[3-i] = value + '0';
        }
        num /= 16;
    }
}
void Insertion_Sort(int * arr, int index_arr){ //Sort numbers to use insertion sort.
    int i,j;
    int key;

    for(j=1;j<index_arr;j++){
        key = arr[j];    
        i = j-1;
        while(i>=0 && arr[i] < key){
            arr[i+1] = arr[i];
            i--;
        }
        arr[i+1] = key;
    }
}
void Merge_Sort(int * arr, int p, int r){ //Sort numbers to use merge sort.
    int q;  //p:first index, r:last index, q:middle index.
    if(p<r){
        q = (p+r)/2;
        Merge_Sort(arr, p, q);
        Merge_Sort(arr, q+1, r);
        Merge(arr,p,q,r);
    }
}
void Merge(int * arr, int p, int q, int r){
    int n1 = q-p+1; //Size of left side array.
    int n2 = r-q;   //Size of right side array.
    int i,j,k;
    int * L;
    int * R;
    L = (int *)malloc(sizeof(int)*(n1+1)); //Make temporary array L, R.
    R = (int *)malloc(sizeof(int)*(n2+1));

    for(i=0;i<n1;i++){ //Bring left side values to array L.
        L[i] = arr[p+i];
    }
    for(j=0;j<n2;j++){ //Bring right side values to array R.
        R[j] = arr[q+j+1];
    }

    L[n1] = -1; //Save the most smallest value in the last index of array.
    R[n2] = -1;

    i = 0;
    j = 0;

    for(k=p;k<=r;k++){ //Combine array L and R.
        if(L[i] >= R[j]){
            arr[k] = L[i++];
        }else{
            arr[k] = R[j++];
        }
    }

    free(L);
    free(R);
}
