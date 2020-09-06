#include <stdio.h>

int main(){
    int num;
    int a;
    char c;
    char s[1];
    int i;

    scanf("%d", &num);
    printf("num : %d\n", num);

    for (i=0;i<num;i++){
        scanf("%c", &c); 
        printf("i : %d, num : %d\n", i, num);
    }
    return 0;
}
