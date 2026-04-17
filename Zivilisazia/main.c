#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define INF INT_MAX

// // // Min-Heap // // //

// Structs
typedef struct HeapNode
{
    int dist;
    int x;
    int y;
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
    if (a.dist != b.dist) return a.dist < b.dist;
    return a.x < b.x || (a.x == b.x && a.y < b.y);
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
    capacity = capacity > 0 ? capacity : 1;
    MHP *heap = (MHP*) malloc(sizeof(MHP));
    heap->data = (HPN*) malloc((size_t) capacity * sizeof(HPN));
    if (!heap || !heap->data) exit(1);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void hp_destroy(MHP *heap)
{
    free(heap->data);
    free(heap);
}

void hp_insert(MHP *heap, int dist, int x, int y)
{
    if (heap->size == heap->capacity)
    {
        int new_capacity = heap->capacity * 2;
        HPN *new_min_heap = (HPN*) realloc(heap->data, (size_t) new_capacity * sizeof(HPN));
        if (!new_min_heap) exit(1);
        heap->data = new_min_heap;
        heap->capacity = new_capacity;
    }

    heap->data[heap->size].dist = dist;
    heap->data[heap->size].x = x;
    heap->data[heap->size].y = y;
    sift_up(heap, heap->size);
    heap->size++;
}

HPN hp_get_min(MHP *heap)
{
    HPN bad = {-1, -1, -1};
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

// // // Dijkstra // // //

typedef struct
{
    int x;
    int y;
} Point;

int cell_value(char c)
{
    if (c == '.') return 1;
    else if (c == 'W') return 2;
    else if (c == '#') return INF;
    else return 0;
}

void dijkstra(
    char **map, 
    int N, int M, 
    Point start, Point end, 
    int *out, char **path
)
{
    int dist[N][M];
    MHP *heap = hp_create(N * M);
    char dir[N][M];
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            dist[i][j] = INF;
            dir[i][j] = '\0';
        }
    }
    dist[start.x][start.y] = 0;
    hp_insert(heap, 0, start.x, start.y);

    while (heap->size > 0)
    {
        HPN node = hp_get_min(heap);
        if (hp_extract_idx(heap, 0) == -1) exit(1);

        if (node.dist != dist[node.x][node.y]) 
        {
            continue;
        }

        int best_y = node.x;
        int best_x = node.y;        

        if (best_x == -1 || dist[best_y][best_x] == INF) break;
        if (best_y == end.x && best_x == end.y)
        {
            *out = dist[best_y][best_x];
            
            int path_len = 0;
            int path_cap = 0;
            *path = (char*) malloc((size_t) (path_len + 1) * sizeof(char));
            if (!*path) exit(1);
            int cy = best_y, cx = best_x;
            while (cy != start.x || cx != start.y)
            {
                if (path_len + 2 > path_cap)
                {
                    int new_cap = path_cap ? path_cap * 2 : 8;
                    char *new_path = (char*) realloc(*path, (size_t) new_cap * sizeof(char));
                    if (!new_path) exit(1);
                    *path = new_path;
                    path_cap = new_cap;
                }

                char d = dir[cy][cx];

                if (d == 'N')
                {
                    (*path)[path_len] = 'N';
                    cy++;
                }
                else if (d == 'S') 
                {
                    (*path)[path_len] = 'S';
                    cy--;
                }
                else if (d == 'W') 
                {
                    (*path)[path_len] = 'W';
                    cx++;
                }
                else if (d == 'E') 
                {
                    (*path)[path_len] = 'E';
                    cx--;
                }
                path_len++;
            }
            (*path)[path_len] = '\0';

            hp_destroy(heap);

            return;
        }

        int dy[4] = {-1, 1, 0, 0};
        int dx[4] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i)
        {
            int ny = best_y + dy[i];
            int nx = best_x + dx[i];

            if (ny < 0 || ny >= N || nx < 0 || nx >= M) continue;
            if (map[ny][nx] == '#') continue;
            if (dist[best_y][best_x] + cell_value(map[ny][nx]) < dist[ny][nx])
            {
                dist[ny][nx] = dist[best_y][best_x] + cell_value(map[ny][nx]);                
                dir[ny][nx] = "NSWE"[i];
                hp_insert(heap, dist[ny][nx], ny, nx);
            }
        }
    }

    *out = INF;
    hp_destroy(heap);
    return;
}

// // // Main logic // // //

int main(void)
{
    int N, M;
    Point start, end;
    if (scanf("%d %d %d %d %d %d", &N, &M, &start.x, &start.y, &end.x, &end.y) != 6) { return 1; }
    start.x--; start.y--;
    end.x--; end.y--;


    char **map = (char**) malloc((size_t) N * sizeof(char*));
    for (int k = 0; k < N; ++k)
    {
        char *line = (char*) malloc((size_t) (M + 1) * sizeof(char));
        if (scanf("%s", line) != 1) { return 1; }
        map[k] = line;
    }

    int out;
    char *path = NULL;
    dijkstra(map, N, M, start, end, &out, &path);

    if (out == INF) printf("-1\n");
    else 
    {
        printf("%d\n", out);
        for (int i = strlen(path) - 1; i >= 0; --i) { printf("%c", path[i]); }
        printf("\n");
    }

    for (int k = 0; k < N; ++k) free(map[k]);
    free(map);
    if (path) free(path);

    return 0;
}
