#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void findmatch(int *numbers, int pos, int precount)
{
  int i, j, sum;

  /* Look for first, second that sum to the target */

  for(i = pos - precount; i < pos - 1; i++)
    if(numbers[i] < numbers[pos])
      for(j = i + 1; j < pos; j++)
        if(numbers[i] + numbers[j] == numbers[pos])
          return;
  printf("%d\n", numbers[pos]);

  /* Part 2 */

  i = j = 0;
  sum = numbers[0];

  while(j < pos)
  {
    /* Leading edge */

    while(sum < numbers[pos] && j < pos - 1)
      sum += numbers[++j];

    /* Following edge */

    while(sum > numbers[pos])
      sum -= numbers[i++];

    /* Reached it? */ 

    if(sum == numbers[pos])
    {
      /* Find the bounds */

      int low = numbers[i], high = numbers[i];

      while(i++ < j)
      {
        if(numbers[i] < low)
          low = numbers[i];
        if(numbers[i] > high)
          high = numbers[i];
      }
      /* ....and done */

      printf("%d\n", low + high);
      exit(0);
    }
  }
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[64];
  int  precount, count = 0, *numbers, i = 0;

  /* Arg checks */

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

  /* Count the lines */

  while(!feof(fp) && fgets(buffer, 64, fp))
    count++;

  rewind(fp);

  /* Load the numbers */

  numbers = malloc(sizeof(int) * count);

  while(!feof(fp) && fgets(buffer, 64, fp))
    sscanf(buffer, "%d", &(numbers[i++]));

  /* Search the numbers for the solution */

  for(i = precount; i < count; i++)
    findmatch(numbers, i, precount);
}
