#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 31
int static compare(const void* first, const void* second);
typedef struct{
    char name[NAME_LEN];
    int age;
    int rank;
}Horse;
int main(){
    int n;
    int i;

    scanf("%d", &n);
    Horse *arr = malloc(sizeof(Horse)*n);
    memset(arr, 0, sizeof(Horse)*n);

    for(i=0;i<n;i++){
        scanf("%s %d %d", arr[i].name, &arr[i].age, &arr[i].rank);
    }
    qsort(arr, n, sizeof(Horse), compare);

    for(i=0;i<n;i++){
        printf("%d %s %d\n", arr[i].age, arr[i].name, arr[i].rank);
    }

    free(arr);
    return 0;
}
int static compare(const void* first, const void* second){
    Horse *f = (Horse *)first;
    Horse *s = (Horse *)second;
    int result;
    if(f->age > s->age){
        return -1;
    }else if(f->age < s->age){
        return 1;
    }else{
        if((result = strcmp(f->name, s->name)) == 0){
            printf("result : %d\n", result);
            if(f->rank > s->rank){
                return -1;
            }else{
                return 1;
            }
        }else{
            printf("result : %d\n", result);
            return result;
        }
    }
}
