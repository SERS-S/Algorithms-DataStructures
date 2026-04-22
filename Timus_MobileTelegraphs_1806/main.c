#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define INF ((long long) 4e18)

// // // HashMap // // //

typedef struct Node
{
    char *num;
    int index;
    struct Node *next;
} ND;

typedef struct Map
{
    ND **buckets;
    int buckets_ct;
} Map;

int map_hash(char *num, int buckets_ct)
{
    uint32_t h = 2166136261u;
    unsigned char *s = (unsigned char*) num;

    while (*s)
    {
        h ^= (uint32_t) *s;
        h *= 16777619u;
        ++s;
    }

    h ^= h >> 15;
    h *= 0x846ca68bU;
    h ^= h >> 16;

    return (int) (h % (uint32_t) buckets_ct);
}

void map_init(Map *m, int buckets_ct)
{
    m->buckets = (ND**) calloc(buckets_ct, sizeof(ND*));
    m->buckets_ct = buckets_ct;
}

void map_free(Map *m)
{
    for (int i = 0; i < m->buckets_ct; ++i)
    {
        ND *next = m->buckets[i];
        while (next)
        {
            ND *nx = next->next;
            free(next);
            next = nx;
        }
    }

    free(m->buckets);
    m->buckets = NULL;
    m->buckets_ct = 0;
}

void map_set(Map *m, char *num, int index)
{
    int idx = map_hash(num, m->buckets_ct);

    for (ND *cur = m->buckets[idx]; cur != NULL; cur = cur->next)
    {
        if (strcmp(num, cur->num) == 0)
        {
            cur->index = index;
            return;
        }
    }

    ND *new_nd = (ND*) malloc(sizeof(ND));
    new_nd->num = num;
    new_nd->index = index;
    new_nd->next = m->buckets[idx];
    m->buckets[idx] = new_nd;
}

int map_get(Map *m, char *num)
{
    int idx = map_hash(num, m->buckets_ct);

    for (ND *cur = m->buckets[idx]; cur != NULL; cur = cur->next)
    {
        if (strcmp(num, cur->num) == 0) return cur->index;
    }

    return -1;
}

// // // Heap // // //

typedef struct HeapNode
{
    long long dist;
    int v;
} HeapNode;

typedef struct Heap
{
    HeapNode *a;
    int size;
    int cap;
} Heap;

void heap_init(Heap *h, int cap)
{
    h->a = (HeapNode*) malloc((size_t)cap * sizeof(HeapNode));
    h->size = 0;
    h->cap = cap;
}

void heap_free(Heap *h)
{
    free(h->a);
    h->a = NULL;
    h->size = 0;
    h->cap = 0;
}

void heap_push(Heap *h, long long dist, int v)
{
    if (h->size == h->cap)
    {
        h->cap *= 2;
        h->a = (HeapNode*) realloc(h->a, (size_t)h->cap * sizeof(HeapNode));
    }

    int i = h->size++;
    h->a[i].dist = dist;
    h->a[i].v = v;

    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (h->a[p].dist <= h->a[i].dist) break;

        HeapNode t = h->a[p];
        h->a[p] = h->a[i];
        h->a[i] = t;
        i = p;
    }
}

HeapNode heap_pop(Heap *h)
{
    HeapNode res = h->a[0];
    h->size--;
    h->a[0] = h->a[h->size];

    int i = 0;
    while (1)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int s = i;

        if (l < h->size && h->a[l].dist < h->a[s].dist) s = l;
        if (r < h->size && h->a[r].dist < h->a[s].dist) s = r;
        if (s == i) break;

        HeapNode t = h->a[i];
        h->a[i] = h->a[s];
        h->a[s] = t;
        i = s;
    }

    return res;
}

// // // Dijkstra // // //

void relax_edge(int from, int to, int w, long long *dist, int *parent, Heap *heap)
{
    if (dist[from] + w < dist[to])
    {
        dist[to] = dist[from] + w;
        parent[to] = from;
        heap_push(heap, dist[to], to);
    }
}

void dijkstra(char (*nums)[11], int n, int *cost, Map *mp, long long *dist, int *parent)
{
    Heap heap;
    heap_init(&heap, 16);

    dist[0] = 0;
    heap_push(&heap, 0, 0);

    while (heap.size > 0)
    {
        HeapNode cur = heap_pop(&heap);
        long long cur_dist = cur.dist;
        int v = cur.v;

        if (cur_dist != dist[v]) continue;
        if (v == n - 1) break;

        char s[11];
        strcpy(s, nums[v]);

        // 1 symbol change
        for (int p = 0; p < 10; p++)
        {
            char old = s[p];
            for (char c = '0'; c <= '9'; c++)
            {
                if (c == old) continue;
                s[p] = c;

                int to = map_get(mp, s);
                if (to != -1)
                    relax_edge(v, to, cost[p], dist, parent, &heap);
            }
            s[p] = old;
        }

        // 2 symbol swap
        for (int p = 0; p < 10; p++)
        {
            for (int q = p + 1; q < 10; q++)
            {
                if (s[p] == s[q]) continue;

                char t = s[p];
                s[p] = s[q];
                s[q] = t;

                int to = map_get(mp, s);
                if (to != -1)
                    relax_edge(v, to, cost[p], dist, parent, &heap);

                t = s[p];
                s[p] = s[q];
                s[q] = t;
            }
        }
    }

    heap_free(&heap);
}

// // // Main logic // // //

int main()
{
    int n;
    if (scanf("%d", &n) != 1) return 1;

    int *cost = (int*) malloc(10 * sizeof(int));
    if (cost == NULL) return 1;

    for (int i = 0; i < 10; i++)
    {
        if (scanf("%d", &cost[i]) != 1)
        {
            free(cost);
            return 1;
        }
    }

    char (*nums)[11] = (char (*)[11]) malloc((size_t)n * sizeof(char[11]));
    if (nums == NULL)
    {
        free(cost);
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        if (scanf("%10s", nums[i]) != 1)
        {
            free(cost);
            free(nums);
            return 1;
        }
    }

    Map mp;
    map_init(&mp, n * 2);
    for (int i = 0; i < n; i++) map_set(&mp, nums[i], i);

    long long *dist = (long long*) malloc((size_t) n * sizeof(long long));
    int *parent = (int*) malloc((size_t)n * sizeof(int));
    if (dist == NULL || parent == NULL)
    {
        free(cost);
        free(nums);
        map_free(&mp);
        free(dist);
        free(parent);
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
    }

    dijkstra(nums, n, cost, &mp, dist, parent);

    if (dist[n - 1] == INF)
    {
        printf("-1\n");
    }
    else
    {
        int *path = (int*) malloc((size_t)n * sizeof(int));
        int cnt = 0;

        for (int v = n - 1; v != -1; v = parent[v]) { path[cnt++] = v + 1; }

        printf("%lld\n", dist[n - 1]);
        printf("%d\n", cnt);
        for (int i = cnt - 1; i >= 0; i--)
        {
            printf("%d", path[i]);
            if (i > 0) printf(" ");
        }
        printf("\n");

        free(path);
    }

    free(dist);
    free(parent);
    map_free(&mp);
    free(cost);
    free(nums);

    return 0;
}
