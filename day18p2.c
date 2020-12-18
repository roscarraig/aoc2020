#include <string.h>
#include "aoc.h"

/* Helper function to print out the expression at a given time */

void print_expression(long *expression, int bound)
{
  int i;

  for (i = 0 ; i < bound; i++)
    if(expression[i] > 0)
      printf("%ld", expression[i]);
    else if (expression[i] < 0)
      printf("%c", (char) -expression[i]);
  printf("\n");
}

/* Walk the expression, bound is the number of terms */

long process_expression(long *expression, int bound)
{
  int  i, j;

  while(bound > 1)
  {
    char oped = 0;

    for(i = 0; i < bound - 2; i++)
    {
      /* First up, eliminate any cases of n + m where n and m are integers
       * */
      if(expression[i] > 0 && expression[i + 2] > 0 && expression[i + 1] == -'+')
      {
        expression[i] += expression[i + 2];

        /* Remove the used terms from the expression */

        if(i < bound - 3)
        {
          memcpy(&expression[i + 1], &expression[i + 3], (bound - i) * sizeof(long));
          bound -= 2;
        }
        else
        {
          expression[i + 1] = 0;
          bound = i + 1;
        }
        oped = 1;
      }
      /* Look for one or more terms multiplied together and collapse to one
       * term. Anything other than an integer or * before the close aborts.
       */
      else if(expression[i] == -'(' && expression[i + 1] > 0)
      {
        int k;
        long v = expression[i + 1];

        for(k = i + 2; k < bound; k++)

          if(expression[k] == -')')
          {
            expression[i] = v;

            if(k < bound - 1)
            {
              memcpy(&expression[i + 1], &expression[k + 1], (bound - k) * sizeof(long));
              bound -= k - i;
            }
            else
            {
              expression[i + 1] = 0;
              bound = i + 1;
            }
            break;
          }
          else if (expression[k] == -'(' || expression[k] == -'+')
            break;
          else if (expression[k] > 0)
            v *= expression[k];

        oped = 1;
      }
    }
    if(oped == 0 && bound > 1 && expression[0] > 0 && expression[2] > 0 && expression[1] == -'*')
    {
      expression[0] *= expression[2];
      if(bound > 3)
      {
        memcpy(&(expression[1]), &(expression[3]), (bound - 3) * sizeof(long));
        bound -= 2;
      } else {
        bound = 1;
        expression[1] = 0;
      }
    }
  }
  return(expression[0]);
}

/* Convert the text line into a series of values, operands are positive,
 * operators and precedence are negative
 */

long process_line(char *line)
{
  long  depth = 0, val = 0, tv, *expression = malset(200 * sizeof(long));
  char *p = line, op = 0;
  int   i = 0;

  while(*p != '\n')
  {
    if (*p >= '0' && *p <= '9')
    {
      tv = *(p++) - '0';

      while(*p >= '0' && *p <= '9')
        tv = tv * 10 + *(p++) - '0';

      expression[i++] = tv;
    }
    else if(*p == ' ')
      p++;
    else
      expression[i++] = -*(p++);
  }
  val = process_expression(expression, i);
  free(expression);
  return(val);
}

int main(int argc, char *argv[])
{
  FILE *fp = open_data(argc, argv);
  char  buffer[512];
  long  line, total = 0;

  while(!feof(fp) && fgets(buffer, 512, fp))
  {
    line = process_line(buffer);
    total += line;
  }
  printf("Part 2: %ld\n", total);
}
