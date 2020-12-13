#include <gmp.h>
#include <stdio.h>
#include <strings.h>

#include "aoc.h"

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char buffer[256], *pos;
  int leavetime, busnumber, soonest, soonestdelay, j;
  mpz_t when, bump, temp;

  mpz_init(when);
  mpz_init(bump);
  mpz_init(temp);

  fgets(buffer, 256, fp);
  sscanf(buffer, "%d", &leavetime);
  soonestdelay = leavetime;
  fgets(buffer, 256, fp);
  pos = buffer;
  while(pos)
  {
    if(*pos != 'x')
    {
      int delay;

      sscanf(pos, "%d", &busnumber);
      delay = busnumber - (leavetime % busnumber);
      
      /* This is unlikely */

      if(delay == busnumber)
      {
        printf("Exact match for %d\n", busnumber);
      }
      if(delay < soonestdelay)
      {
        soonest = busnumber;
        soonestdelay = delay;
      }
    }
    pos = index(pos, ',');
    if(pos)
      pos++;
  }
  printf("Part 1 bus: %d in %d minutes : %d\n", soonest, soonestdelay, soonest * soonestdelay);
  pos = buffer;
  j = 0;
  mpz_set_ui(when, 0);

  while(pos)
  {
    int bus;

    if(*pos != 'x')
    {
      sscanf(pos, "%d", &bus);
      if(j == 0)
        mpz_set_ui(bump, bus);
      else
      {
        while(1)
        {
          mpz_add(when, when, bump);
          mpz_add_ui(temp, when, j);
          if(mpz_tdiv_r_ui(temp, temp, bus) == 0)
          {
            char output[32];
            mpz_mul_ui(bump, bump, bus);
            printf("%d ", j);
            mpq_out_str(stdout, 10, bump);
            printf("\n");
            break;
          }
        }
      }
    }
    j++;
    pos = index(pos, ',');
    if(pos)
      pos++;
  }
}
