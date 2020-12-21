#include <string.h>
#include "aoc.h"

typedef struct tile_s {
  unsigned int  data[10], sides[4], rsides[4];
  unsigned int  id;
  unsigned char sidematched;
  unsigned char x, y;
  unsigned char sidecount;
} tile;

int monsterhunt(char map[][97])
{
  int i, j, x, y, count = 0;
  char nessie[3][20];
  
  strncpy(nessie[0], "   #  #  #  #  #  # ", 20);
  strncpy(nessie[1], "###    ##    ##    #", 20);
  strncpy(nessie[2], " #                  ", 20);

  for(x = 0; x < 76; x++)
    for(y = 0; y < 93; y++)
    {
      int found = 1;

      for(j = 0; j < 3; j++)
        for(i = 0; i < 20; i++)
          if(nessie[j][i] == '#')
            if(map[y + j][x + i] != '#')
            {
              found = 0;
              j = 3;
              break;
            }
      if(found)
      {
        for(j = 0; j < 3; j++)
          for(i = 0; i < 20; i++)
            if(nessie[j][i] == '#')
              map[y + j][x + i] = 'O';
        count++;
      }
    }
  return(count);
}

unsigned int mirrorint(unsigned int input)
{
  unsigned int result = 0, i;

  for (i = 0; i < 10; i++)
    if(input & (1 << i))
      result |= (1 << (9 - i));
  return result;
}

unsigned int bitcountc(unsigned char input)
{
  unsigned int result = 0;

  while(input > 0)
  {
    if(input & 1)
      result++;
    input = input >> 1;
  }
  return(result);
}

unsigned char matchable_sides(tile this, tile that)
{
  int           i, j;
  unsigned char result = 0;
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(this.sides[i] == that.sides[j]
      || this.sides[i] == that.rsides[j])
      {
        result |= (1 << i);
      }
  return result;
}

void vflip(tile *tiles, int n)
{
  int i, tv;

  for(i = 0; i < 5; i++)
  {
    tv = tiles[n].data[i];
    tiles[n].data[i] = tiles[n].data[9 - i];
    tiles[n].data[9 - i] = tv;
  }
}

void hflip(tile *tiles, int n)
{
  int i, j;

  for(i = 0; i < 10; i++)
  {
    int tv = 0;

    for(j = 0; j < 10; j++)
      tv |= (tiles[n].data[i] & (1 << j)) ? (1 << (9 - j)) : 0;

    tiles[n].data[i] = tv;
  }
}

void rot90(tile *tiles, int n)
{
  unsigned int copy[10], i, j;

  for(j = 0; j < 10; j++)
  {
    copy[j] = tiles[n].data[j];
    tiles[n].data[j] = 0;
  }
  for(j = 0; j < 10; j++)
    for(i = 0; i < 10; i++)
      if(copy[j] & (1 << i))
        tiles[n].data[i] |= 1 << (9 - j);
}

/* Read the sides, top, bottom, left, right. Then mirror those */

void calc_sides(tile *tiles, int n, int tilecount)
{
  unsigned int pixels1 = 0, pixels2 = 0, i;

  tiles[n].sides[0] = tiles[n].data[0];
  tiles[n].sides[1] = tiles[n].data[9];

  for(i = 0; i < 10; i++)
  {
    unsigned int line = tiles[n].data[i];

    if (line & 1)
      pixels1 |= (1 << i);
    if (line & (1 << 9))
      pixels2 |= (1 << i);
  }
  tiles[n].sides[2] = pixels1;
  tiles[n].sides[3] = pixels2;

  for(i = 0; i < 4; i++)
    tiles[n].rsides[i] = mirrorint(tiles[n].sides[i]);

  tiles[n].sidematched = 0;

  for(i = 0; i < tilecount; i++)
  {
    unsigned char m;

    if(i == n || tiles[i].x)
      continue;

    tiles[n].sidematched |= matchable_sides(tiles[n], tiles[i]);
  }
}

int side_candidate_count(tile *tiles, int n, int side, int tilecount, int *result)
{
  int i, count=0;

  for(i = 0; i < tilecount; i++)
  {
    int j;

    if(tiles[i].x > 0)
      continue;

    for(j = 0; j < 4; j++)
      if(
          tiles[n].sides[side] == tiles[i].sides[j] ||
          tiles[n].sides[side] == tiles[i].rsides[j]
        )
      {
        count++;
        *result = i;
        /* Match for file i side j */
      }
  }
  return(count);
}

void mapfix(tile *tiles, int n, int side, int m, int tilecount)
{
  unsigned char um, op = 0;
  unsigned int  i, j, mside, x = tiles[n].x, y = tiles[n].y, candidate;

  tiles[n].sidematched -= 1 << side;

  for(j = 0; j < 4; j++)
  {
    if(tiles[n].sides[side] == tiles[m].sides[j])
    {
      mside = j;
      break;
    }
    if(tiles[n].sides[side] == tiles[m].rsides[j])
    {
      mside = j + 4;
      break;
    }
  }
  if (
    (side == 0 && mside == 0) || (side == 1 && mside == 1) ||
    (side == 2 && mside == 7) || (side == 3 && mside == 6)
  )
  {
    vflip(tiles, m);
  }
  else if (
    (side == 0 && mside == 2) || (side == 1 && mside == 3) ||
    (side == 2 && mside == 5) || (side == 3 && mside == 4)
  )
  {
    rot90(tiles, m);
    vflip(tiles, m);
    hflip(tiles, m);
  }
  else if (
    (side == 0 && mside == 3) || (side == 1 && mside == 2) ||
    (side == 2 && mside == 1) || (side == 3 && mside == 0)
  )
  {
    rot90(tiles, m);
    hflip(tiles, m);
  }
  else if (
    (side == 0 && mside == 4) || (side == 1 && mside == 5) ||
    (side == 2 && mside == 6) || (side == 3 && mside == 7)
  )
  {
    vflip(tiles, m);
    hflip(tiles, m);
  }
  else if (
    (side == 0 && mside == 5) || (side == 1 && mside == 4) ||
    (side == 2 && mside == 2) || (side == 3 && mside == 3)
  )
  {
    hflip(tiles, m);
  }
  else if (
    (side == 0 && mside == 6) || (side == 1 && mside == 7) ||
    (side == 2 && mside == 4) || (side == 3 && mside == 5)
  )
  {
    rot90(tiles, m);
    vflip(tiles, m);
  }
  else if (
    (side == 0 && mside == 7) || (side == 1 && mside == 6) ||
    (side == 2 && mside == 0) || (side == 3 && mside == 1)
  )
  {
    rot90(tiles, m);
  }
  switch(side)
  {
    case 0:
      y--;
      break;
    case 1:
      y++;
      break;
    case 2:
      x--;
      break;
    case 3:
      x++;
      break;
  }
  tiles[m].x = x;
  tiles[m].y = y;
  calc_sides(tiles, m, tilecount);

  for (i = 0; i < 4; i++)
    if(tiles[m].sidematched & (1 << i))
      if(side_candidate_count(tiles, m, i, tilecount, &candidate) == 1)
        mapfix(tiles, m, i, candidate, tilecount);
}

int part2(tile *tiles, int start, int tilecount)
{
  unsigned int i, j, k, candidate, count = 0;
  char map[96][97];

  tiles[start].x = 1;
  tiles[start].y = 1;

  if(tiles[start].sidematched & 1)
    vflip(tiles, start);

  if(tiles[start].sidematched & 4)
    hflip(tiles, start);

  calc_sides(tiles, start, tilecount);

  for (i = 0; i < 4; i++)
    if(tiles[start].sidematched & (1 << i))
      if(side_candidate_count(tiles, start, i, tilecount, &candidate) == 1)
        mapfix(tiles, start, i, candidate, tilecount);

  for(j = 0; j < 96; j++)
  {
    for(i = 0; i < 96; i++)
      map[j][i] = '-';
    map[j][96] = 0;
  }
  for(k = 0; k < tilecount; k++)
  {
    int x = ((tiles[k].x - 1) * 8) - 1, y = ((tiles[k].y - 1) * 8) - 1;

    for(j = 1; j < 9; j++)
    {
      unsigned int line = tiles[k].data[j];

      for(i = 1; i < 9; i++)
        map[x + i][y + j] = (line & (1 << i)) ? '#' : '.';
    }
  }
  monsterhunt(map);
  for(j = 0; j < 96; j++)
  {
    for(i = 0; i < 96; i++)
      if(map[j][i] == '#')
        count++;
  }
  printf("Part 2: %d\n", count);
}

int main(int argc, char *argv[])
{
  FILE         *fp = open_data(argc, argv);
  char          buffer[512];
  int           tilecount = 0, i = -1, row = 0, start;
  tile         *tiles;
  unsigned long l = 1;

  while(!feof(fp) && fgets(buffer, 512, fp))
    if(strncmp(buffer, "Tile ", 5) == 0)
      tilecount++;

  rewind(fp);
  tiles = malset(tilecount * sizeof(tile));

  while(!feof(fp) && fgets(buffer, 512, fp))
  {
    if(strncmp(buffer, "Tile ", 5) == 0)
    {
      int id;
      i++;
      sscanf(buffer, "Tile %d:", &id);
      tiles[i].id = id;
    }
    else if (strlen(buffer) > 1)
    {
      unsigned int pixels = 0, x;

      /* Normally I'd iteratively bitshift the whole thing, but since it may
       * be flipped . . . .
       */
      for(x = 0; x < 10; x++)
        if(buffer[x] == '#')
          pixels |= (1 << x);
      tiles[i].data[row++] = pixels;
    }
    else
    {
      calc_sides(tiles, i, tilecount);
      row = 0;
    }
  }
  for(i = 0; i < tilecount - 1; i++)
  {
    int j;
    unsigned char m;

    for(j = i + 1; j < tilecount; j++)
    {
      m = matchable_sides(tiles[i], tiles[j]);
      tiles[i].sidematched |= m;
      m = matchable_sides(tiles[j], tiles[i]);
      tiles[j].sidematched |= m;
    }
    tiles[i].sidecount = bitcountc(tiles[i].sidematched);

    if(bitcountc(tiles[i].sidematched) == 2)
    {
      l *= tiles[i].id;
      start = i;
    }
  }
  printf("Part 1: %ld\n", l);
  part2(tiles, start, tilecount);
}
