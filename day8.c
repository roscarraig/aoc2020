#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOP 0
#define JMP 1
#define ACC 2

typedef struct op_s {
  int code;
  int val;
  int count;
} op;

int part1(op *prog, int count)
{
  int i = 0, acc = 0;

  while(1)
  {
    if(prog[i].count == 1)
    {
      printf("%d\n", acc);
      exit(0);
    }
    prog[i].count++;
    if(prog[i].code == ACC)
    {
      acc += prog[i].val;
      i++;
    }
    else if(prog[i].code == JMP)
      i += prog[i].val;
    else if(prog[i].code == NOP)
      i++;
  }
}

int estep(op *prog, unsigned int *ptr, unsigned int *acc, unsigned int count)
{
  if(prog[*ptr].count)
    return(0);
  prog[*ptr].count++;
  if(prog[*ptr].code == ACC)
  {
    *acc += prog[*ptr].val;
    (*ptr)++;
  }
  else if(prog[*ptr].code == JMP)
    (*ptr) += prog[*ptr].val;
  else if(prog[*ptr].code == NOP)
    (*ptr)++;
  if(*ptr >= count)
  {
    printf("%d\n", *acc);
    exit(0);
  }
  return(1);
}

void *malset(size_t n)
{
  void *result = malloc(n);
  memset(result, 0, n);
  return(result);
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[64];
  unsigned int count = 0, i = 0, j = 0, acc = 0, *accs, *ptrs;
  op *prog, **progs;

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
  while(!feof(fp) && fgets(buffer, 64, fp))
    count++;
  rewind(fp);
  prog = malloc(sizeof(op) * count);
  memset(prog, 0, sizeof(op) * count);

  while(!feof(fp) && fgets(buffer, 64, fp))
  {
    if(strncmp(buffer, "nop", 3) == 0)
      prog[i].code = NOP;
    else if(strncmp(buffer, "acc", 3) == 0)
    {
      prog[i].code = ACC;
      j++;
    }
    else if(strncmp(buffer, "jmp", 3) == 0)
    {
      prog[i].code = JMP;
      j++;
    }
    sscanf(buffer + 4, "%d", &(prog[i++].val));
  }
  progs = malloc(sizeof(op*) * j);
  ptrs = malset(sizeof(int) * j);
  accs = malset(sizeof(int) * j);

  for(i = 0; i < j; i++)
  {
    int k, l = 0;

    progs[i] = malloc(sizeof(op) * count);
    memcpy(progs[i], prog, sizeof(op) * count);

    for(k = 0; k < count; k++)
      if(progs[i][k].code != ACC && l++ == i)
      {
        progs[i][k].code = 1 - progs[i][k].code;
        k = count;
      }
  }
  while(1)
    for(i = 0; i < j; i++)
      estep(progs[i], &ptrs[i], &accs[i], count);
}
