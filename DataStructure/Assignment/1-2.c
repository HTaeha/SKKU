#include <stdio.h>
#include <stdlib.h>

void dfs(int x, int y, int n);
int check(int x, int y, int n);
int ** arr;
int r,c;
int main(){
    int i,j,k;
    int num = 2;
    int result = 0;
    int flag = 0;

    scanf("%d %d", &r, &c);

    arr = malloc(sizeof(int*) * r);
    for(i=0;i<r;i++){
        arr[i] = malloc(sizeof(int)*c);
    }
    for(i=0;i<r;i++){
        for(j=0;j<c;j++){
            scanf("%d", &arr[i][j]);
        }
    }
    for(i=0;i<r;i++){
        for(j=0;j<c;j++){
            if(arr[i][j] == 0){
                dfs(i,j,num);
                num++;
            }
        }
    }
    for(i=2;i<num;i++){
        flag = 0;
        for(j=0;j<r;j++){
            for(k=0;k<c;k++){
                if(arr[j][k] == i){
                    if(check(j,k,i)){
                        result++;
                        flag = 1;
                        break;
                    }
                }
            }
            if(flag){
                break;
            }
        }
    }
    printf("%d\n",result);

    for(i=0;i<r;i++){
        free(arr[i]);
    }
    free(arr);
    return 0;
}
void dfs(int x, int y, int n){
    int i,j;
    arr[x][y] = n;

    for (i = -1; i <= 1; i++){ 
        for (j = -1; j <= 1; j++){
            if((i==-1&&j!=0) || (i==1&&j!=0)){
                continue;
            }
            int a = x + i;
            int b = y + j;
            if(a == -1 || b == -1 || a == r || b == c){
                continue;
            }
            if (arr[a][b] == 0){
                dfs(a, b, n);
            }
        }
    }
}
int check(int x, int y, int n){
    int i,j;
    for(i=0;i<=1;i++){
        for(j=0;j<=1;j++){
            int a = x + i;
            int b = y + j;
            if(a == r || b == c){
                return 0;
            }else if(arr[a][b] != n){
                return 0;
            }
        }
    }
    return 1;
}
