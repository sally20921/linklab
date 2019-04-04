#include <stdlib.h>

int main(void)
{
  void *a;

  a = malloc(1024);
  free(a);
  free(a);
  free((void*)0x1706e90);

  return 0;
}
