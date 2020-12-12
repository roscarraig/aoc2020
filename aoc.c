#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Allocate and zero data */

void *malset(size_t n)
{
  void *result = malloc(n);

  memset(result, 0, n);
  return(result);
}

/* Check arguments and open data */

FILE *open_data(int argc, char *argv[])
{
  FILE *fp;

  /* Arg checks */

  if(argc < 2)
  {
    printf("%s <filename>\n", argv[0]);
    exit(1);
  }
  if(!(fp = fopen(argv[1], "r")))
  {
    printf("Unable to open %s\n", argv[1]);
    exit(2);
  }
  return(fp);
}
