#include <stdio.h>
#include <stdlib.h>

void dfs(int *arr, int *visited, int index, int *ct)
{
    visited[index] = 1;

    int to = arr[index] - 1;
    if (visited[to] == 0) 
    {
        dfs(arr, visited, to, ct);
    }
    else if (visited[to] == 1)
    {
        (*ct)++;
    }

    visited[index] = 2;
}

int main(void)
{
    int n;
    if (scanf("%d", &n) != 1) return 1;

    int *arr = (int*) malloc((size_t) n * sizeof(int));
    int *visited = (int*) malloc((size_t) n * sizeof(int));
    if (arr == NULL || visited == NULL) return 1;

    for (int i = 0; i < n; ++i) 
    {
        if (scanf("%d", &arr[i]) != 1) return 1;
        visited[i] = 0;
    }

    int ct = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!visited[i]) 
        {
            dfs(arr, visited, i, &ct);
        }
    }

    fprintf(stdout, "%d\n", ct);

    free(arr);
    free(visited);

    return 0;
}