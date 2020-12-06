#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[32];
  int counts[26], tcount = 0, pcount = 0, i;

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
  memset(counts, 0, 26 * sizeof(int));
  while(!feof(fp) && fgets(buffer, 32, fp))
  {
    if(strlen(buffer) > 1)
    {
      pcount++;
      char *p = index(buffer, '\n');

      *p = 0;
      p = buffer;

      while(*p)
      {
        counts[*p - 'a']++;
        p++;
      }
    }
    else
    {
      for(i = 0; i < 26; i++)
        if(counts[i] == pcount)
          tcount++;
      memset(counts, 0, 26 * sizeof(int));
      pcount = 0;
    }
  }
  for(i = 0; i < 26; i++)
    if(counts[i] == pcount)
      tcount++;
  printf("%d\n", tcount);
}
