#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int*) a;
    int y = *(const int*) b;
    return (x > y) - (x < y);
}

int main(void)
{
    int n;
    if (scanf("%d", &n) != 1) return 1;

    int *xn = (int*) malloc((size_t) n * sizeof(int));
    int *yn = (int*) malloc((size_t) n * sizeof(int));
    if (!xn || !yn) { free(xn); free(yn); return 1; }
    for (int i = 0; i < n; ++i)
    {
        if (scanf("%d %d", &xn[i], &yn[i]) != 2) return 1;
    }

    qsort(xn, (size_t) n, sizeof(int), cmp_int);
    qsort(yn, (size_t) n, sizeof(int), cmp_int);

    long long sum = 0;
    long long pref_x_sum = xn[0];
    long long pref_y_sum = yn[0];
    for (int i = 1; i < n; ++i)
    {
        long long dx = llabs((long long) xn[i] * i - pref_x_sum);
        long long dy = llabs((long long) yn[i] * i - pref_y_sum);
        sum += (dx + dy);
        pref_x_sum += xn[i];
        pref_y_sum += yn[i];
    }

    long long cn2 = (long long) n * (n - 1) / 2;
    long long avg = sum / cn2;
    printf("%lld\n", avg);

    free(xn);
    free(yn);
    return 0;
}
