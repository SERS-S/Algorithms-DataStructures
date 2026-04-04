#include <stdio.h>
#include <stdlib.h>

// // // Heap // // //

// Structs
typedef struct HeapNode
{
    int value;
    int index;
} HPN;

typedef struct MinHeap
{
    HPN *data;
    int size;
    int capacity;
} MHP;

// Special functions
int hp_less(HPN a, HPN b)
{
    if (a.value != b.value) return a.value < b.value;
    return a.index < b.index;
}

void swap(HPN *a, HPN *b)
{
    HPN tmp = *b;
    *b = *a;
    *a = tmp;
}

void sift_up(MHP *heap, int idx)
{
    while (idx > 0 && hp_less(heap->data[idx], heap->data[(idx - 1) / 2]))
    {
        swap(&heap->data[idx], &heap->data[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

void sift_down(MHP *heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && hp_less(heap->data[left], heap->data[smallest])) smallest = left;
    if (right < heap->size && hp_less(heap->data[right], heap->data[smallest])) smallest = right;

    if (smallest != idx)
    {
        swap(&heap->data[idx], &heap->data[smallest]);
        sift_down(heap, smallest);
    }
}

// Heap functions
MHP* hp_create(int capacity)
{
    MHP *heap = (MHP*) malloc(sizeof(MHP));
    heap->data = (HPN*) malloc((size_t) capacity * sizeof(HPN));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void hp_insert(MHP *heap, int value, int index)
{
    if (heap->size == heap->capacity) return;
    heap->data[heap->size].value = value;
    heap->data[heap->size].index = index;
    sift_up(heap, heap->size);
    heap->size++;
}

HPN hp_get_min(MHP *heap)
{
    HPN bad = {-1, -1};
    if (heap->size == 0) return bad;
    return heap->data[0];
}

int hp_extract_idx(MHP *heap, int idx)
{
    if (idx >= heap->size) return -1;
    heap->data[idx] = heap->data[heap->size - 1];
    heap->size--;

    if (idx < heap->size)
    {
        int parent = (idx - 1) / 2;
        if (idx > 0 && hp_less(heap->data[idx], heap->data[parent])) { sift_up(heap, idx); }
        else { sift_down(heap, idx); }
    }

    return 0;
}

// // // Main logic // // //

int main(void)
{
    int N, K;
    if (scanf("%d %d", &N, &K) != 2) return 1;

    int *n_arr = (int*) malloc((size_t) N * sizeof(int));
    MHP *k_heap = hp_create(N);

    for (int i = 0; i < N; ++i)
    {
        if (scanf("%d", &n_arr[i]) != 1) return 1;

        hp_insert(k_heap, n_arr[i], i);

        while (k_heap->size > 0 && hp_get_min(k_heap).index <= i - K)
        {
            hp_extract_idx(k_heap, 0);
        }

        if (i >= K - 1)
        {
            fprintf(stdout, "%d ", hp_get_min(k_heap).value);
        }
    }
    printf("\n");

    free(n_arr);
    free(k_heap->data);
    free(k_heap);
    return 0;
}
