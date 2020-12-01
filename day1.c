#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char buffer[256];
  int count = 0, i, j, k;
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
  for(i = 0; i < count - 2; i++)
    for(j = i + 1; j < count - 1; j++)
      for(k = j + 1; k < count; k++)
        if (numbers[i] + numbers[j] + numbers[k] == 2020)
        {
          printf("%d %d %d %d\n", numbers[i], numbers[j], numbers[k], numbers[i] * numbers[j] * numbers[k]);
        }
}
