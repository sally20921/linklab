#include <stdlib.h>

void foo(void)
{
  void *ptr = malloc(1000);
}

void main(void)
{
  void *ptr = malloc(100);
  foo();
}
