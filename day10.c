#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b)
{
  return (*(int*) a - *(int*)b);
}

void *malset(size_t n)
{
  void *result = malloc(n);
  memset(result, 0, n);
  return(result);
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[64];
  int  count = 0, *numbers, i = 0, j, diffcount[3];
  long *counts;

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

  /* Count the lines */

  while(!feof(fp) && fgets(buffer, 64, fp))
    count++;

  rewind(fp);

  /* Load the numbers */

  numbers = malset(sizeof(int) * count + 2);
  counts = malset(sizeof(long) * count);

  while(!feof(fp) && fgets(buffer, 64, fp))
    sscanf(buffer, "%d", &(numbers[++i]));

  numbers[count + 1] = 10000;
  qsort(numbers, count + 2, sizeof(int), cmp);
  memset(diffcount, 0, sizeof(int) * 3);
  numbers[count + 1] = numbers[count] + 3;

  for(i = 1; i < count + 2; i++)
    diffcount[numbers[i] - numbers[i - 1] - 1]++;

  for(i = 0 ; i < 3; i ++)
    printf("%d: %d\n", i + 1, diffcount[i]);
  printf("Part 1: %d\n", diffcount[0] * diffcount[2]);

  counts[0] = 1;

  for(i = 0; i < count + 1; i++)
    for(j = 1; j < 4 && i + j < count + 1 && numbers[i + j] < numbers[i] + 4; j++)
      counts[i + j] += counts[i];

  printf("Part 2: %ld\n", counts[count]);
}
