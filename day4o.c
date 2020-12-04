#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
int main(int a,char*v[]){FILE*f;char b[512],u[2];int c=0,i,j,d;
const char*x="byriyreyrhgthcleclpidcidambblubrngrygrnhzloth";if
(a==1){printf("%s <filename>\n",v[0]);exit(1);}a=0;if(!(f=fopen
(v[1],"r"))){printf("Unable to open %s\n",v[1]);exit(2);}while(
!feof(f)&&fgets(b,512,f)){if(strlen(b)>1){char*p=index(b,10);p[
1]=0;p[0]=32;p=b;while(*p){for(i=0;i<8;i++)if((strncmp(p,x+i*3,
3)==0)&&p[3]==':'){p+=4;if(i>=0&&i<=2){sscanf(p,"%d ",&d);if((i
==0&&d>=1920&&d<=2002)||(i==1&&d>=2010&&d<=2020)||(d>=2020&&d<=
2030&&i==2)){c|=1<<i;break;}c=0 ;break;}else if(i==3){sscanf(p,
"%d%c%c ",&d,u,u+1);if(((strncmp(u,"cm ",2)==0)&&(d>=150)&&(d<=
193))||((!strncmp(u,"in",2))&&(d>=59)&&(d<=76))){c|=1<<i;break;
}c=0;break;}else if(i==4){if(*p!=35){c=0;break;}for(j=1;j<7;j++
)if((p[j]<'0')||(p[j]>=103)||((p[j]>'9')&&(p[j]<'a'))){c=0;d=1;
break;}if(d==1)break;if(*(p+7)!=' '){c=0;break;}c|=1<<i;break;}
else if(i==5){for(j=0;j<7;j++){if(strncmp(p,x+24+j*3,3)==0){c|=
1<<i;break;}}break;}else if(i==6){d=0;for(j=0;j<9;j++)if((p[j]<
48)||(p[j]>57)){c=0;d=1;break;}if(d==1)break;if(p[9]!=' '){c=0;
break;}c|=1<<i;break;}c|=1<<i;break;}p=index(p,' ')+1;}}else{if
(c%0x80==0x7f)a++;c=0;}}if(c%0x80==0x7f)a++;printf("%d!\n",a);}
