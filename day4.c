#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[512];
  unsigned int count = 0, fields = 0;
  const char* const fieldtags[] = {"byr:", "iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:", "cid:"};
  const char* const ecl[] = {"amb ", "blu ", "brn ", "gry ", "grn ", "hzl ", "oth "};

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
  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    if(strlen(buffer) > 1)
    {
      char *p = index(buffer, '\n');
      int   i;

      p[1] = 0;
      p[0] = ' ';
      p = buffer;

      while(*p)
      {
        for(i = 0; i < 8; i++)
          if(strncmp(p, fieldtags[i], 4) == 0)
          {
            p += 4;

            if(i >= 0 && i <= 2)
            {
              int year = 0;
              sscanf(p, "%d ", &year);
              if(
                  (i == 0 && year >= 1920 && year <= 2002) ||
                  (i == 1 && year >= 2010 && year <= 2020) ||
                  (i == 2 && year >= 2020 && year <= 2030)
                )
              {
                fields |= 1 << i;
                break;
              }
              fields = 0;
              break;
            }
            else if (i == 3)
            {
              int val = 0;
              char unit[2];

              sscanf(p, "%d%c%c ", &val, unit, unit + 1);
              if (
                  ((strncmp(unit, "cm", 2) == 0) && (val >= 150) && (val <= 193)) ||
                  ((strncmp(unit, "in", 2) == 0) && (val >= 59) && (val <= 76))
                 )
              {
                fields |= 1 << i;
                break;
              }
              fields = 0;
              break;
            }
            else if (i == 4)
            {
              int j;
              int b = 0;

              if (*p != '#')
              {
                fields = 0;
                break;
              }
              for(j = 1; j < 7; j++)
                if((p[j] < '0') || (p[j] > 'f') || ((p[j] > '9') && (p[j] < 'a')))
                {
                  fields = 0;
                  b = 1;
                  break;
                }
              if(b == 1)
                break;
              if(p[7] != ' ')
              {
                fields = 0;
                break;
              }
              fields |= 1 << i;
              break;
            }
            else if (i == 5)
            {
              int j;
              for(j = 0; j < 7; j++)
              {
                if(strncmp(p, ecl[j], 4) == 0)
                {
                  fields |= 1 << i;
                  break;
                }
              }
              break;
            }
            else if (i == 6)
            {
              int j;
              int b = 0;
              for(j = 0; j < 9; j++)
                if((p[j] < '0') || (p[j] > '9'))
                {
                  fields = 0;
                  b = 1;
                  break;
                }
              if(b == 1)
                break;
              if(p[9] != ' ')
              {
                fields = 0;
                break;
              }
              fields |= 1 << i;
              break;
            }
            fields |= 1 << i;
            break;
          }
        p = index(p, ' ') + 1;
      }
    }
    else
    {
      if(fields == (1 << 7) - 1 || fields == (1 << 8) - 1)
        count++;
      fields = 0;
    }
  }
  if(fields == (1 << 7) - 1 || fields == (1 << 8) - 1)
    count++;
  printf("%d\n", count);
}
