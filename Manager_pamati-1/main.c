#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TN
{
    int l;
    int r;
    int order;
    int mx;
    uint32_t prio;
    struct TN *left;
    struct TN *right;
} TN;

typedef struct
{
    TN *root;
    uint32_t rng;
} Treap;

static uint32_t treap_rand(Treap *t)
{
    t->rng ^= t->rng << 13;
    t->rng ^= t->rng >> 17;
    t->rng ^= t->rng << 5;
    return t->rng;
}

static int node_len(const TN *n)
{
    return n->r - n->l + 1;
}

static int node_mx(const TN *n)
{
    return n ? n->mx : 0;
}

static void pull(TN *n)
{
    if (!n) return;

    n->mx = node_len(n);
    if (node_mx(n->left) > n->mx) n->mx = node_mx(n->left);
    if (node_mx(n->right) > n->mx) n->mx = node_mx(n->right);
}

static TN *treap_new_node(Treap *t, int l, int r, int order)
{
    TN *n = (TN *)malloc(sizeof(*n));
    if (!n) return NULL;

    n->l = l;
    n->r = r;
    n->order = order;
    n->mx = r - l + 1;
    n->prio = treap_rand(t);
    n->left = NULL;
    n->right = NULL;
    return n;
}

static TN *rotate_right(TN *p)
{
    TN *q = p->left;
    p->left = q->right;
    q->right = p;
    pull(p);
    pull(q);
    return q;
}

static TN *rotate_left(TN *p)
{
    TN *q = p->right;
    p->right = q->left;
    q->left = p;
    pull(p);
    pull(q);
    return q;
}

static void destroy_rec(TN *n)
{
    if (!n) return;
    destroy_rec(n->left);
    destroy_rec(n->right);
    free(n);
}

static TN *insert_rec(Treap *t, TN *n, int l, int r, int order, bool *inserted)
{
    if (!n)
    {
        *inserted = true;
        return treap_new_node(t, l, r, order);
    }

    if (l < n->l)
    {
        n->left = insert_rec(t, n->left, l, r, order, inserted);
        if (n->left && n->left->prio > n->prio) n = rotate_right(n);
    }
    else if (l > n->l)
    {
        n->right = insert_rec(t, n->right, l, r, order, inserted);
        if (n->right && n->right->prio > n->prio) n = rotate_left(n);
    }
    else
    {
        n->r = r;
        n->order = order;
        *inserted = false;
    }

    pull(n);
    return n;
}

static TN *merge_rec(TN *a, TN *b)
{
    if (!a) return b;
    if (!b) return a;

    if (a->prio > b->prio)
    {
        a->right = merge_rec(a->right, b);
        pull(a);
        return a;
    }

    b->left = merge_rec(a, b->left);
    pull(b);
    return b;
}

static TN *erase_rec(TN *n, int l, bool *erased)
{
    if (!n) return NULL;

    if (l < n->l)
    {
        n->left = erase_rec(n->left, l, erased);
        pull(n);
        return n;
    }
    if (l > n->l)
    {
        n->right = erase_rec(n->right, l, erased);
        pull(n);
        return n;
    }

    *erased = true;
    TN *res = merge_rec(n->left, n->right);
    free(n);
    return res;
}

static TN *find_first_fit(TN *n, int k)
{
    if (!n || n->mx < k) return NULL;
    if (n->left && n->left->mx >= k) return find_first_fit(n->left, k);
    if (node_len(n) >= k) return n;
    return find_first_fit(n->right, k);
}

static TN *find_prev(TN *n, int l)
{
    TN *res = NULL;

    while (n)
    {
        if (n->l < l)
        {
            res = n;
            n = n->right;
        }
        else
        {
            n = n->left;
        }
    }

    return res;
}

static TN *find_next(TN *n, int l)
{
    TN *res = NULL;

    while (n)
    {
        if (n->l > l)
        {
            res = n;
            n = n->left;
        }
        else
        {
            n = n->right;
        }
    }

    return res;
}

static void treap_init(Treap *t)
{
    t->root = NULL;
    t->rng = 2463534242u;
}

static void treap_destroy(Treap *t)
{
    destroy_rec(t->root);
    t->root = NULL;
}

static int treap_insert(Treap *t, int l, int r, int order)
{
    bool inserted = false;
    t->root = insert_rec(t, t->root, l, r, order, &inserted);
    return inserted ? 1 : 0;
}

static int treap_erase(Treap *t, int l)
{
    bool erased = false;
    t->root = erase_rec(t->root, l, &erased);
    return erased ? 1 : 0;
}

int main(void)
{
    int N;
    int M;
    if (scanf("%d %d", &N, &M) != 2) return 1;

    Treap tp;
    treap_init(&tp);
    treap_insert(&tp, 1, N, 0);

    int *alloc_l = (int*) calloc((size_t) M + 1, sizeof(int));
    int *alloc_r = (int*) calloc((size_t) M + 1, sizeof(int));
    char *alive = (char*) calloc((size_t) M + 1, sizeof(char));
    if (!alloc_l || !alloc_r || !alive)
    {
        free(alloc_l);
        free(alloc_r);
        free(alive);
        treap_destroy(&tp);
        return 1;
    }

    for (int i = 1; i <= M; ++i)
    {
        int x;
        if (scanf("%d", &x) != 1)
        {
            free(alloc_l);
            free(alloc_r);
            free(alive);
            treap_destroy(&tp);
            return 1;
        }

        if (x > 0)
        {
            TN *n = find_first_fit(tp.root, x);
            if (!n)
            {
                printf("-1\n");
                continue;
            }

            int l = n->l;
            int r = n->r;
            int rr = l + x - 1;
            printf("%d\n", l);

            alloc_l[i] = l;
            alloc_r[i] = rr;
            alive[i] = 1;

            treap_erase(&tp, l);
            if (rr < r) treap_insert(&tp, rr + 1, r, i);
        }
        else
        {
            int t = -x;
            if (!alive[t]) continue;

            int l = alloc_l[t];
            int r = alloc_r[t];
            TN *pr = find_prev(tp.root, l);
            TN *nx = find_next(tp.root, l);

            if (pr && pr->r + 1 == l)
            {
                l = pr->l;
                treap_erase(&tp, pr->l);
            }
            if (nx && r + 1 == nx->l)
            {
                r = nx->r;
                treap_erase(&tp, nx->l);
            }

            treap_insert(&tp, l, r, t);
            alive[t] = 0;
        }
    }

    free(alloc_l);
    free(alloc_r);
    free(alive);
    treap_destroy(&tp);
    return 0;
}
