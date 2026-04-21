#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dfs(
    int **ref, 
    int *sz, 
    int *color, 
    int index, 
    int v,
    int *flag
)
{
    if (*flag) return;
    if (color[index] == -1)
    {
        color[index] = v;
    }
    else if (color[index] != v)
    {
        *flag = 1;
        return;
    }

    for (int i = 0; i < sz[index]; ++i)
    {
        int u = ref[index][i] - 1;
        if (color[u] == -1)
        {
            dfs(ref, sz, color, u, !v, flag);
        }
        else if (color[u] == v)
        {
            *flag = 1;
            return;
        }
    }
    return;
}

int main(void)
{
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) return 1;

    int **ref = (int**) calloc((size_t) N, sizeof(int*));
    int *sz = (int*) calloc((size_t) N, sizeof(int));
    int *cp = (int*) calloc((size_t) N, sizeof(int));
    int *color = (int*) malloc((size_t) N * sizeof(int));
    if (ref == NULL || sz == NULL || cp == NULL || color == NULL) return 1;
    memset(color, -1, (size_t) N * sizeof(int));

    for (int i = 0; i < M; ++i)
    {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) return 1;

        if (sz[x - 1] == cp[x - 1])
        {
            int new_cp = cp[x - 1] == 0 ? 8 : cp[x - 1] * 2;
            int *new_ref = (int*) realloc(ref[x - 1], (size_t) new_cp * sizeof(int));
            if (new_ref == NULL) return 1;
            ref[x - 1] = new_ref;
            cp[x - 1] = new_cp;
        }
        ref[x - 1][sz[x - 1]] = y;
        sz[x - 1]++;

        if (sz[y - 1] == cp[y - 1])
        {
            int new_cp = cp[y - 1] == 0 ? 8 : cp[y - 1] * 2;
            int *new_ref = (int*) realloc(ref[y - 1], (size_t) new_cp * sizeof(int));
            if (new_ref == NULL) return 1;
            ref[y - 1] = new_ref;
            cp[y - 1] = new_cp;
        }
        ref[y - 1][sz[y - 1]] = x;
        sz[y - 1]++;
    }

    int flag = 0;
    for (int i = 0; i < N; ++i)
    {
        if (color[i] != -1) continue;
        dfs(ref, sz, color, i, 0, &flag);
    }
    printf("%s\n", flag ? "NO" : "YES");

    for (int i = 0; i < N; ++i) free(ref[i]);
    free(ref);
    free(sz);
    free(cp);
    free(color);

    return 0;
}
