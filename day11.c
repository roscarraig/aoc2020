#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *malset(size_t n)
{
  void *result = malloc(n);
  memset(result, 0, n);
  return(result);
}

int neighbours1(char *seats, int x, int y, int mx, int my)
{
  int count = 0, i, j;
  for(i = x - 1; i < x + 2 && i < mx; i++)
    if(i >= 0)
      for(j = y - 1; j < y + 2 && j < my; j++)
        if(j >= 0 && (i != x || j != y))
          if(seats[i + j * mx] == '#')
            count++;
  return(count);
}

int vectorcount(char *seats, int x, int y, int dx, int dy, int mx, int my)
{
  x += dx;
  y += dy;
  while(x >= 0 && y >= 0 && x < mx && y < my)
  {
    char c = seats[x + y * mx];
    if(c == '#')
      return(1);
    if(c == 'L')
      return(0);
    x += dx;
    y += dy;
  }
  return(0);
}

int neighbours2(char *seats, int x, int y, int mx, int my)
{
  int count = 0, i, j;
  for(i = -1 ; i < 2; i++)
    for(j = -1; j < 2; j++)
      if(i != 0 || j != 0)
        count += vectorcount(seats, x, y, i, j, mx, my);
  return(count);
}

int generation(char *seats, int mx, int my, int tsize)
{
  char *copy = malloc(tsize);
  int x, y, changes = 0;

  memcpy(copy, seats, tsize);
  for(y = 0; y < my; y++)
    for(x = 0; x < mx; x++)
    {
      int count = neighbours2(seats, x, y, mx, my);

      if(seats[x + y * mx] == '#' && count >= 5)
      {
        copy[x + y * mx] = 'L';
        changes++;
      } else if(seats[x + y * mx] == 'L' && count == 0) {
        copy[x + y * mx] = '#';
        changes++;
      }
    }
  if(changes > 0)
    memcpy(seats, copy, tsize);
  return(changes);
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[256], *seats, *pos;
  int  count = 0, seatlen = 0, rowlen, i = 0, changes;

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

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    count++;
    seatlen += strlen(buffer);
  }
  rowlen = strlen(buffer);
  seats = malset(seatlen + 1);
  rewind(fp);
  fread(seats, 1, seatlen, fp);

  while((changes = generation(seats, rowlen, count, seatlen)) > 0)
    i++;

  pos = seats;
  count = 0;

  while((pos = index(pos, '#')))
  {
    count++;
    pos++;
  }
  printf("%d\n", count);
}
