#include <string.h>
#include "aoc.h"

typedef struct ingredient_s
{
  char *name;
  int  *allergen, rcount, acount;
} ingredient;

typedef struct allergen_s
{
  char *name;
  int  *ingredients, icount, seen, identified;
} allergen;

typedef struct ilist_s {
  ingredient  *items;
  unsigned int count;
} ilist;

typedef struct alist_s {
  allergen    *items;
  unsigned int count;
} alist;

typedef struct pair_s {
  char allergen[10];
  char ingredient[10];
} pair;

int cmp(const void *a, const void *b)
{
  return(strcmp(((pair *)a)->allergen, ((pair *)b)->allergen));
}

unsigned int ingredient_id(ilist *list, char *name)
{
  int i;

  for(i  = 0; i < list->count; i++)
    if(strcmp(name, list->items[i].name) == 0)
      return(i);
  list->items = realloc(list->items, (i + 1) * sizeof(ingredient));
  list->items[i].name = malloc(strlen(name) + 1);
  strcpy(list->items[i].name, name);
  list->items[i].allergen = NULL;
  list->items[i].rcount = 0;
  list->items[i].acount = 0;
  list->count++;
  return(i);
}

unsigned int allergen_id(alist *list, char *name)
{
  int i;

  for(i  = 0; i < list->count; i++)
    if(strcmp(name, list->items[i].name) == 0)
      return(i);
  list->items = realloc(list->items, (i + 1) * sizeof(allergen));
  list->items[i].name = malloc(strlen(name) + 1);
  strcpy(list->items[i].name, name);
  list->items[i].seen = 0;
  list->items[i].identified = -1;
  list->count++;
  return(i);
}

int possible_allergen(alist *list, int id)
{
  int i, j;

  for(i = 0; i < list->count; i++)
    for(j = 0; j < list->items[i].icount; j++)
      if(list->items[i].ingredients[j] == id)
        return(1);
  return(0);
}

int main(int argc, char *argv[])
{
  FILE         *fp = open_data(argc, argv);
  char          buffer[1024], *p, notunique = 1;
  alist         allergens;
  ilist         ingredients;
  unsigned int *ibuffer = NULL, *abuffer = NULL, abufcount = 0, ibufcount = 0, part1 = 0, i;
  pair         *part2;

  allergens.items = NULL;
  allergens.count = 0;
  ingredients.items = NULL;
  ingredients.count = 0;

  while(!feof(fp) && fgets(buffer, 1024, fp))
  {
    char ingbuf[16], *p = buffer, icount = 0, acount = 0, i;

    /* Read the ingredients */

    while(*p != '(')
    {
      char *p1 = index(p, ' ');

      strncpy(ingbuf, p, (p1 - p));
      ingbuf[p1 - p] = 0;

      if(ibufcount < icount + 1)
        ibuffer = realloc(ibuffer, (++ibufcount) * sizeof(int));

      ibuffer[icount] = ingredient_id(&ingredients, ingbuf);
      ingredients.items[ibuffer[icount++]].rcount++;
      p = index(p, ' ');
      p++;
    }
    p += 10;

    /* Read the allergens */

    while(p)
    {
      char *p1 = index(p, ',');

      if(!p1)
        p1 = index(p, ')');

      strncpy(ingbuf, p, (p1 - p));
      ingbuf[p1 - p] = 0;

      if(abufcount < acount + 1)
        abuffer = realloc(abuffer, (++abufcount) * sizeof(int));

      abuffer[acount++] = allergen_id(&allergens, ingbuf);

      p = (*p1 == ')') ? 0 : p1 + 2;
    }

    for (i = 0; i < acount; i++)
    {
      /* Is this a new allergen? Record all the ingredients */

      if(allergens.items[abuffer[i]].seen == 0)
      {
        int j;
        allergens.items[abuffer[i]].icount = icount;
        allergens.items[abuffer[i]].ingredients = malloc(icount * sizeof(int));
        allergens.items[abuffer[i]].seen = 1;
        memcpy(allergens.items[abuffer[i]].ingredients, ibuffer, icount * sizeof(int));
      }
      /* Otherwise cross off any ingredients that aren't present */

      else
      {
        int j, k;

        for(j = 0; j < allergens.items[abuffer[i]].icount; j++)
        {
          int i_id = allergens.items[abuffer[i]].ingredients[j], match = 0;

          if(i_id >= 0)
            for(k = 0; k < icount; k++)
            {
              if(i_id == ibuffer[k])
              {
                match = 1;
                break;
              }
            }
          if(match == 0)
            allergens.items[abuffer[i]].ingredients[j] = -1;
        }
      }
    }
  }
  /* Tot up the scores for part 1 */

  for(i = 0; i < ingredients.count; i++)
    if(possible_allergen(&allergens, i) == 0)
      part1 += ingredients.items[i].rcount;

  printf("Part 1: %d\n", part1);

  /* Reduce the mapping down to 1:1 */

  while(notunique)
  {
    notunique = 0;

    for(i = 0; i < allergens.count; i++)
    {
      int count = 0, j;

      /* Has this one already been elimitated? */

      if(allergens.items[i].identified >= 0)
        continue;

      /* Count the candidates */

      for(j = 0; j < allergens.items[i].icount; j++)
        if(allergens.items[i].ingredients[j] >= 0)
          count++;

      if(count > 1)
        notunique = 1;
      else
      {
        /* We have a new 1:1. fix it as identified and then remove from the
         * other allergens
         */
        for(j = 0; j < allergens.items[i].icount; j++)
          if(allergens.items[i].ingredients[j] >= 0)
          {
            allergens.items[i].identified = allergens.items[i].ingredients[j];
            break;
          }

        for(j = 0; j < allergens.count; j++)
          if(i != j && allergens.items[j].identified < 0)
          {
            int k;

            for(k = 0; k < allergens.items[j].icount; k++)
              if(allergens.items[j].ingredients[k] == allergens.items[i].identified)
                allergens.items[j].ingredients[k] = -1;
          }
      }
    }
  }
  /* Let's dump all the 1:1s out so we can sort and report them */

  part2 = malset(allergens.count * sizeof(pair));

  for(i = 0; i < allergens.count; i++)
  {
    strcpy(part2[i].allergen, allergens.items[i].name);
    strcpy(part2[i].ingredient, ingredients.items[allergens.items[i].identified].name);
  }
  qsort(part2, allergens.count, sizeof(pair), cmp);

  /* and done */

  printf("Part2:");

  for(i = 0; i < allergens.count; i++)
    printf("%c%s", i == 0 ? ' ' : ',', part2[i].ingredient);

  printf("\n");
}
