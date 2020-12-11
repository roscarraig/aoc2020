#include <stdlib.h>
#include <string.h>

void *malset(size_t n)
{
  void *result = malloc(n);

  memset(result, 0, n);
  return(result);
}

