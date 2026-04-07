#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    int a[100000];
    int stack[100000];

    int N;
    if (scanf("%d", &N) != 1) return 1;

    for (int i = 0; i < N; ++i) { if (scanf("%d", &a[i]) != 1) return 1; }

    bool ok = true;
    int pos = 0, pos2 = 0, sz = 0;

    while (ok && pos < N)
    {
        if (sz > 0 && stack[sz - 1] == a[pos])
        {
            --sz;
            ++pos;
        }
        else if (pos2 < N)
        {
            stack[sz] = ++pos2;
            ++sz;
        }
        else ok = false;
    }

    printf("%s\n", ok ? "Not a proof" : "Cheater");

    return 0;
}