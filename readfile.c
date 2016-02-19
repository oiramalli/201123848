#include <stdio.h>

int main ()
{
   FILE *fp;
   unsigned char buffer[10];
   int c;
  
   fp = fopen("Disco1.dsk","r");
   fread(buffer,sizeof(buffer),1,fp);
   for(int i = 0; i<10; i++)
    printf("%u ", buffer[i]); 
   fclose(fp);
   
   return(0);
}