#include<stdio.h>
#include <unistd.h>

#define SOH                       0x01
#define EOT                       0xFF
#define ACK                       0x55
#define NACK                      0xAA
#define MSG_LENGHT                 128
#define PACKET_LENGHT   MSG_LENGHT + 4
#define MSG_DELIMITER              '*'


unsigned char buffer[132];
FILE *F1,*F2;

unsigned char uart_recv (void)
{
 unsigned char redBuf[5];
 unsigned char ret;
 if(F1 != NULL)
 {
  fread(redBuf,1,1,F1);
  ret=redBuf[0];
 }
return ret;
}

void uart_send (unsigned int c)
{
  printf("uart-send %02x\n",c);
}

void PUT8(unsigned char *c)
{
  printf("PUT8:%02x ",*c);
  fwrite(c,1,1,F2);
}

void init(void)
{
  F1 = fopen("x.bin","r");
  if(F1 == NULL)
  {
    printf("file1 open failed\n");
  }

  F2 = fopen("x2.bin","ab");
  if(F2 == NULL)
  {
    printf("file2 open failed\n");
  }
}

int main(void)
{
  unsigned char len=0;
  unsigned char crc, seqNum = 0;
  unsigned char i;
  
  init();
  
  //addr = ADDRBASE;
  while(1)
  {
    //len = 0;
    buffer[0] = uart_recv();
    printf("id: %02x\n",buffer[0]);
    if(buffer[0] == SOH)
    {
      len = 1;
      if(seqNum != (buffer[len++]=uart_recv()))
      {
        uart_send(NACK);
        continue;
      }
      printf("recvd seqNum: %02x seqNum:%02x\n",buffer[1],seqNum);
      crc = SOH+seqNum;
      while(MSG_DELIMITER != (buffer[len++]=uart_recv()))
      {
        crc += buffer[len-1];
        printf("buff index:%d buff data:%02x\n",len-1,buffer[len-1]);
      }
      if(crc != uart_recv())
      {
        uart_send(NACK);
        printf("computed crc:%02x recvd crc%02x\n",crc,buffer[len]);
        continue;
      }
      
      for(i=2; i<len-1; i++)
      {
        PUT8(&buffer[i]);
      }
      uart_send(ACK);
      seqNum++;
      printf("\n");
    }
    
    if(buffer[0] == EOT)
    {
      uart_send(ACK);
      break;
    }
   usleep(100000);
  }
  //JUMPTO(ADDRBASE);
  if(F1 != NULL)
    fclose(F1);
  if(F2 != NULL)
    fclose(F2);
  
  return 0; 
}
