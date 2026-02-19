#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main() {
  int t;
  scanf("%d", &t);

  char s[101];
  for (int i = 0; i < t; ++i) {
    scanf("%100s", s);

    int s_len = strlen(s);
    int ok = 0;
    if (s_len % 2 != 0)
      ok = 1;
    else {
      for (int j = 0; j < s_len / 2; ++j) {
        if (s[j] != s[j + s_len / 2]) {
          ok = 1;
          break;
        }
      }
    }
    printf(ok ? "NO\n" : "YES\n");
  }

  return 0;
}