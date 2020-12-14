#include <string.h>

#include "aoc.h"

typedef struct entry_s {
  long address;
  long value;
} entry;

typedef struct store_s {
  entry *data;
  long   count;
} memstore;

/* Store a key/value pair */

void setval(memstore *memory, long address, long value)
{
  long i;

  /* Is the key already present? If so, update */

  for(i = 0; i < (*memory).count; i++)
    if((*memory).data[i].address == address)
    {
      (*memory).data[i].value = value;
      return;
    }

  /* Otherwise expand and store */

  (*memory).data = realloc((*memory).data, ((*memory).count + 1) * sizeof(entry));
  (*memory).data[(*memory).count].address = address;
  (*memory).data[(*memory).count].value = value;
  (*memory).count++;
}

/* Recursive fuzzy store, splits for the least significant set bit on the
 * mask, if none stores at the specified location
 */
void store(memstore *memory, long loc, long val, long xmask)
{
  long i = 1;

  if(xmask)
  {
    while((i & xmask) == 0)
      i *= 2;
    store(memory, loc | i, val, xmask ^ i);
    store(memory, loc & (((1L << 36) - 1) ^ i), val, xmask ^ i);
  }
  else
    setval(memory, loc, val);
}

int main(int argc, char *argv[])
{
  FILE          *fp = open_data(argc, argv);
  char           buffer[256], *pos;
  int            loc, maxloc = 0;
  unsigned long *mem, maskzero, maskone, maskx, val;
  memstore       memory;

  memory.count = 0;
  memory.data = NULL;

  while(!feof(fp) && fgets(buffer, 256, fp))
    if(strncmp(buffer, "mem[", 4) == 0)
    {
      sscanf(buffer + 4, "%d", &loc);

      if(loc > maxloc)
        maxloc = loc;
    }
  maxloc++;
  rewind(fp);
  mem = malset(maxloc * sizeof(long));

  /* Pass through the data and store for both parts 1 and 2 simultaneously */

  while(!feof(fp) && fgets(buffer, 256, fp))
    if(strncmp(buffer, "mask = ", 7) == 0)
    {
      pos = buffer + 7;
      maskzero = 0;
      maskone = 0;
      maskx = 0;

      while(*pos != '\n')
      {
        maskzero *= 2;
        maskone *= 2;
        maskx *= 2;

        if(*pos == '1')
        {
          maskone |= 1;
          maskzero |= 1;
        }
        else if(*pos == 'X')
        {
          maskzero |= 1;
          maskx |= 1;
        }
        pos++;
      }
    }
    else if(strncmp(buffer, "mem[", 4) == 0)
    {
      sscanf(buffer + 4, "%d] = %ld", &loc, &val);
      mem[loc] = (val | maskone) & maskzero;
      store(&memory, loc | maskone, val, maskx);
    }
  val = 0;

  for(loc = 0; loc < maxloc; loc++)
    if(mem[loc])
      val += mem[loc];

  printf("Part 1: %ld\n", val);

  val = 0;
  for(loc = 0; loc < memory.count; loc++)
    val += memory.data[loc].value;

  printf("Part 2: %ld\n", val);
}
