#include <string.h>
#include "aoc.h"

int pop(unsigned char *deck, unsigned int count)
{
  int result = deck[0];
  memcpy(deck, deck + 1, count);
  return(result);
}

/* Part 1 */

void play(unsigned char deck[][52], unsigned int *count)
{
  unsigned int card[2], winner;

  while(count[0] > 0 && count[1] > 0)
  {
    card[0] = pop(deck[0], count[0]--);
    card[1] = pop(deck[1], count[1]--);
    winner = card[0] > card[1] ? 0 : 1;
    deck[winner][count[winner]++] = card[winner];
    deck[winner][count[winner]++] = card[1 - winner];
  }
}

/* Part 2 . . . which is somewhat longer */

int play2(unsigned char *p1, unsigned char *p2, unsigned int c1, unsigned int c2)
{
  unsigned int card[2], winner, rounds = 0, i, th = 0;
  unsigned char *history = NULL, buffer[64];

  while(c1 > 0 && c2 > 0)
  {
    int   ts = c1 + c2 + 1;
    unsigned char *p = history;

    /* Compose the fingerprint of the hands */

    if(p1[0] < p2[0])
    {
      memcpy(buffer, p1, c1);
      buffer[c1] = 0;
      memcpy(buffer + c1 + 1, p2, c2);
    } else {
      memcpy(buffer, p2, c2);
      buffer[c2] = 0;
      memcpy(buffer + c2 + 1, p1, c1);
    }

    /* Check to see already seen */

    for(i = 0; i < rounds; i++)
    {
      if(memcmp(buffer, p, ts) == 0)
      {
        free(history);
        return(0);
      }
      p += ts;
    }
    /* Otherwise log it */

    if((rounds + 1) * ts > th)
    {
      th += 16384;
      history = realloc(history, th);
    }
    memcpy(history + rounds * ts, buffer, ts);
    rounds++;

    /* On to play */

    card[0] = pop(p1, --c1);
    card[1] = pop(p2, --c2);

    /* The subgame version */

    if(card[0] <= c1 && card[1] <= c2)
    {
      unsigned char *s1, *s2;

      s1 = malloc(card[0] + card[1]);
      s2 = malloc(card[1] + card[0]);
      memcpy(s1, p1, card[0]);
      memcpy(s2, p2, card[1]);
      winner = play2(s1, s2, card[0], card[1]);
      free(s1);
      free(s2);
    }
    /* High card */

    else
      winner = card[0] > card[1] ? 0 : 1;

    if(winner)
    {
      p2[c2++] = card[winner];
      p2[c2++] = card[1 - winner];
    }
    else
    {
      p1[c1++] = card[winner];
      p1[c1++] = card[1 - winner];
    }
  }
  if(rounds > 0)
    free(history);

  return(c1 > 0 ? 0 : 1);
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[512];
  unsigned int count[2], count2[2], player = -1, v, card[2], i;
  unsigned char *deck2[2], deck[2][52];

  memset(deck, 0, 2 * 52);
  deck2[0] = malset(52);
  deck2[1] = malset(52);

  /* Read in the data */

  while(!feof(fp) && fgets(buffer, 512, fp))
  {
    if(strncmp(buffer, "Player ", 7) == 0)
    {
      player++;
      count[player] = 0;
      count2[player] = 0;
    }
    else if(strlen(buffer) > 1)
    {
      sscanf(buffer, "%d", &v);
      deck[player][count[player]++] = v;
      deck2[player][count2[player]++] = v;
    }
  }
  /* Play part 1 */

  play(deck, count);
  player = count[0] > 0 ? 0 : 1;
  v = 0;

  for(i = 0; i < count[player]; i++)
    v += deck[player][i] * (count[player] - i);

  printf("Part 1: Player %d: %d\n", player + 1, v);

  /* Play part 2 */

  player = play2(deck2[0], deck2[1], count2[0], count2[1]);
  v = 0;

  for(i = 0; i < count2[0] + count2[1]; i++)
    v += deck2[player][i] * (count2[0] + count2[1] - i);

  printf("Part 2: Player %d: %d\n", player + 1, v);
}
