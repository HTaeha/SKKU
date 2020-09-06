#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 51
int static compare (const void* first, const void* second);
typedef struct{
    char name[WORD_LEN];
    int cnt;
}Word;
int main() {
    char input[WORD_LEN];
    int n;
    int i,j;
    int idx=0;
    int flag = 0;

    scanf("%d", &n);
    Word *dict = malloc(sizeof(Word)*n);
    memset(dict, 0, sizeof(Word)*n);

    for(i=0;i<n;i++){
        flag = 0;
        scanf("%s", input);
        for(j=0;j<=idx;j++){
            if(strcmp(dict[j].name, input) == 0){
                dict[j].cnt++;
                flag = 1;
                break;
            }
        }
        if(!flag){
            strcpy(dict[idx++].name, input);
        }
    }
    qsort(dict, idx, sizeof(Word), compare);
    for(i=0;i<idx;i++){
        printf("%s\n", dict[i].name);
    }

    free(dict);
    return 0;
}
int static compare (const void* first, const void* second){
    Word *f = (Word *)first;
    Word *s = (Word *)second;
    if (f->cnt > s->cnt){
        return -1;
    }else if (f->cnt < s->cnt){
        return 1;
    }else{
        return strcmp(f->name, s->name);
    }
}
