#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

/* HashMap */

typedef struct Node
{
    int num;
    int ki;
    struct Node *next;
} ND;

typedef struct Map
{
    ND **buckets;
    int buckets_ct;
} Map;

int map_hash(int num, int buckets_ct)
{
    uint32_t h = (uint32_t) num;

    h ^= h >> 16;
    h *= 0x7feb352dU;
    h ^= h >> 15;
    h *= 0x846ca68bU;
    h ^= h >> 16;

    return (int) (h % (uint32_t) buckets_ct);
}

void map_init(Map *m, int buckets_ct)
{
    m->buckets = (ND**) calloc(buckets_ct, sizeof(ND*));
    m->buckets_ct = buckets_ct;
    return;
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
    return;
}

void map_set(Map *m, int num, int ki)
{
    int idx = map_hash(num, m->buckets_ct);

    for (ND *cur = m->buckets[idx]; cur != NULL; cur = cur->next)
    {
        if (num == cur->num)
        {
            cur->ki = ki;
            return;
        }
    }

    ND *new_nd = (ND*) malloc(sizeof(ND));
    new_nd->num = num;
    new_nd->ki = ki;
    new_nd->next = m->buckets[idx];
    m->buckets[idx] = new_nd;
    return;
}

int map_get(Map *m, int num)
{
    int idx = map_hash(num, m->buckets_ct);

    for (ND *cur = m->buckets[idx]; cur != NULL; cur = cur->next)
    {
        if (num == cur->num) return cur->ki;
    }

    return -1;
}

/* Code */

int func(int a, int b, int c, int d)
{
    if (a * b < c) return 0;
    else
    {
        return min((a * b - c), d);
    }
}

int main(void)
{
    Map map;
    int a, b , c, d;
    long long k;

    scanf("%d %d %d %d %lld", &a, &b, &c, &d, &k);

    int *nums = (int*) malloc((d + 2) * sizeof(int));
    map_init(&map, d + 2);
    
    nums[0] = func(a, b, c, d);
    map_set(&map, nums[0], 0);
    if (k == 1)
    {
        printf("%d\n", nums[0]);
        free(nums);
        map_free(&map);
        return 0;
    }

    for (int64_t i = 1; i < d + 2; ++i)
    {
        nums[i] = func(nums[i - 1], b, c, d);
        if (i == k - 1)
        {
            printf("%d\n", nums[i]);
            break;
        }

        int mpg = map_get(&map, nums[i]);

        if (mpg == -1)
        {
            map_set(&map, nums[i], i);
        }
        else
        {
            int id = mpg + ((k - 1 - mpg) % (i - mpg));
            printf("%d\n", nums[id]);
            break;
        }
    }

    free(nums);
    map_free(&map);

    return 0;
}
