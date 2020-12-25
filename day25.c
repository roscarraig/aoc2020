#include <string.h>
#include "aoc.h"

unsigned long find_loop(long key)
{
  long v = 1, i = 1;
  
  while(1)
  {
    v = (v * 7) % 20201227;
    if(v == key)
      return(i);
    i++;
  }
}

unsigned long run_loop(unsigned long key, unsigned long count)
{
  unsigned long v = 1, i;

  for(i = 0; i < count; i++)
    v = (v * key) % 20201227;
  return(v);
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[512];
  unsigned long card, door, cloop, dloop;
  int i;

  /* Read in the data */

  fgets(buffer, 512, fp);
  sscanf(buffer, "%ld", &card);
  fgets(buffer, 512, fp);
  sscanf(buffer, "%ld", &door);

  cloop = find_loop(card);
  printf("Part 1 %ld\n", run_loop(door, cloop));
}
