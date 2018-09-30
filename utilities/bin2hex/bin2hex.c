/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include<stdio.h>

unsigned char xstring[16]={0};

int main(int argc,char* argv[])
{
  unsigned char bytesRead,i;
  unsigned int addr=0;
  FILE *fp;
  fp = fopen(argv[1],"r");

  if(fp==NULL)
  {
    printf("cannot open the specified file\n");
    return 1;
  }
  printf("\t");
  printf("\t");
  for(i=0;i<16;i+=2)
  {
    printf(" ");
    printf("%x",i);
    printf("%x",i+1);	
    printf("  ");
  }
  printf("\n");

  bytesRead = fread(xstring,1,sizeof(xstring),fp);

  while(bytesRead>0)
  {
    for(i=0;i<bytesRead;i+=2)
    {
      if(i==0)
      {
        printf("%08x\t",addr);
        addr+=16;
      }
      printf("%02x",xstring[i]);
      printf("%02x",xstring[i+1]);
      printf(" ");
    }
    printf("\n");
    bytesRead = fread(xstring,1,sizeof(xstring),fp);
  }
return 0;
}

//EOF
