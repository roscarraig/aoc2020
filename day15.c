#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TARGET 30000000

int stow(int *numbers, int value, int pos)
{
  int j = numbers[value];

  numbers[value] = pos;
  return(j ? pos - j : 0);
}

int main(int argc, char *argv[])
{
  char *pos = argv[1];
  int   latest, i = 1, *numbers = malloc(TARGET * sizeof(int));

  memset(numbers, 0, TARGET * sizeof(int));

  while(pos)
  {
    sscanf(pos, "%d,", &latest);
    latest = stow(numbers, latest, i++);
    pos = index(pos, ',');
    if(pos)
      pos++;
  }
  while(i < TARGET)
  {
    latest = stow(numbers, latest, i++);
    if(i == 2020)
      printf("Part 1: %d\n", latest);
  }
  printf("Part 2: %d\n", latest);
}
