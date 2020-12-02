#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b)
{
  return (*(int*) a - *(int*)b);
}

int multsearch(int *numbers, int target, int count, int start, int terms)
{
  int i;
  int divm = target / terms;

  for (i = start; (i < (count + 1 - terms)) && (numbers[i] <= divm); i++)
  {
    if (terms == 1)
    {
      if (numbers[i] == target)
      {
        printf("%d ", numbers[i]);
        return(target);
      }
      else if (numbers[i] > target)
        return(-1);
    }
    else
    {
      int j = multsearch(numbers, target - numbers[i], count, i + 1, terms - 1);
      if(j > 0)
      {
        printf("* %d ", numbers[i]);
        return(numbers[i] * j);
      }
    }
  }
  return(-1);
}

int main(int argc, char **argv)
{
  FILE *fp;
  char buffer[512];
  int  i, h, count = 0;
  char m;
  int *numbers;
  int target = 2020;

  if(argc == 1)
  {
    printf("%s <filename>\n", argv[0]);
    exit(1);
  }
  if(!(fp = fopen(argv[1], "r")))
  {
    printf("Unable to open %s\n", argv[1]);
    exit(2);
  }
  while(!feof(fp) && fgets(buffer, 256, fp))
    count++;
  rewind(fp);
  numbers = (int *) malloc(count * sizeof(int));
  for(i = 0; i < count; i++)
  {
    fgets(buffer, 256, fp);
    sscanf(buffer, "%d\n", &(numbers[i]));
  }
  qsort(numbers, count, sizeof(int), cmp);
  printf("= %d\n", multsearch(numbers, target, count, 0, 3));
}
