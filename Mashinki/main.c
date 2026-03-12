#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

// // // Структуры // // //

typedef struct Node
{
    int key, next;
    int h, sz;
    struct Node *l, *r;
} ND;

typedef struct OrderedSet
{
    struct Node *root;
} OS;

static int h(const ND *n) { return n ? n->h : 0; }
static int sz(const ND *n) { return n ? n->sz : 0; }
static int cmp(int next_a, int key_a, int next_b, int key_b)
{
    if (next_a != next_b) return (next_a < next_b) ? -1 : 1;
    if (key_a != key_b) return (key_a < key_b) ? -1 : 1;
    return 0;
}

// // // Методы упорядоченного множества // // //

static void UND(ND *n)
{
    n->h = 1 + max(h(n->l), h(n->r));
    n->sz = 1 + sz(n->l) + sz(n->r);
}

ND* balance(ND *n)
{
    if (!n) return NULL;
    UND(n);

    if (h(n->l) - h(n->r) > 1)
    {
        if (h(n->l->r) > h(n->l->l))
        {
            ND *x = n->l, *y = x->r;
            x->r = y->l;
            y->l = x;
            UND(x);
            UND(y);
            n->l = y;
        }
        ND *y = n, *x = y->l;
        y->l = x->r;
        x->r = y;
        UND(y);
        UND(x);
        return x;
    }

    if (h(n->r) - h(n->l) > 1) 
    {
        if (h(n->r->l) > h(n->r->r)) 
        {
            ND *x = n->r, *y = x->l;
            x->l = y->r;
            y->r = x;
            UND(x);
            UND(y);
            n->r = y;
        }
        {
            ND *x = n, *y = x->r;
            x->r = y->l;
            y->l = x;
            UND(x);
            UND(y);
            return y;
        }
    }

    return n;
}

void os_init(OS *s)
{
    s->root = NULL;
}

void os_destroy(OS *s)
{
    ND *n = s->root;
    while (n)
    {
        if (n->l)
        {
            ND *x = n->l;
            n->l = x->r;
            x->r = n;
            n = x;
        }
        else
        {
            ND *x = n;
            n = n->r;
            free(x);
        }
    }
    s->root = NULL;
}

bool os_contains(OS *s, int key)
{
    ND *n = s->root;
    while (n)
    {
        if (key < n->key) n = n->l;
        else if (key > n->key) n = n->r;
        else return true;
    }
    return false;
}

ND* os_kth(OS *s, int k)
{
    ND *n = s->root;
    if (!n) return NULL;
    if (k < 0 || k >= n->sz) return NULL;

    while (n)
    {
        int left_sz = sz(n->l);
        if (k < left_sz)
        {
            n = n->l;
        }
        else if (k > left_sz)
        {
            k -= left_sz + 1;
            n = n->r;
        }
        else return n;
    }

    return NULL;
}

int os_size(OS *s) { return s->root ? s->root->sz : 0; }

bool os_insert(OS *s, int key, int next)
{
    ND ***path = NULL, **p = &s->root;
    int top = 0, cap = 0;

    while (*p)
    {
        int c = cmp(next, key, (*p)->next, (*p)->key);
        if (c == 0)
        {
            free(path);
            return false;
        }
        if (top == cap)
        {
            cap = cap ? cap * 2 : 8;
            path = realloc(path, cap * sizeof(*path));
            if (!path) exit(1);
        }
        path[top++] = p;
        p = (c < 0) ? &(*p)->l : &(*p)->r;
    }

    *p = malloc(sizeof(**p));
    if (!*p) exit(1);
    (*p)->key = key;
    (*p)->next = next;
    (*p)->h = 1;
    (*p)->sz = 1;
    (*p)->l = (*p)->r = NULL;

    if (top == cap)
    {
        cap = cap ? cap * 2 : 8;
        path = realloc(path, cap * sizeof(*path));
        if (!path) exit(1);
    }
    path[top++] = p;

    while (top)
    {
        p = path[--top];
        *p = balance(*p);
    }

    free(path);
    return true;
}

bool os_erase(OS *s, int key, int next)
{
    ND ***path = NULL, **p = &s->root;
    int top = 0, cap = 0;

    while (*p)
    {
        int c = cmp(next, key, (*p)->next, (*p)->key);
        if (c == 0) break;
        if (top == cap)
        {
            cap = cap ? cap * 2 : 8;
            path = realloc(path, cap * sizeof(*path));
            if (!path) exit(1);
        }
        path[top++] = p;
        p = (c < 0) ? &(*p)->l : &(*p)->r;
    }

    if (!*p)
    {
        free(path);
        return false;
    }

    if (top == cap) 
    {
        cap = cap ? cap * 2 : 8;
        path = realloc(path, cap * sizeof(*path));
        if (!path) exit(1);
    }
    path[top++] = p;

    if ((*p)->l && (*p)->r)
    {
        ND **q = &(*p)->r;
        while ((*q)->l)
        {
            if (top == cap) 
            {
                cap = cap ? cap * 2 : 8;
                path = realloc(path, cap * sizeof(*path));
                if (!path) exit(1);
            }
            path[top++] = q;
            q = &(*q)->l;
        }
        if (top == cap) 
        {
            cap = cap ? cap * 2 : 8;
            path = realloc(path, cap * sizeof(*path));
            if (!path) exit(1);
        }
        path[top++] = q;

        (*p)->key = (*q)->key;
        (*p)->next = (*q)->next;
        ND *t = *q;
        *q = t->r;
        free(t);
    }
    else
    {
        ND *t = *p;
        *p = t->l ? t->l : t->r;
        free(t);
    }

    while (top)
    {
        p = path[--top];
        *p = balance(*p);
    }

    free(path);
    return true;
}

// // // Основной код // // //

int main(void)
{
    int N, K, P;
    if (scanf("%d %d %d", &N, &K, &P) != 3)
    {
        return 1;
    }

    int *p = (int*) malloc((size_t) P * sizeof(int));
    if (!p) return 1;
    for (int i = 0; i < P; ++i)
    {
        if (scanf("%d", &p[i]) != 1) { free(p); return 1; }
    }

    int *next = (int*) malloc((size_t) P * sizeof(int));
    int *last = (int*) malloc((size_t) (N + 1) * sizeof(int));
    if (!next || !last) { free(p); free(next); free(last); return 1; }
    for (int i = 0; i <= N; ++i) last[i] = INT_MAX;

    for (int i = P - 1; i >= 0; --i)
    {
        next[i] = last[p[i]];
        last[p[i]] = i;
    }

    bool *on_floor = (bool*) malloc((size_t) (N + 1) * sizeof(bool));
    if (!on_floor) { free(p); free(next); free(last); return 1; }
    memset(on_floor, 0, (size_t) (N + 1) * sizeof(bool));

    OS ord_s;
    os_init(&ord_s);

    int ct_v = 0, ans = 0;

    for (int i = 0; i < P; ++i)
    {
        int x = p[i];

        if (on_floor[x])
        {
           if (!os_erase(&ord_s, x, i)) return 1;
           if (!os_insert(&ord_s, x, next[i])) return 1;
        }
        else
        {
            ++ans;
            if (ct_v == K)
            {
                ND *last_v_by_next = os_kth(&ord_s, ct_v - 1);
                if (!last_v_by_next) return 1;
                
                int kicked_key = last_v_by_next->key;
                int kicked_next = last_v_by_next->next;
                if (!os_erase(&ord_s, kicked_key, kicked_next)) return 1;
                on_floor[kicked_key] = 0;
            }
            else
            {
                ++ct_v;
            }
            if (!os_insert(&ord_s, x, next[i])) return 1;
            on_floor[x] = 1;
        }
    }

    printf("%d\n", ans);

    free(p);
    free(next);
    free(last);
    free(on_floor);
    os_destroy(&ord_s);

    return 0;
}
