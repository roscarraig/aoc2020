#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[512];
  long count[5], row = 0;

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
  memset(count, 0, 5 * sizeof(long));

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    int l = strlen(buffer) - 1;

    if(buffer[row % l] == '#')
      count[0]++;
    if((row % 2 == 0) && (buffer[(row / 2) % l] == '#'))
      count[4]++;
    if(buffer[(3 * row) % l] == '#')
      count[1]++;
    if(buffer[(5 * row) % l] == '#')
      count[2]++;
    if(buffer[(7 * row) % l] == '#')
      count[3]++;
    row++;
  }
  printf("Part 1: %ld\n", count[1]);
  printf("Part 2: %ld * %ld * %ld * %ld * %ld = %ld\n",
      count[0], count[1], count[2], count[3], count[4],
      count[0] * count[1] * count[2] * count[3] * count[4]);
}
