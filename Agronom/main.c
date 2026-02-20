#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int n;
  scanf("%d", &n);

  int* a = (int*)malloc(n * sizeof(int));
  for (int i = 0; i < n; ++i)
    scanf("%d", &a[i]);

  int mxd = 0;
  int l = 1, r = 1;

  int p = 0;
  for (int i = 2; i < n; ++i) {
    if ((a[i] == a[i - 1]) && (a[i - 1] == a[i - 2])) {
      if (mxd < (i - p)) {
        mxd = i - p;
        l = p + 1;
        r = i;
      }
      p = i - 1;
    }
  }

  if (mxd < (n - p)) {
    mxd = n - p;
    l = p + 1;
    r = n;
  }

  printf("%d %d\n", l, r);

  free(a);
  return 0;
}
