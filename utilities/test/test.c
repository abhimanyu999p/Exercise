#include<stdio.h>

unsigned char xstring[20]={0};

int main(int argc,char* argv[])
{
 unsigned char bytesRead,i;
 unsigned char blockNum=0,crc=0;
 unsigned char cnt=2;
 int byt;
	FILE *fp;
	fp = fopen(argv[1],"r");

	if(fp==NULL)
	{
		printf("cannot open the specified file\n");
		return 1;
	}

	while((byt=fgetc(fp)) != EOF)
	{		
		if(cnt == 18)
		{
			xstring[0] = 0x01;
			xstring[1] = blockNum++;
			xstring[cnt] = '*';
			xstring[cnt+1] = crc+blockNum;
			cnt = 2;
		}
		xstring[cnt++] = byt;
		crc += byt;
	}
	
	xstring[0] = 0x01;
			xstring[1] = blockNum++;
			xstring[cnt] = '*';
			xstring[cnt+1] = crc+blockNum;
	
	printf("%02x",xstring[0]);
			printf(" ");
			printf("%02x",xstring[1]);
			printf(" ");
			for(i=2;i< cnt+2; i++)
			{
				printf("%02x",xstring[i]);
			}
			printf("\n");
return 0;
}
			
	
