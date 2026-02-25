#include <stdio.h>

int main(void)
{
    int t;
    scanf("%d", &t);

    for (int i = 0; i < t; ++i)
    {
        int n, k;
        scanf("%d %d", &n, &k);

        int q = n / k;
        int r = n % k;

        int cn2 = n * (n - 1) / 2;
        int inside = r * (q + 1) * q / 2 + (k - r) * q * (q - 1) / 2;
        int ans = cn2 - inside;

        printf("%d\n", ans);
    }

    return 0;
}
