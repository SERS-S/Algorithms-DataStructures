#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
  int tmp = *b;
  *b = *a;
  *a = tmp;
}

int partition(int* arr, int low, int high) {
  int pivot = arr[high];
  int i = low - 1;

  for (int j = low; j < high; ++j) {
    if (arr[j] < pivot) {
      ++i;
      swap(&arr[i], &arr[j]);
    }
  }

  swap(&arr[i + 1], &arr[high]);
  return i + 1;
}

void quick_sort(int* arr, int low, int high) {
  if (low < high) {
    int pivet = partition(arr, low, high);

    quick_sort(arr, low, pivet - 1);
    quick_sort(arr, pivet + 1, high);
  }
}

int main(void) {
  int n, k;
  if (scanf("%d %d", &n, &k) != 2) {
    return 1;
  }

  int* prices = (int*)malloc((size_t)n * sizeof(int));
  if (!prices) {
    return 1;
  }
  for (int i = 0; i < n; ++i) {
    if (scanf("%d", &prices[i]) != 1) {
      free(prices);
      return 1;
    }
  }

  quick_sort(prices, 0, n - 1);

  int sum = 0;

  int t = 1;
  for (int i = n - 1; i >= 0; --i) {
    if (t % k != 0) {
      sum += prices[i];
    }
    ++t;
  }

  printf("%d\n", sum);

  free(prices);
  return 0;
}
