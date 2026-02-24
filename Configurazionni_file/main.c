#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VAL_LEN 10

/* Реализация map */

typedef struct Node {
    char var_name[VAL_LEN + 1];
    int  val;
    struct Node *next;
} ND;

typedef struct {
    ND **buckets;
    size_t bucket_count;
} Map;

size_t map_hash(const char *s, size_t bucket_count)
{
    uint32_t h = 2166136261u;
    while (*s)
    {
        h ^= (unsigned char) (*s++);
        h *= 16777619u;
    }

    return (size_t) (h % bucket_count);
}

void map_init(Map *m, size_t bucket_count)
{
    m->buckets = (ND**) calloc(bucket_count, sizeof(ND*));
    m->bucket_count = bucket_count;

    return;
}

void map_free(Map *m)
{
    for (size_t i = 0; i < m->bucket_count; ++i) 
    {
        ND *cur = m->buckets[i];
        while (cur) 
        {
            ND *next = cur->next;
            free(cur);
            cur = next;
        }
    }

    free(m->buckets);
    m->buckets = NULL;
    m->bucket_count = 0;
}

void map_set(Map *m, char *name, int val)
{
    size_t idx = map_hash(name, m->bucket_count);

    for (ND *cur = m->buckets[idx]; cur != NULL; cur = cur->next)
    {
        if (strcmp(cur->var_name, name) == 0)
        {
            cur->val = val;
            return;
        }
    }

    ND *new_nd = (ND*) malloc(sizeof(ND));
    strncpy(new_nd->var_name, name, VAL_LEN);
    new_nd->val = val;
    new_nd->var_name[VAL_LEN] = '\0';
    new_nd->next = m->buckets[idx];
    m->buckets[idx] = new_nd;

    return;
}

int map_get(Map *m, const char *name)
{
    size_t idx = map_hash(name, m->bucket_count);

    for (ND *cur = m->buckets[idx]; cur != NULL; cur = cur->next)
    {
        if (strcmp(cur->var_name, name) == 0)
        {
            return cur->val;
        }
    }

    return 0;
}

/* Код */

int main(void) 
{
    Map map;
    map_init(&map, 1e5);

    int ct = 0;
    char (*stack_names)[VAL_LEN + 1] = malloc((size_t)1e5 * sizeof(*stack_names));
    int *stack_vl = (int*) malloc(1e5 * sizeof(int));
    int *brd = (int*) malloc(1e5 * sizeof(int));
    int brd_p = 0;

    char s[200005];
    while (scanf("%200000s", s) == 1) 
    {
        if (s[0] == '{' && s[1] == '\0') 
        {
            brd[brd_p++] = ct;
        } 
        else if (s[0] == '}' && s[1] == '\0') 
        {
            if (brd_p > 0)
            {
                int border = brd[--brd_p];
                while (ct > border)
                {
                    --ct;
                    map_set(&map, stack_names[ct], stack_vl[ct]);
                }
            }
        } 
        else 
        {
            char* e = strchr(s, '=');
            if (e == NULL) 
            {
                continue;
            }
            *e = '\0';

            char* lvr = s;
            char* rvr = e + 1;

            if (isdigit((unsigned char)rvr[0]) || (rvr[0] == '-' && isdigit((unsigned char)rvr[1]))) 
            {
                int num = atoi(rvr);

                strncpy(stack_names[ct], lvr, VAL_LEN);
                stack_names[ct][VAL_LEN] = '\0';
                stack_vl[ct] = map_get(&map, lvr);

                map_set(&map, lvr, num);
                ++ct;
            } 
            else 
            {
                char* var2 = rvr;

                int prevrs = map_get(&map, lvr);
                int rs = map_get(&map, var2);
                printf("%d\n", rs);

                strncpy(stack_names[ct], lvr, VAL_LEN);
                stack_names[ct][VAL_LEN] = '\0';
                stack_vl[ct] = prevrs;

                map_set(&map, lvr, rs);
                ++ct;
            }
        }
    }

    return 0;
}
