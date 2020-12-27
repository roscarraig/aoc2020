#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b)
{
  return (*(int*) a - *(int*)b);
}

int part1(int *numbers, int count)
{
  int i, j;

  for(i = 0; i < count - 1; i++)
    for(j = i + 1; j < count; j++)
    {
      int v = numbers[i] + numbers[j];

      if(v == 2020)
        return(numbers[i] * numbers[j]);

      if(v > 2020)
        break;
    }
  return(-1);
}

int part2(int *numbers, int count)
{
  int i, j, k;

  for(i = 0; i < count - 2; i++)
    for(j = i + 1; j < count - 1 && 2020 >= numbers[j + 1] + numbers[j] + numbers[i]; j++)
      for(k = j + 1; k < count; k++)
      {
        int v = numbers[i] + numbers[j] + numbers[k];

        if(v == 2020)
          return(numbers[i] * numbers[j] * numbers[k]);

        if(v > 2020)
          break;

      }
  return(-1);
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char buffer[256];
  int count = 0, i;
  int *numbers;

  while(!feof(fp))
  {
    fgets(buffer, 256, fp);
    count++;
  }
  rewind(fp);
  numbers = (int *) malloc(count * sizeof(int));
  for(i = 0; i < count; i++)
  {
    fgets(buffer, 256, fp);
    sscanf(buffer, "%d\n", &(numbers[i]));
  }
  qsort(numbers, count, sizeof(int), cmp);
  printf("Part 1: %d\n", part1(numbers, count));
  printf("Part 2: %d\n", part2(numbers, count));
}
