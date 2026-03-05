#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

bool d_check(const int *x, int D, int K, int N)
{
    --K;
    int cp = 0;
    for (int i = 1; i < N; ++i)
    {
        if (x[i] - x[cp] >= D)
        {
            if (K > 0) --K;
            cp = i;
            if (K == 0) return true;
        }
    }
    return K == 0;
}

int main(void)
{
    int N, K;
    if (scanf("%d %d", &N, &K) != 2) return 0;

    int *x = (int*) malloc(N * sizeof(int));
    if (x == NULL) return 0;
    
    for (int i = 0; i < N; ++i)
    {
        if (scanf("%d", &x[i]) != 1)
        {
            free(x);
            return 0;
        }
    }

    int mx = 0;
    int L = 0, R = x[N - 1] - x[0];
    while (L < R)
    {
        int middle = (L + R + 1) / 2;
        bool kc = d_check(x, middle, K, N);
        if (kc) 
        {
            L = middle;
            mx = max(mx, middle);
        }
        else R = middle - 1;
    }

    printf("%d\n", mx);
    free(x);

    return 0;
}
