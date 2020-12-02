#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int passvalid1(char *password)
{
  int low, high, count = 0;
  char match, *pos;

  sscanf(password, "%d-%d %c: ", &low, &high, &match);
  pos = index(password, ':') + 1;
  while((pos = index(pos + 1, match)))
    count++;
  if(count <= high && count >= low)
    return(1);
  return(0);
}

int passvalid2(char *password)
{
  int first, second, count = 0;
  char match, *pos;

  sscanf(password, "%d-%d %c: ", &first, &second, &match);
  pos = index(password, ':') + 1;
  if(pos[first] == match)
    count++;
  if(pos[second] == match)
    count++;

  if(count == 1)
    return(1);
  return(0);
}

int main(int argc, char **argv)
{
  FILE *fp;
  char buffer[512];
  int  l, h, count = 0;
  char m;

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
  while(!feof(fp) && fgets(buffer, 512, fp))
    count += passvalid2(buffer);
  fclose(fp);
  printf("Count: %d\n", count);
}
