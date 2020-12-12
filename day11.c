#include <string.h>

#include "aoc.h"

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

int neighbours1(char *seats, int x, int y, int mx, int my)
{
  int count = 0, i, j;

  for(i = x - 1; i < x + 2 && i < mx; i++)
    if(i >= 0)
      for(j = y - 1; j < y + 2 && j < my; j++)
        if(j >= 0 && (i != x || j != y))
          if(seats[i + j * mx] == '#')
            count++;

  if(count == 0)
    return(0);
  if(count >= 4)
    return(1);
  return(2);
}

int neighbours2(char *seats, int x, int y, int mx, int my)
{
  int count = 0, i, j;

  for(i = -1 ; i < 2; i++)
    for(j = -1; j < 2; j++)
      if(i != 0 || j != 0)
        count += vectorcount(seats, x, y, i, j, mx, my);

  if(count == 0)
    return(0);
  if(count >= 5)
    return(1);
  return(2);

  return(count);
}

int generation(char *seats, int mx, int my, int tsize, int (*neigh)(char *, int, int, int, int))
{
  char *copy = malloc(tsize);
  int x, y, changes = 0;

  memcpy(copy, seats, tsize);

  for(y = 0; y < my; y++)
    for(x = 0; x < mx; x++)
    {
      int count = neigh(seats, x, y, mx, my);

      if(seats[x + y * mx] == '#' && count == 1)
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
  FILE *fp = open_data(argc, argv);
  char buffer[256], *seats1, *seats2, *pos;
  int  rowcount = 0, count = 0, seatlen = 0, rowlen, i = 0, changes, j;

  /* Count the lines */

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    rowcount++;
    seatlen += strlen(buffer);
  }
  rowlen = strlen(buffer);
  seats1 = malset(seatlen + 1);
  seats2 = malset(seatlen + 1);
  rewind(fp);
  fread(seats1, 1, seatlen, fp);
  memcpy(seats2, seats1, seatlen);

  for(j = 0; j < 2; j++)
  {
    if(j == 0)
    {
      while((changes = generation(seats1, rowlen, rowcount, seatlen, neighbours1)) > 0)
        i++;
      pos = seats1;
    }
    else
    {
      while((changes = generation(seats2, rowlen, rowcount, seatlen, neighbours2)) > 0)
        i++;
      pos = seats2;
    }
    count = 0;

    while((pos = index(pos, '#')))
    {
      count++;
      pos++;
    }
    printf("Part %d, %d generations: %d\n", j + 1, i, count);
  }
}
