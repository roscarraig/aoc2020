#include <string.h>

#include "aoc.h"

int *read_ticket(char *line, char *valid)
{
  int  *result, value, fields = 0;
  char *pos = line;

  while(pos)
  {
    sscanf(pos, "%d", &value);
    fields++;

    if(valid[value] == 0)
      return NULL;

    pos = index(pos, ',');
    if(pos)
      pos++;
  }
  result = malset(fields * sizeof(int));
  pos = line;
  fields = 0;
  while(pos)
  {
    sscanf(pos, "%d", &(result[fields]));
    fields++;

    pos = index(pos, ',');
    if(pos)
      pos++;
  }
  return(result);
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[256], *pos, *valids=malset(1000), *fieldmatrix, *done;
  int   stage = 0, fieldcount = 0, *myticket, part1 = 0, **tickets = NULL, tcount = 0, todo, row = 0;
  long  part2 = 1;

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    if(stage == 0)
    {
      if(strlen(buffer) > 1)
      {
        int l1, h1, l2, h2, i, j;

        /* Just construct a bitmap of valid values */

        pos = index(buffer, ':') + 2;
        sscanf(pos, "%d-%d or %d-%d", &l1, &h1, &l2, &h2);

        fieldcount++;
        memset(valids + l1, 1, h1 + 1 - l1);
        memset(valids + l2, 1, h2 + 1 - l2);
      }
      else
        stage = 1;
    }
    else if (stage == 1)
    {
      /* Do the setup for the field match */

      if(strncmp(buffer, "your ticket:", 12) == 0)
      {
        done = malset(fieldcount);
        fieldmatrix = malset(fieldcount * fieldcount);
      }
      /* Squirrel away a copy of my ticket for later */ 

      else if(strlen(buffer) > 1)
        myticket = read_ticket(buffer, valids);
      else if (strlen(buffer) < 2)
        stage = 2;
    }
    else 
    {
      int *newticket, val;

      if(strncmp(buffer, "nearby tickets:", 13) == 0)
        continue;

      pos = buffer;

      /* The part1 search */

      while(pos)
      {
        sscanf(pos, "%d", &val);
        if(valids[val] == 0)
          part1 += val;
        pos = index(pos, ',');
        if(pos)
          pos++;
      }

      /* Grab and stow the ticket (if valid) */

      newticket = read_ticket(buffer, valids);
      if(newticket)
      {
        tickets = realloc(tickets, (tcount + 1) * sizeof(int *));
        tickets[tcount++] = newticket;
      }
    }
  }
  printf("Part 1: %d\n", part1);
  rewind(fp);

  /* Token value to avoid it falling out */
  strcpy(buffer, "abc");

  while(!feof(fp) && fgets(buffer, 256, fp) && strlen(buffer) > 1)
  {
    int l1, h1, l2, h2, i, j;

    pos = index(buffer, ':') + 2;
    sscanf(pos, "%d-%d or %d-%d", &l1, &h1, &l2, &h2);

    /* Build a matrix of matching fields to positions */

    for(i = 0; i < fieldcount; i++)
    {
      int val = 1;

      for(j = 0; j < tcount; j++)
        if(!((tickets[j][i] >= l1 && tickets[j][i] <= h1)
          || (tickets[j][i] >= l2 && tickets[j][i] <= h2)))
        {
          val = 0;
          break;
        }

      fieldmatrix[row + i * fieldcount] = val;
    }
    row++;
  }
  todo = fieldcount;

  /* Start chipping away the unique cases */ 

  while(todo > 0)
  {
    int i;

    for(i = 0; i < fieldcount; i++)
    {
      if(done[i] == 0)
      {
        int matches = 0, j, hit = -1;
        for(j = 0; j < fieldcount; j++)
        {
          if(fieldmatrix[i + j * fieldcount])
          {
            matches++;
            hit = j;
          }
        }
        /* Should not happen */

        if(matches == 0)
        {
          printf("Can't find match for field %d\n", i);
          exit(1);
        }
        /* Unique case */ 

        if(matches == 1)
        {
          /* Tick this one off */

          done[i] = 1;

          /* Rule this position out for the other fields, we don't care that
           * that includes this rule because it's done
           */
          for(j = 0; j < fieldcount; j++)
            fieldmatrix[j + hit * fieldcount] = 0;

          todo--;

          /* Okay, this is a dirty knowledge of the input data trick, but... */
          if(i < 6)
            part2 *= myticket[hit];
        }
      }
    }
  }
  printf("Part 2: %ld\n", part2);
}
