#include <stdio.h>
#include <stdlib.h>

            int
            main
             (
            int
             c
           ,char
            **v){
           FILE*f=
          fopen(v[1]
         ,"r");char b
         [256];int*n,i,
       j,k;for(c=0;!feof
      (f);c++)fgets(b,256
    ,f);rewind(f);n=malloc
   (c*sizeof(int));for(i=0;
   i<c;i++){fgets(b,256,f);
  sscanf(b,"%d\n",&(n[i]));}
 for(i=0;i<c-2;i++)for(j=i+1;
j<c-1;j++)for(k=j+1;k<c;k++)if
 (n[i]+n[j]+n[k]==2020)printf(
        "%d\n",n[i]*n[
           j]*n[k]);}
