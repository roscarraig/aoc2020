#define a 30000000
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

             int
            s(int
           *n,int
           b,int c)
          {int j=n[b
           ];n[b]=c;
          return(j?c-
        j:0);}int main
       (int l,char *c[]
      ){char*b=c[1];int*
      n=malloc(a*sizeof(
     int)),i=1;memset(n,0,
    a*sizeof(int));while(b)
   {sscanf(b,"%d,",&l);l=s(n
  ,l,i++);b=index(b,44);if(b)
 b++;}while(i<a){l=s(n,l,i++);
if(i==2020)printf("Part 1: %d\n"
             ,l);}
            printf(
         "Part 2: %d\n"
             ,l);}
