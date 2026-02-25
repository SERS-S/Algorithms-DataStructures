#include <stdio.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

int main(void)
{
    int n;
    scanf("%d", &n);

    int x;
    int cur = 0;
    int mx = 0;

    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &x);

        cur += x;
        if (cur < 0) cur = 0;
        else mx = max(mx, cur);
    }

    printf("%d\n", mx);

    return 0;
}