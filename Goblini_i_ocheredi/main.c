#include <stdio.h>
#include <stdlib.h>

// // // Deque implementation // // //

// Structs
enum
{
    DQ_BLOCK_SIZE = 256,
    DQ_INITIAL_BLOCKS = 8
};

typedef struct
{
    int **map;
    size_t map_size;
    size_t head;
    size_t size;
} Deque;

// Special functions
static size_t deque_capacity(const Deque *dq)
{
    return dq->map_size * (size_t) DQ_BLOCK_SIZE;
}

static void deque_free_map(int **map, size_t map_size) 
{
    if (!map) return;
    for (size_t i = 0; i < map_size; ++i) free(map[i]);
    free(map);
}

static int deque_write_slot(Deque *dq, size_t slot, int value)
{
    size_t block = slot / DQ_BLOCK_SIZE;
    size_t offset = slot % DQ_BLOCK_SIZE;

    if (dq->map[block] == NULL) 
    {
        dq->map[block] = (int*) malloc(DQ_BLOCK_SIZE * sizeof(int));
        if (!dq->map[block]) return -1;
    }

    dq->map[block][offset] = value;
    return 0;
}

static int deque_read_slot(const Deque *dq, size_t slot) 
{
    size_t block = slot / DQ_BLOCK_SIZE;
    size_t offset = slot % DQ_BLOCK_SIZE;
    return dq->map[block][offset];
}

static int deque_grow(Deque *dq)
{
    size_t old_map_size = dq->map_size;
    size_t old_cap = deque_capacity(dq);
    int **old_map = dq->map;
    size_t old_head = dq->head;
    size_t old_size = dq->size;

    size_t new_map_size = (old_map_size == 0 ? DQ_INITIAL_BLOCKS : old_map_size * 2);
    int **new_map = (int**) calloc(new_map_size, sizeof(int*));
    if (!new_map) return -1;

    size_t new_cap = new_map_size * (size_t) DQ_BLOCK_SIZE;

    size_t new_head = (new_cap - old_size) / 2;
    for (size_t i = 0; i < old_size; ++i)
    {
        size_t old_slot = (old_head + i) % old_cap;
        size_t old_block = old_slot / DQ_BLOCK_SIZE;
        size_t old_off = old_slot % DQ_BLOCK_SIZE;

        int value = old_map[old_block][old_off];

        size_t new_slot = new_head + i;
        size_t new_block = new_slot / DQ_BLOCK_SIZE;
        size_t new_off = new_slot % DQ_BLOCK_SIZE;

        if (new_map[new_block] == NULL) 
        {
            new_map[new_block] = (int*) malloc(DQ_BLOCK_SIZE * sizeof(int));
            if (!new_map[new_block]) 
            {
                deque_free_map(new_map, new_map_size);
                return -1;
            }
        }

        new_map[new_block][new_off] = value;
    }

    deque_free_map(old_map, old_map_size);

    dq->map = new_map;
    dq->map_size = new_map_size;
    dq->head = new_head;
    dq->size = old_size;

    return 0;
}

// Major functions

static int deque_init(Deque *dq)
{
    dq->map = (int**) calloc(DQ_INITIAL_BLOCKS, sizeof(int*));
    if (!dq->map) return -1;

    dq->map_size = DQ_INITIAL_BLOCKS;
    dq->size = 0;
    dq->head = deque_capacity(dq) / 2;
    return 0;
}

static void deque_destroy(Deque *dq) 
{
    deque_free_map(dq->map, dq->map_size);
    dq->map = NULL;
    dq->map_size = 0;
    dq->head = 0;
    dq->size = 0;
}

static int deque_push_back(Deque *dq, int value)
{
    if (dq->size == deque_capacity(dq))
    {
        if (deque_grow(dq) != 0) return -1;
    }

    size_t cap = deque_capacity(dq);
    size_t slot = (dq->head + dq->size) % cap;

    if (deque_write_slot(dq, slot, value) != 0) return -1;
    dq->size++;
    return 0;
}

static int deque_push_front(Deque *dq, int value)
{
    if (dq->size == deque_capacity(dq))
    {
        if (deque_grow(dq) != 0) return -1;
    }

    size_t cap = deque_capacity(dq);
    dq->head = (dq->head + cap - 1) % cap;

    if (deque_write_slot(dq, dq->head, value) != 0) return -1;
    dq->size++;
    return 0;
}

static int deque_pop_front(Deque *dq, int *out) 
{
    if (dq->size == 0) return -1;

    size_t cap = deque_capacity(dq);
    *out = deque_read_slot(dq, dq->head);
    dq->head = (dq->head + 1) % cap;
    dq->size--;
    return 0;
}

static int deque_pop_back(Deque *dq, int *out) 
{
    if (dq->size == 0) return -1;

    size_t cap = deque_capacity(dq);
    size_t slot = (dq->head + dq->size - 1) % cap;
    *out = deque_read_slot(dq, slot);
    dq->size--;
    return 0;
}

static int rebalance(Deque *left, Deque *right)
{
    int value;

    if (left->size < right->size)
    {
        if (deque_pop_front(right, &value) != 0) return -1;
        if (deque_push_back(left, value) != 0) return -1;
    }
    else if (left->size > right->size + 1)
    {
        if (deque_pop_back(left, &value) != 0) return -1;
        if (deque_push_front(right, value) != 0) return -1;
    }

    return 0;
}

// // // Main logic // // //

int main(void)
{
    int N;
    if (scanf("%d", &N) != 1) return 1;

    Deque ldq;
    Deque rdq;
    if (deque_init(&ldq) != 0) return 1;
    if (deque_init(&rdq) != 0)
    {
        deque_destroy(&ldq);
        return 1;
    }

    for (int i = 0; i < N; ++i)
    {
        char cmd;
        if (scanf(" %c", &cmd) != 1)
        {
            deque_destroy(&ldq);
            deque_destroy(&rdq);
            return 1;
        }

        if (cmd == '-')
        {
            int out;
            if (deque_pop_front(&ldq, &out) != 0)
            {
                deque_destroy(&ldq);
                deque_destroy(&rdq);
                return 1;
            }
            if (rebalance(&ldq, &rdq) != 0)
            {
                deque_destroy(&ldq);
                deque_destroy(&rdq);
                return 1;
            }

            fprintf(stdout, "%d\n", out);
        }
        else
        {
            int value;
            if (scanf("%d", &value) != 1)
            {
                deque_destroy(&ldq);
                deque_destroy(&rdq);
                return 1;
            }

            if (cmd == '+')
            {
                if (deque_push_back(&rdq, value) != 0)
                {
                    deque_destroy(&ldq);
                    deque_destroy(&rdq);
                    return 1;
                }
            }
            else if (cmd == '*')
            {
                if (ldq.size > rdq.size)
                {
                    if (deque_push_front(&rdq, value) != 0)
                    {
                        deque_destroy(&ldq);
                        deque_destroy(&rdq);
                        return 1;
                    }
                }
                else
                {
                    if (deque_push_back(&ldq, value) != 0)
                    {
                        deque_destroy(&ldq);
                        deque_destroy(&rdq);
                        return 1;
                    }
                }
            }
            else
            {
                deque_destroy(&ldq);
                deque_destroy(&rdq);
                return 1;
            }

            if (rebalance(&ldq, &rdq) != 0)
            {
                deque_destroy(&ldq);
                deque_destroy(&rdq);
                return 1;
            }
        }
    }

    deque_destroy(&ldq);
    deque_destroy(&rdq);

    return 0;
}
