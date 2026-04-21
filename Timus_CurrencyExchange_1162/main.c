#include <stdio.h>
#include <stdlib.h>

int bellman_ford(
    int N, int M, int S, double V,
    int *A, int *B,
    double *RAB, double *CAB,
    double *RBA, double *CBA
)
{
    double *dist = (double*) calloc((size_t) N + 1, sizeof(double));
    if (dist == NULL) return 0;

    dist[S] = V;

    for (int k = 0; k < N; k++) 
    {
        int changed = 0;

        for (int i = 0; i < M; i++) 
        {
            if (dist[A[i]] > CAB[i]) 
            {
                double x = (dist[A[i]] - CAB[i]) * RAB[i];
                if (x > dist[B[i]]) 
                {
                    dist[B[i]] = x;
                    changed = 1;
                    if (k == N - 1) 
                    {
                        free(dist);
                        return 1;
                    }
                }
            }

            if (dist[B[i]] > CBA[i]) 
            {
                double x = (dist[B[i]] - CBA[i]) * RBA[i];
                if (x > dist[A[i]]) 
                {
                    dist[A[i]] = x;
                    changed = 1;
                    if (k == N - 1) 
                    {
                        free(dist);
                        return 1;
                    }
                }
            }
        }

        if (!changed) break;
    }

    free(dist);
    return 0;
}

int main(void)
{
    int N, M, S;
    double V;
    if (scanf("%d %d %d %lf", &N, &M, &S, &V) != 4) return 1;

    int *A = (int *) malloc(M * sizeof(int));
    int *B = (int *) malloc(M * sizeof(int));
    double *RAB = (double *) malloc(M * sizeof(double));
    double *CAB = (double *) malloc(M * sizeof(double));
    double *RBA = (double *) malloc(M * sizeof(double));
    double *CBA = (double *) malloc(M * sizeof(double));
    if (A == NULL || B == NULL || RAB == NULL || CAB == NULL || RBA == NULL || CBA == NULL) 
    {
        free(A);
        free(B);
        free(RAB);
        free(CAB);
        free(RBA);
        free(CBA);
        return 1;
    }

    for (int i = 0; i < M; i++) 
    {
        if (scanf("%d %d %lf %lf %lf %lf", &A[i], &B[i], &RAB[i], &CAB[i], &RBA[i], &CBA[i]) != 6) 
        {
            free(A);
            free(B);
            free(RAB);
            free(CAB);
            free(RBA);
            free(CBA);
            return 1;
        }
    }

    puts(bellman_ford(N, M, S, V, A, B, RAB, CAB, RBA, CBA) ? "YES" : "NO");

    free(A);
    free(B);
    free(RAB);
    free(CAB);
    free(RBA);
    free(CBA);

    return 0;
}
