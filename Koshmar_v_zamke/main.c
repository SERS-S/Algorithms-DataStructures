#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void maximization_s_weight(char alp[], int ct_alp[], char *s, int N)
{
    int l = 0;
    int r = N - 1;

    for (int i = 25; i >= 0; --i)
    {
        if (ct_alp[i] >= 2)
        {
            s[l++] = alp[i];
            s[r--] = alp[i];
            ct_alp[i] -= 2;
        }
    }

    for (int i = 0; i < 26; ++i)
    {
        while (ct_alp[i] > 0)
        {
            s[l++] = alp[i];
            --ct_alp[i];
        }
    }

    s[N] = '\0';
}

int alp_num_map(char a)
{
    switch (a)
    {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'g': return 6;
        case 'h': return 7;
        case 'i': return 8;
        case 'j': return 9;
        case 'k': return 10;
        case 'l': return 11;
        case 'm': return 12;
        case 'n': return 13;
        case 'o': return 14;
        case 'p': return 15;
        case 'q': return 16;
        case 'r': return 17;
        case 's': return 18;
        case 't': return 19;
        case 'u': return 20;
        case 'v': return 21;
        case 'w': return 22;
        case 'x': return 23;
        case 'y': return 24;
        case 'z': return 25;
        default: break;
    }
    return 0;
}

void swap_int(int *a, int *b)
{
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

void swap_char(char *a, char *b)
{
    char tmp = *b;
    *b = *a;
    *a = tmp;
}

int partition(
    char alp[], 
    int ct_alp[], 
    int low, 
    int high, 
    int walp[]
)
{
    int pivot = walp[alp_num_map(alp[high])];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        if (walp[alp_num_map(alp[j])] < pivot)
        {
            ++i;
            swap_int(&ct_alp[i], &ct_alp[j]);
            swap_char(&alp[i], &alp[j]);
        }
    }

    swap_int(&ct_alp[i + 1], &ct_alp[high]);
    swap_char(&alp[i + 1], &alp[high]);
    return i + 1;
}

void quick_sort(
    char alp[], 
    int ct_alp[], 
    int low, 
    int high, 
    int walp[]
)
{
    if (low < high)
    {
        int pivet = partition(alp, ct_alp, low, high, walp);

        quick_sort(alp, ct_alp, low, pivet - 1, walp);
        quick_sort(alp, ct_alp, pivet + 1, high, walp);
    }
}

int main(void)
{
    // input
    char *s = (char*) malloc((size_t)(1e5 + 1) * sizeof(char));
    if (!s) return 1;
    if (scanf("%100000s", s) != 1) return 1;
    int N = strlen(s);

    int walp[26];
    int error = 0;
    for (int i = 0; i < 26; ++i)
    {
        if (scanf("%d", &walp[i]) != 1) ++error;
    }
    if (error) return 1;

    // code
    char alp[26] = 
    {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };
    int ct_alp[26] = {0};
    for (int i = 0; i < N; ++i) ct_alp[alp_num_map(s[i])]++;

    quick_sort(alp, ct_alp, 0, 25, walp);

    // for (int i = 0; i < 26; ++i) printf("%c ", alp[i]);
    // printf("\n");
    // for (int i = 0; i < 26; ++i) printf("%d ", ct_alp[i]);
    // printf("\n");

    char *mx_w_s = (char*) malloc(((size_t)N + 1) * sizeof(char));
    if (!mx_w_s) return 1;
    maximization_s_weight(alp, ct_alp, mx_w_s, N);
    printf("%s\n", mx_w_s);

    free(mx_w_s);
    free(s);
    return 0;
}
