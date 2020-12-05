#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bpcsum(char *buffer)
{
  int row = 0, cross = 0;
  int i;

  for(i = 0; i < 7; i++)
  {
    row *= 2;
    if(buffer[i] == 'B')
      row += 1;
  }
  for(i = 0; i < 3; i++)
  {
    cross *= 2;
    if(buffer[7 + i] == 'R')
      cross += 1;
  }
  printf("%11s %d * 8 + %d = %d\n", buffer, row, cross, row * 8 + cross);
  return(row * 8 + cross);
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[32];
  unsigned int top = 0, this, seats[989];

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
  memset(seats, 0, 989 * sizeof(int));
  while(!feof(fp) && fgets(buffer, 32, fp))
  {
    this = bpcsum(buffer);
    seats[this] = 1;
    if (this > top)
      top = this;
  }
  printf("%d\n", top);
  for(this = 1; this < 988; this++)
  {
    if(seats[this] == 0 && seats[this - 1] && seats[this + 1])
      printf(">>> %d\n", this);
  }
}
