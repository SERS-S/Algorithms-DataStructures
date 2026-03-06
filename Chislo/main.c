#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp(const void *a, const void *b)
{
    const char *x = (const char*) a;
    const char *y = (const char*) b;
    size_t lx = strlen(x), ly = strlen(y);

    for (size_t i = 0; i < lx + ly; ++i) 
    {
        char cx = (i < lx) ? x[i] : y[i - lx];
        char cy = (i < ly) ? y[i] : x[i - ly];
        if (cx != cy) return (cx > cy) ? -1 : 1;
    }
    return 0;
}

int main(void)
{
    char (*n)[101] = (char(*)[101]) malloc((size_t)100 * sizeof(*n));
    if (!n) return 1;

    int ct = 0;
    while (ct < 100 && scanf("%100s", n[ct]) == 1) ++ct;

    qsort(n, ct, sizeof(n[0]), cmp);

    for (int i = 0; i < ct; ++i) printf("%s", n[i]);
    printf("\n");

    free(n);
    return 0;
}
