#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

int dfs_backward(int **map, int n, int vl)
{
    int used[n];
    memset(used, 0, sizeof(used));

    int stack[n];
    int st_p = 0;

    stack[st_p++] = 0;
    used[0] = 1;

    while (st_p > 0)
    {
        int v = stack[--st_p];
        for (int from = 0; from < n; ++from)
        {
            if (!used[from] && map[from][v] <= vl)
            {
                used[from] = 1;
                stack[st_p++] = from;
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (!used[i]) return 0;
    }

    return 1;
}

int dfs_forward(int **map, int n, int vl)
{
    int used[n];
    memset(used, 0, sizeof(used));

    int stack[n];
    int st_p = 0;

    stack[st_p++] = 0;
    used[0] = 1;

    while (st_p > 0)
    {
        int v = stack[--st_p];
        for (int to = 0; to < n; ++to)
        {
            if (!used[to] && map[v][to] <= vl)
            {
                used[to] = 1;
                stack[st_p++] = to;
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (!used[i]) return 0;
    }

    return 1;
}

int check(int **map, int n, int vl)
{
    return dfs_forward(map, n, vl) && dfs_backward(map, n, vl);
}

int main(void)
{
    int n;
    if (scanf("%d", &n) != 1) return 1;

    int mx = 0;
    int **map = (int**) malloc((size_t) n * sizeof(int*));
    if (map == NULL) return 1;
    for (int i = 0; i < n; ++i) 
    {
        map[i] = (int*) malloc((size_t) n * sizeof(int));
        if (map[i] == NULL) return 1;
        for (int j = 0; j < n; ++j) 
        {
            if (scanf("%d", &map[i][j]) != 1) return 1;
            mx = max(mx, map[i][j]);
        }   
    }

    int left = 0, right = mx;
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (check(map, n, mid)) right = mid;
        else left = mid + 1;
    }

    fprintf(stdout, "%d\n", left);

    for (int k = 0; k < n; ++k) free(map[k]);
    free(map);

    return 0;
}