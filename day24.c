#include <string.h>
#include "aoc.h"

typedef struct tile_s
{
  short int x;
  short int y;
  short int hits;
  char      next;
  char      seen;
} tile;

int find_tile(tile *tiles, int x, int y)
{
  register short int i = 0;
  
  while(tiles[i].seen > 0)
  {
    if(tiles[i].x == x && tiles[i].y == y)
    {
      tiles[i].seen = 1;
      return(i);
    }
    i++;
  }
  tiles[i].seen = 1;
  tiles[i].x = x;
  tiles[i].y = y;
  return(i);
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[512];
  tile *tiles = malset(12000 * sizeof(tile));
  int   x = 0, y = 0, i, j, part1 = 0;

  /* Read in the data */

  while(!feof(fp) && fgets(buffer, 512, fp))
  {
    char *p = buffer;

    x = 0;
    y = 0;
    while(*p > 'a' && *p < 'z')
    {
      if(*p =='e')
        x++;
      else if(*p == 'w')
        x--;
      else
      {
        if(*p == 'n')
          y++;
        else
          y--;
        p++;
        if(*p == 'e' && (y + 12000) % 2 == 0)
          x++;
        else if (*p == 'w' && (y + 12000) % 2 == 1)
          x--;
      }
      p++;
    }
    i = find_tile(tiles, x, y);
    tiles[i].hits++;
  }
  i = 0;

  while(tiles[i].hits > 0)
    part1 += tiles[i++].hits % 2;

  printf("Part 1: %d\n", part1);

  for(j = 0; j < 100; j++)
  {
    int count = 0;

    i = 0;

    while(tiles[i].seen > 0)
    {
      if(tiles[i].hits % 2 == 1)
      {
        int x = tiles[i].x, y = tiles[i].y;

        tiles[find_tile(tiles, x - 1, y)].next++;
        tiles[find_tile(tiles, x + 1, y)].next++;
        tiles[find_tile(tiles, x, y + 1)].next++;
        tiles[find_tile(tiles, x, y - 1)].next++;

        if((y + 12000) % 2 == 0)
        {
          tiles[find_tile(tiles, x - 1, y + 1)].next++;
          tiles[find_tile(tiles, x - 1, y - 1)].next++;
        } else {
          tiles[find_tile(tiles, x + 1, y + 1)].next++;
          tiles[find_tile(tiles, x + 1, y - 1)].next++;
        }
      }
      i++;
    }
    i = 0;
    while(tiles[i].seen > 0)
    {
      if((tiles[i].hits % 2 == 1 && (tiles[i].next == 0 || tiles[i].next > 2)) ||
         (tiles[i].hits % 2 == 0 && tiles[i].next == 2))
        tiles[i].hits++;
      tiles[i].next = 0;
      count += tiles[i].hits % 2;
      i++;
    }
    if(j == 99)
      printf("Part 2: %d\n", count);
  }
}
