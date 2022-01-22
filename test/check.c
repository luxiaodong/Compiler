
#include <stdio.h>
#include <stdlib.h>

static int codeId = 0;
void check(int expected, int actual) {
  if (expected == actual) {
    printf("code(%d) => %d\n", codeId++, actual);
  }else {
    printf("code(%d) expected %d, but got %d\n", codeId++, expected, actual);
    exit(-1);
  }
}
