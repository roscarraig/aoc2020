#include "aoc.h"

/* Apply an operation, if no operator is passed just return b */ 

long aop(long a, long b, char op)
{
  if(op == '+')
    return(a + b);
  if(op == '*')
    return(a * b);
  return(b);
}

/* Recurse into the parentheses */

long process_line(char *line)
{
  long  depth = 0, val = 0, tv;
  char *p = line, op = 0;

  while(*p != '\n' && *p != ')')
  {
    if(*p == '(')
    {
      p++;
      depth = 1;

      /* Handle the sub-string */

      tv = process_line(p);

      /* and skip past it */

      while(depth > 0)
      {
        if(*p == '(')
          depth++;
        else if(*p == ')')
          depth--;
        p++;
      }
      /* Operate on the result */ 
      val = aop(val, tv, op);
    }
    /* Parse a number */

    else if (*p >= '0' && *p <= '9')
    {
      tv = *p - '0';
      p++;
      while(*p >= '0' && *p <= '9')
      {
        tv = tv * 10 + *p - '0';
        p++;
      }
      val = aop(val, tv, op);
    }
    /* Store an operator */

    else if (*p == '*' || *p == '+')
      op = *(p++);

    /* Skip spaces */

    else if (*p == ' ')
      p++;
  }
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
    printf("Line: %ld\n", line);
  }
  printf("Part 1: %ld\n", total);
}
