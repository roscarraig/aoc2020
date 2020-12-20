#include <string.h>

#include "aoc.h"

typedef struct rule_s {
  unsigned char val;          /* If the rule is an emitter */
  unsigned char type;         /* Slight cheat, 3 for emit, otherwise a branch count */
  unsigned char branch[2][3]; /* The branch rules */
} rule;

void match(rule *rules, char *candidates, int candnum, int *queue, int queuelen, char *already)
{
  if (rules[queue[0]].type == 3)
  {
    int   i, check = 0, l = strlen(already);;
    char *prefix = malset(l + 2);

    strcpy(prefix, already);
    prefix[l++] = rules[queue[0]].val;
    prefix[l] = 0;

    /* Make sure there's a message that matches the prefix, otherwise it's
     * not interesting and we can move on
     */
    for(i = 0; i < candnum; i++)
      if(candidates[i * 128] == ' ' && strncmp(candidates + i * 128 + 1, prefix, l) == 0)
      {
        check = 1;
        break;
      }
    if(check == 0)
      return;

    /* Is there more to do? */

    if(queuelen > 1)
      match(rules, candidates, candnum, queue + 1, queuelen - 1, prefix);
    else
    {
      /* Can the messages for a match and tag those */
      for(i = 0; i < candnum; i++)
        if(candidates[i * 128] == ' ' && strcmp(candidates + i * 128 + 1, prefix) == 0)
          candidates[i * 128] = 'y';
    }
    free(prefix);
  }
  else
  {
    int i, j;

    /* For each branch */

    for(j = 0; j < rules[queue[0]].type; j++)
    {
      int subq[128];

      /* Insert the branch rules at the front of the queue */

      for(i = 0; i < 3 && rules[queue[0]].branch[j][i] != 0; i++)
        subq[i] = rules[queue[0]].branch[j][i];

      /* Append any remaining queue items */

      if(queuelen > 1)
        memcpy(subq + i, &(queue[1]), (queuelen - 1) * sizeof(int));

      /* and go one level down */

      match(rules, candidates, candnum, subq, queuelen + i - 1, already);
    }
  }
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[512];
  rule *rules = malset(132 * sizeof(rule));
  char *candidates = malset(128 * 400);
  int   c = 0, i, total = 0;

  /* Read the rules */

  while(!feof(fp) && fgets(buffer, 512, fp) && strlen(buffer) > 1)
  {
    int   ruleno, i = 0, j = 0;
    char *p = index(buffer, ' ') + 1;

    sscanf(buffer, "%d:", &ruleno);

    /* For an emitter just store the value and skip to the next loop */

    if(*p == '"')
    {
      rules[ruleno].val = *(p + 1);
      rules[ruleno].type = 3;
      continue;
    }

    rules[ruleno].type = 1;

    while(p && *p && *p != '\n')
    {
      int v;

      if(*p == '|')
      {
        j++;
        i = 0;
        p += 2;
        rules[ruleno].type = 2;
        continue;
      }
      sscanf(p, "%d ", &v);
      rules[ruleno].branch[j][i] = v;
      i++;
      p = index(p, ' ');
      if(p)
        p++;
    }
  }
  /* Read the messages */

  while(!feof(fp) && fgets(buffer, 512, fp))
  {
    char *p = index(buffer, '\n');

    *p = 0;

    /* Leave a space for match to use later */

    candidates[c * 128] = ' ';
    strncpy(candidates + c * 128 + 1, buffer, 127);
    c++;
  }
  /* Okay, the use of &total is a bit lazy, it fakes a one element array
   * containing 0
   */

  match(rules, candidates, c, &total, 1, "");

  /* and match tagged all the matching messages */

  for(i = 0; i < c; i++)
    if(candidates[i * 128] == 'y')
      total++;
  printf("Result: %d\n", total);
}
