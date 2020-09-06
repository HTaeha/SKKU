#include <stdio.h>
#include <stdlib.h>

int main() {
    char input;  //Character type input.
    int N=0;  //The number of raccoon.
    int state[50];  //Store raccoon's state.
    long long ** dp; //Store the minimum number of changes i-th raccoon.
    long long PofTwo = 1;  //Power of 2.
    int i, j;

    //Input is only 0 or 1.
    while ((input = getchar()) == '0' || input == '1') {
        state[N++] = input - '0'; //Save the input as an integer type.
    }

    //Allocate array dp.
    dp = (long long**)malloc(sizeof(long long*) * N);
    for (i = 0; i < N; i++) {
        dp[i] = (long long*)malloc(sizeof(long long) * 2);
    }
    //Last raccoon.
    dp[N-1][state[N-1]] = 0;
    dp[N-1][!state[N-1]] = 1;
    //Save the minimum number of changes i-th raccoon in the array dp.
    //dp[i][0] : The minimum number of changes that needed to put to sleep the last N-i+1 raccoons.
    //dp[i][1] : The minimum number of changes that needed to wake up a i-th raccoon and put to sleep all the following ones.
    for (i = N-2; i >= 0; i--) {
        PofTwo = 1;
        for (j = 0; j < N - i - 1; j++) {
            PofTwo *= 2;
        }
        if (state[i] == 0) { //i-th raccoon is sleeping.
            dp[i][0] = dp[i + 1][0];
            dp[i][1] = dp[i + 1][1] + PofTwo;
        }
        else { //i-th raccoon is awake.
            dp[i][0] = dp[i + 1][1] + PofTwo;
            dp[i][1] = dp[i + 1][0];
        }
    }

    //Result is dp[0][0].
    printf("%lld\n", dp[0][0]);

    //Free array dp.
    for (i = 0; i < N; i++) {
        free(dp[i]);
    }
    free(dp);

    return 0;
}
