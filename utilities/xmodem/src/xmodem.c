#include<stdio.h>
#include<string.h>
#include"types.h"
#include"serial.h"

FILE *fd = NULL;
int sd;
unsigned char msgString[SIZE_MSG_PACKET] = {0};

int xmodem(FILE *fd, int port);

int main(int argc, const char* argv[])
{
  unsigned char u8RetVal = 0;

  if(argc < 3)
  {
    printf("Invalid number of arguments\n");
    u8RetVal = 1;
  }
  else if(NULL == (fd = fopen(argv[1],"r")))
  {
    printf("failed to open given file\n");
    u8RetVal = 1;
  }
  else if(0 > (sd = serial_open(argv[2])))
  {
    printf("failed to open serial device %s\n",argv[2]);
    u8RetVal = 1;
  }
  else if(0 > xmodem(fd,sd))
  {
    printf("upload failed!! aborting..\n");
    u8RetVal = 1;
  }
  else
  {
	
  }

  if(NULL != fd)
    fclose(fd);
  if(0 < sd)
    serial_close(sd);
		
  return u8RetVal;
}

int xmodem(FILE *fd, int port)
{
  unsigned char bytesRead,sRead;
  unsigned char seqNum=0,crc=0;
  unsigned char i,readBuffer[1];
  int ret = 0;
	
  bytesRead = fread((msgString+2),1,SIZE_DATA_FIELD,fd);

  while(bytesRead>0)
  {
    msgString[0]= SOH;
    msgString[1]= seqNum++;	
    crc = seqNum;
    
    for(i=2;i<bytesRead+2;i++)
    {
      crc += msgString[i];
    }
    msgString[bytesRead+2] = '*';
    msgString[bytesRead+3] = crc;
      
    for(i=0;i<MAX_TRIES;i++)
    {
      serial_write(port,msgString,bytesRead+4);
      sRead = serial_read(port,(unsigned char*)readBuffer,sizeof(readBuffer));
      if((sRead == 1) && (ACK == readBuffer[0]))
      {
        printf("block %d flashed\n",seqNum-1);
        break;
      }
      else
      {
        printf("no ACK from target %02x\n",readBuffer[0]);
      }
    }
		
    if(i == MAX_TRIES)
    {
      printf("write failed for block %d\n",seqNum-1);
      ret = -1;
      break;
    }
    memset(msgString,0,sizeof(msgString));
    bytesRead = fread((msgString+2),1,SIZE_DATA_FIELD,fd);
  }
  
  if(ret == 0)
  {
    msgString[0] = EOT;
    for(i=0;i<MAX_TRIES;i++)
    {
      serial_write(port,msgString,1);
      sRead = serial_read(port,readBuffer,sizeof(readBuffer));
      if((sRead == 1) && (ACK == readBuffer[0]))
      {
        printf("Pi flashed successfully :)\n");
        break;
      }
      printf("2 response: %02x\n",readBuffer[0]);
    }
  }
	
  return ret;
}
