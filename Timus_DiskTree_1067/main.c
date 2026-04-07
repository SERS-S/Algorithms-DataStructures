#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// // // Ordered Tree // // //

typedef struct Node
{
    char name[9];
    struct Node **children;
    int child_count;
    int child_cap;
} ND;

typedef struct OrderedTree
{
    ND *root;
} ORT;

// Special functions
int comparator(const void *a, const void *b) 
{
    const ND* const *lhs = a;
    const ND* const *rhs = b;
    return strcmp((*lhs)->name, (*rhs)->name);
}

int check_name_exist(ND **nodes, int count, const char *name)
{
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(nodes[i]->name, name) == 0) return i;
    }
    return -1;
}

void dfs(ND *node, int depth)
{
    for (int i = 0; i < depth; ++i) printf(" ");

    printf("%s\n", node->name);

    for (int i = 0; i < node->child_count; ++i)
    {
        dfs(node->children[i], depth + 1);
    }
}

void sort_tree(ND *node)
{
    qsort(node->children, (size_t) node->child_count, sizeof(ND*), comparator);

    for (int i = 0; i < node->child_count; ++i)
    {
        sort_tree(node->children[i]);
    }
}

// Ordered tree functions
void ort_init(ORT *t)
{
    t->root = malloc(sizeof(ND));
    if (t->root == NULL) exit(1);

    t->root->name[0] = '\0';
    t->root->children = NULL;
    t->root->child_count = 0;
    t->root->child_cap = 0;
}

void ort_destroy(ORT *t)
{
    for (int i = 0; i < t->root->child_count; ++i)
    {
        ort_destroy(&(ORT) { .root = t->root->children[i] });
    }
    free(t->root->children);
    free(t->root);
}

int ort_insert(ORT *t, const char *path)
{
    ND *nd = t->root;

    const char *p = path;
    while (*p != '\0')
    {
        const char *sep = strchr(p, '\\');
        int len = sep ? (int) (sep - p) : (int) strlen(p);

        char part[9];
        memcpy(part, p, (size_t) len);
        part[len] = '\0';

        int idx = check_name_exist(nd->children, nd->child_count, part);
        if (idx == -1)
        {
            if (nd->child_count == nd->child_cap)
            {
                int new_cap = nd->child_cap ? nd->child_cap * 2 : 8;
                ND **new_children = realloc(nd->children, (size_t) new_cap * sizeof(ND *));
                if (!new_children) return -1;

                nd->children = new_children;
                nd->child_cap = new_cap;
            }

            ND *new_node = malloc(sizeof(ND));
            if (!new_node) return -1;
            strncpy(new_node->name, part, 9);
            new_node->children = NULL;
            new_node->child_count = 0;
            new_node->child_cap = 0;
            nd->children[nd->child_count++] = new_node;

            nd = new_node;
        }
        else
        {
            nd = nd->children[idx];
        }


        if (sep == NULL) break;
        p = sep + 1;
    }

    return 0;
}

void ort_output(ORT *t)
{
    sort_tree(t->root);
    
    for (int i = 0; i < t->root->child_count; ++i)
    {
        dfs(t->root->children[i], 0);
    }
}
// // // Main logic // // //

int main(void)
{
    int N;
    if (scanf("%d", &N) != 1) return 1;

    ORT ort_tree;
    ort_init(&ort_tree);

    for (int i = 0; i < N; ++i)
    {
        char path[80];
        if (scanf("%s", path) != 1) return 1;
        if (ort_insert(&ort_tree, path) != 0) return 1;
    }

    ort_output(&ort_tree);

    ort_destroy(&ort_tree);

    return 0;
}