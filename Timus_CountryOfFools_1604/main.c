#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

int partition(int *arr, int *num, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        if (arr[j] < pivot)
        {
            ++i;
            swap(&arr[i], &arr[j]);
            swap(&num[i], &num[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    swap(&num[i + 1], &num[high]);
    return i + 1;
}

void quick_sort(int *arr, int *num, int low, int high)
{
    if (low < high)
    {
        int pivet = partition(arr, num, low, high);

        quick_sort(arr, num, low, pivet - 1);
        quick_sort(arr, num, pivet + 1, high);
    }
}

int main(void)
{
    int k;
    int n = 0;
    if (scanf("%d", &k) != 1) return 1;
    
    int *nk = (int*) malloc((size_t) k * sizeof(int));
    int *num = (int*) malloc((size_t) k * sizeof(int));
    if (!nk || !num) { free(nk); free(num); return 1; }
    for (int i = 0; i < k; ++i) 
    { 
        if (scanf("%d", &nk[i]) != 1) return 1;
        else n += nk[i];
    }
    for (int i = 0; i < k; ++i) num[i] = i + 1;

    quick_sort(nk, num, 0, k - 1);
    
    int *arr = (int*) malloc((size_t) n * sizeof(int));
    if (!arr) { free(nk); free(num); free(arr); return 1; }

    int last = -1;
    for (int i = 0; i < n; ++i)
    {
        int best = -1;
        for (int j = 0; j < k; ++j)
        {
            if (nk[j] == 0 || num[j] == last) continue;
            if (best == -1 || nk[j] > nk[best]) best = j;
        }

        if (best == -1)
        {
            for (int j = 0; j < k; ++j)
            {
                if (nk[j] > 0)
                {
                    best = j;
                    break;
                }
            }
        }

        arr[i] = num[best];
        nk[best]--;
        last = arr[i];
    }

    for (int i = 0; i < n; ++i) printf("%d ", arr[i]);
    printf("\n");

    free(nk);
    free(num);
    free(arr);
    return 0;
}
