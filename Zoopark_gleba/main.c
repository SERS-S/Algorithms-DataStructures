#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // ABba // AaBb
  char a[100000 + 1];
  scanf("%100000s", a);

  int n = (int)strlen(a) / 2;

  char stack[100000 + 1];
  int stack_id[100000 + 1];
  int stack_is_trap[100000 + 1];
  int stk_p = -1;
  int zag_ct = 0, strok_ct = 0;
  int ans[100000 + 1];

  for (int i = 0; i < n * 2; ++i) {
    int cur_is_trap = (a[i] >= 'A' && a[i] <= 'Z');
    int cur_id = cur_is_trap ? ++zag_ct : ++strok_ct;

    if (stk_p >= 0) {
      if (abs(stack[stk_p] - a[i]) == 32) {
        if (stack_is_trap[stk_p]) {
          ans[stack_id[stk_p]] = cur_id;
        } else {
          ans[cur_id] = stack_id[stk_p];
        }
        --stk_p;
      } else {
        stack[++stk_p] = a[i];
        stack_id[stk_p] = cur_id;
        stack_is_trap[stk_p] = cur_is_trap;
      }
    } else {
      stack[++stk_p] = a[i];
      stack_id[stk_p] = cur_id;
      stack_is_trap[stk_p] = cur_is_trap;
    }
    // printf("i: %d, Stack_p: %d, stack_el: %d, a[i]: %d\n", i, stk_p,
    // stack[stk_p], a[i]);
  }

  if (stk_p == -1) {
    printf("Possible\n");
    for (int i = 1; i <= n; ++i) {
      if (i > 1)
        printf(" ");
      printf("%d", ans[i]);
    }
    printf("\n");
  } else {
    printf("Impossible\n");
  }

  return 0;
}
