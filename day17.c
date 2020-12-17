/* Life, don't talk to me about life */

#include <string.h>

#include "aoc.h"

int ref(int x, int y, int z, int t, int bound)
{
  /* Map coordinates into a linear space */

  return(x + bound
      + (y + bound) * (2 * bound + 1)
      + (z + bound) * (2 * bound + 1) * (2 * bound + 1)
      + (t + bound) * (2 * bound + 1) * (2 * bound + 1) * (2 * bound + 1));
}

int neighbours(int x, int y, int z, int t, int bound, char *map)
{
  /* ...everybody needs good neigggghnbours... */

  int i, j, k, l, result = 0;
  
  for(i = -1; i <= 1; i++)
    for(j = -1; j <= 1; j++)
      for(k = -1; k <= 1; k++)
        for(l = -1; l <= 1; l++)
          if(i != 0 || j != 0 || k != 0 || l != 0)
            result += map[ref(x + i, y + j, z + k, t + l, bound)];
  return(result);
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[256];

  /* So I'm just assuming a maximum size of the space based on the initial
   * data set and the generation count plus a margin, doing so allows me to
   * directly address nodes rather than much searching, I could dynamically
   * pick the bound size and centre it correctly but the effort involved
   * isn't justified for the result.
   */
  int   bound = 20, mapsize = (2 * bound + 1) * (2 * bound + 1) * (2 * bound + 1) * (2 * bound + 1);
  int   i, j = 0, k, l, box[8], gen;
  char *now = malset(mapsize * sizeof(int)), *next = malset(mapsize * sizeof(int)), *tmp;

  /* Current size of the universe */

  memset(box, 0, 6 * sizeof(int));

  /* Read in the start point */

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    for (i = 0; i < strlen(buffer); i++)
      if(buffer[i] == '#')
        now[ref(i, j, 0, 0, bound)] = 1;
    j++;
  }
  box[1] = i;
  box[3] = j;

  /* And we rolling */

  for(gen = 0; gen < 6; gen++)
  {
    int count = 0;

    /* Expand the box one step */

    box[0]--; box[2]--; box[4]--; box[6]--;
    box[1]++; box[3]++; box[5]++; box[7]++;

    /* Step through every position in the box */

    for(i = box[0]; i <= box[1]; i++)
      for(j = box[2]; j <= box[3]; j++)
        for(k = box[4]; k <= box[5]; k++)
          for(l = box[6]; l <= box[7]; l++)
          {
            int nc = neighbours(i, j, k, l, bound, now), pos = ref(i, j, k, l, bound);

            /* Active */

            if(now[pos])
            {
              if (nc == 2 || nc == 3)
              {
                next[pos] = 1;
                count++;
              }
              else
                next[pos] = 0;
            }
            /* Inactive */

            else if(nc == 3)
            {
              next[pos] = 1;
              count++;
            }
            else
              next[pos] = 0;
          }
    /* Swap the now and next images */

    tmp = now;
    now = next;
    next = tmp;

    /* Result */

    printf("Gen %d %d\n", gen + 1, count);
  }
}
