#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char buffer[256];
  int  x1 = 0, y1 = 0, x2 = 0, y2 = 0, wx = 10, wy = 1, dir1 = 90, dir2, dist, i, tmp;

  /* Arg checks */

  if(argc < 2)
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
    dir2 = 0;
    sscanf(buffer + 1, "%d\n", &dist);

    switch(buffer[0])
    {
      case 'N':
        y1 += dist;
        wy += dist;
        break;
      case 'S':
        y1 -= dist;
        wy -= dist;
        break;
      case 'E':
        x1 += dist;
        wx += dist;
        break;
      case 'W':
        x1 -= dist;
        wx -= dist;
        break;
      case 'L':
        dir1 = (360 + dir1 - dist) % 360;
        dir2 = (360 - dist) % 360;
        break;
      case 'R':
        dir1 = (360 + dir1 + dist) % 360;
        dir2 = dist % 360;
        break;
      case 'F':
        switch(dir1)
        {
          case 0:
            y1 += dist;
            break;
          case 90:
            x1 += dist;
            break;
          case 180:
            y1 -= dist;
            break;
          case 270:
            x1 -= dist;
            break;
        }
        x2 += wx * dist;
        y2 += wy * dist;
    }
    if(dir2 != 0)
    {
      for( i = 0; i < dir2 ; i += 90)
      {
        tmp = wx;
        wx = wy;
        wy = -tmp;
      }
    }
  }
  printf("Part 1: %d %d: %d\n", x1, y1, abs(x1) + abs(y1));
  printf("Part 2: %d %d: %d\n", x2, y2, abs(x2) + abs(y2));
}
