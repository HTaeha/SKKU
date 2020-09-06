#include <stdio.h>
#include <stdlib.h>

//Card has 4 suit and 13 rank, so max num is 4*13.
#define MAX_NUM 52
//Card's suit.
typedef enum{
    CLUBS = 0, DIAMONDS, HEARTS, SPADES
}Suit;
//Card's rank.
typedef enum{
    ONE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, J, Q, K, A
}Rank;
typedef struct {
    Suit suit;
    Rank rank;
}Card;

void set_suit(Card * c, char * s);
void set_rank(Card * c, char * r);
void print_enum(Suit s, Rank r);
int static compare(const void* first, const void* second);
int main(){
    int num;
    char s[1];
    char r[2];
    Card temp;
    int i;

    scanf("%d", &num);
    Card *cards = malloc(sizeof(Card)*num);

    for(i=0;i<num;i++){
        scanf("%s %s", s, r);
        set_suit(&temp, s);
        set_rank(&temp, r);
        cards[i] = temp;
    }

    qsort(cards, num, sizeof(Card), compare);
    for(i = 0;i<num;i++){
        print_enum(cards[i].suit, cards[i].rank);
    }

    free(cards);
    return 0;
}
void set_suit(Card * c, char * s){
    if(s[0] == 'C'){
        c->suit = CLUBS;
    }else if(s[0] == 'D'){
        c->suit = DIAMONDS;
    }else if(s[0] == 'H'){
        c->suit = HEARTS;
    }else if(s[0] == 'S'){
        c->suit = SPADES;
    }
}
void set_rank(Card * c, char * r){
    if(r[0] == 'J'){
        c->rank = J;
    }else if(r[0] == 'Q'){
        c->rank = Q;
    }else if(r[0] == 'K'){
        c->rank = K;
    }else if(r[0] == 'A'){
        c->rank = A;
    }else{
        if(r[0] == '1'){
            c->rank = TEN;
        }else{
            c->rank = r[0] - '0';
        }
    }
}
void print_enum(Suit s, Rank r){
    if(s == 0){
        printf("C ");
    }else if(s == 1){
        printf("D ");
    }else if(s == 2){
        printf("H ");
    }else if(s == 3){
        printf("S ");
    }
    if(r == 11){
        printf("J\n");
    }else if(r == 12){
        printf("Q\n");
    }else if(r == 13){
        printf("K\n");
    }else if(r == 14){
        printf("A\n");
    }else{
        printf("%d\n", r);
    }

}
//Sort by suit, then sort by rank.
int static compare(const void* first, const void* second){
    Card *f = (Card *)first;
    Card *s = (Card *)second;
    if(f->suit > s->suit){
        return 1;
    }else if(f->suit < s->suit){
        return -1;
    }else{
        if(f->rank > s->rank){
            return 1;
        }else if(f->rank < s->rank){
            return -1;
        }else{
            return 0;
        }
    }
}
