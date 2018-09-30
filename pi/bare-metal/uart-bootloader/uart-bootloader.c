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

#include"defines.h"

extern void PUT32 (unsigned int, unsigned int);
extern void PUT8 (unsigned int, unsigned int);
extern unsigned int GET32 (unsigned int);
extern unsigned int GET8 (unsigned int);
extern void JUMPTO (unsigned int);
extern void DUMMY (unsigned int);

unsigned char buffer[132];

unsigned int uart_recv (void)
{
  while(1)
  {
    if(GET32(AUX_MU_LSR_REG)&0x01) break;
  }
  return(GET32(AUX_MU_IO_REG)&0xFF);
}

void uart_send (unsigned int c)
{
  while(1)
  {
    if(GET32(AUX_MU_LSR_REG)&0x20) break;
  }
  PUT32(AUX_MU_IO_REG,c);
}

void init(void)
{
  unsigned int var1;
  
  PUT32(AUX_ENABLES,1);
  PUT32(AUX_MU_IER_REG,0);
  PUT32(AUX_MU_CNTL_REG,0);
  PUT32(AUX_MU_LCR_REG,3);
  PUT32(AUX_MU_MCR_REG,0);
  PUT32(AUX_MU_IER_REG,0);
  PUT32(AUX_MU_IIR_REG,0xC6);
  PUT32(AUX_MU_BAUD_REG,270);

  var1 = GET32(GPFSEL1);
  var1 &= ~(7<<12); //gpio14
  var1 |= 2<<12;    //alt5
  var1 &= ~(7<<15); //gpio15
  var1 |= 2<<15;    //alt5 
  PUT32(GPFSEL1,var1);
  
  PUT32(GPPUD,0);
  for(var1=0;var1<150;var1++) DUMMY(var1);
  PUT32(GPPUDCLK0,(1<<14)|(1<<15));
  for(var1=0;var1<150;var1++) DUMMY(var1);
  PUT32(GPPUDCLK0,0);

  PUT32(AUX_MU_CNTL_REG,3);
  uart_send('R');
}
int myMain(void)
{
  unsigned int addr;
  unsigned char len=0;
  unsigned char crc, seqNum = 0;
  unsigned char i;
  
  init();
  
  addr = ADDRBASE;
  while(1)
  {
    buffer[0] = uart_recv();
    if(buffer[0] == SOH)
    {
      len = 1;
      if(seqNum != (buffer[len++]=uart_recv()))
      {
        uart_send(NAK);
        continue;
      }
      crc = SOH+seqNum;
      while(MSG_DELIMITER != (buffer[len++]=uart_recv()))
      {
        crc += buffer[len-1];
      }
      if(crc != uart_recv())
      {
        uart_send(NAK);
        continue;
      }
      
      for(i=2; i<len-1; i++)
      {
        PUT8(addr++,buffer[i]);
      }
      uart_send(ACK);
      seqNum++;
    }
    
    if(buffer[0] == EOT)
    {
      uart_send(ACK);
      break;
    }
  }
  JUMPTO(ADDRBASE);
  
  return 0; 
}
