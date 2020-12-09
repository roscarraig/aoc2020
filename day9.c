#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void findmatch(int *numbers, int pos, int precount)
{
  int i, j, sum;

  for(i = pos - precount; i < pos; i++)
    if(numbers[i] < numbers[pos])
      for(j = i + 1; j < pos; j++)
        if(numbers[i] + numbers[j] == numbers[pos])
          return;
  printf("%d\n", numbers[pos]);
  i = 0;
  sum = numbers[i];
  j = 0;
  while(j < pos)
  {
    while(sum < numbers[pos] && j < pos - 1)
      sum += numbers[++j];
    if(sum == numbers[pos])
    {
      int low = numbers[i], high = numbers[i];

      while(i++ < j)
      {
        if(numbers[i] < low)
          low = numbers[i];
        if(numbers[i] > high)
          high = numbers[i];
      }
      printf("%d\n", low + high);
      exit(0);
    }
    while(sum > numbers[pos])
      sum -= numbers[i++];
  }
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[64];
  int  precount, count = 0, *numbers, i = 0;

  if(argc < 3)
  {
    printf("%s <filename> <count>\n", argv[0]);
    exit(1);
  }
  if(!(fp = fopen(argv[1], "r")))
  {
    printf("Unable to open %s\n", argv[1]);
    exit(2);
  }
  sscanf(argv[2], "%d", &precount);
  while(!feof(fp) && fgets(buffer, 64, fp))
    count++;
  rewind(fp);
  numbers = malloc(sizeof(int) * count);
  while(!feof(fp) && fgets(buffer, 64, fp))
    sscanf(buffer, "%d", &(numbers[i++]));
  for(i = precount; i < count; i++)
    findmatch(numbers, i, precount);
}
