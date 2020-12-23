#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct cup_s {
  int  next;
  char moving;
} cup;

void play(char *input, int n, int turns)
{
  cup *cups = malloc((n + 1) * sizeof(cup));
  int  i, prev = 0, current = input[0] - '0';

  /* Load the seed */

  for(i = 0; i < strlen(input); i++)
  {
    int v = input[i] - '0';

    cups[v].moving = 0;
    
    if(prev)
      cups[prev].next = v;

    prev = v;
  }
  /* Fill out for part 2 */

  if(n > strlen(input))
  {
    for(i = strlen(input) + 1; i <= n; i++)
    {
      cups[prev].next = i;
      prev = i;
    }
  }
  cups[prev].next = current;

  for(i = 0; i < turns; i++)
  {
    int j, c = current, t = (c + n - 2) % n + 1, save;

    /* Mark the moving cups */

    for(j = 0; j < 3; j++)
    {
      c = cups[c].next;
      cups[c].moving = 1;
    }
    /* Save the next cup after the olds that are moving */ 

    save = cups[c].next;

    /* Find the target cup for the move */

    while(cups[t].moving)
      t = (t + n - 2) % n + 1;

    /* Splice */

    cups[c].next = cups[t].next;
    cups[t].next = cups[current].next;
    cups[current].next = save;
    c = t;

    /* Unmark moving cups */

    for(j = 0; j < 3; j++)
    {
      c = cups[c].next;
      cups[c].moving = 0;
    }
    current = save;
  }
  /* Read out the result */

  current = 1;

  if(n < 10)
  {
    printf("Part 1: ");

    for(i = 0; i < n; i++)
    {
      printf("%d", current);
      current = cups[current].next;
    }
    printf("\n");
  }
  else
  {
    unsigned long a = cups[1].next, b = cups[a].next, v = a * b;

    printf("Part 2: %d * %d = %lu\n", cups[1].next, cups[cups[1].next].next, v);
  }
}

int main(int argc, char *argv[])
{
  /* Arg checks */

  if(argc < 2)
  {
    printf("%s <cups>\n", argv[0]);
    exit(1);
  }
  play(argv[1], 9, 100);
  play(argv[1], 1000000, 10000000);
}
