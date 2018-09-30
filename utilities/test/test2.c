#include<stdio.h>

unsigned char xstring[132]={0};

int main(int argc,char* argv[])
{
 unsigned char bytesRead,i;
 unsigned char blockNum=0,crc=0;
	FILE *fp;
	fp = fopen(argv[1],"r");

	if(fp==NULL)
	{
		printf("cannot open the specified file\n");
		return 1;
	}

  bytesRead = fread((xstring+2),1,128,fp);

	while(bytesRead>0)
	{
  	xstring[0]= 0x01;
		xstring[1]= blockNum++;
		
		printf("%02x",xstring[0]);
		//printf("\n");
		printf("%02x",xstring[1]);
		//printf("\n");
		
		crc = blockNum;
		for(i=2;i<bytesRead+2;i++)
		{
			crc += xstring[i];
			printf("%02x",xstring[i]);
			//printf("\n");
		}
		xstring[bytesRead+2] = '*';
		xstring[bytesRead+3] = crc;
		printf("%02x",xstring[bytesRead+2]);
		//printf("\n");
		printf("%02x",xstring[bytesRead+3]);
		//printf("\n");
    bytesRead = fread((xstring+2),1,128,fp);
  }
  printf("FF");
  return 0;
}
