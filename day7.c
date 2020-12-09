#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bag_s {
  char *name;
  int   count;
  int  *subs;
  int  *subcount;
  int   held;
} bag;

int lookup(bag *bags, char *colour, int count)
{
  int i;
  for(i = 0; i < count; i++)
    if(strcmp(bags[i].name, colour) == 0)
      return(i);
  return(-1);
}

int contains(bag *bags, int colour, int entry)
{
  int i;

  for(i = 0; i < bags[entry].count; i++)
  {
    if(bags[entry].subs[i] == colour)
      return(1);
    if(contains(bags, colour, bags[entry].subs[i]))
      return(1);
  }
  return(0);
}

int contained(bag *bags, int colour)
{
  int i;

  if(bags[colour].held == 0)
    for(i = 0; i < bags[colour].count; i++)
      bags[colour].held += bags[colour].subcount[i] * contained(bags, bags[colour].subs[i]);
  return(bags[colour].held + 1);
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[256];
  int  count = 0;
  int  i = 0, target = -1, matchcount = 0;
  bag *bags;

  /* Sanity check on the args */

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

  /* Count the lines */

  while(!feof(fp) && fgets(buffer, 256, fp))
    count++;

  rewind(fp);
  bags = malloc(sizeof(bag) * count);
  memset(bags, 0, sizeof(bag) * count);

  /* Enumerate the bags */

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    char *p = strstr(buffer, " bags contain ");
    *p = 0;
    bags[i].name = malloc(strlen(buffer) + 1);
    strcpy(bags[i].name, buffer);
    i++;
  }
  rewind(fp);
  i = 0;

  /* Super tedious content mapping */

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    char *p = strstr(buffer, " bags contain "), *q;
    p += 14;

    if(strcmp(p, "no other bags.\n"))
    {
      /* Yes, that means if the string doesn't match */

      int j = 1;
      q = p;

      /* Count the sub bag classes */

      while((q = index(q, ',')))
      {
        j++;
        q++;
      }
      bags[i].count = j;
      bags[i].subs = malloc(j * sizeof(int));
      bags[i].subcount = malloc(j * sizeof(int));
      q = p;
      j = 0;

      /* Read in the sub bag level */
      while(p)
      {
        int bagentry;

        /* Get the count */

        sscanf(p, "%d ", &(bags[i].subcount[j]));

        /* Skip the number */

        p = index(p, ' ') + 1;

        /* Find the bag delimiter and trim that point in the string */

        q = strstr(p, " bag");
        *q = 0;
        q++;

        bagentry = lookup(bags, p, count);

        if(bagentry < 0)
        {
          printf("Could not find colour %s\n", p);
          exit(3);
        }
        bags[i].subs[j] = bagentry;
        j++;
        p = index(q, ',');

        if(p)
          p+= 2;
      }
    }
    i++;
  }
  target = lookup(bags, "shiny gold", count);

  for(i = 0; i < count; i++)
    matchcount += contains(bags, target, i);

  printf("%d\n", matchcount);
  printf("%d\n", contained(bags, target) - 1);
}
