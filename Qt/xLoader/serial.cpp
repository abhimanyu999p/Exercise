/*
 * This file is a part of xLoader.
 *
 * xLoader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include "serial.h"

serial::serial()
{
    fd = 0;
    baudRate = 0;
    stopBit = false;
    noParity = false;
}

int serial::sConfig()
{
    struct termios mySerial;
    int ret = 0;

    fcntl(fd,F_SETFL,FNDELAY);
    memset(&mySerial,0,sizeof(mySerial));

    if(baudRate == 115200)
    {
        mySerial.c_cflag = (B115200 | CS8 | CLOCAL | CREAD);
    }
    if(noParity)
    {
        mySerial.c_iflag = ~IGNPAR;
    }
    if(stopBit)
    {
        mySerial.c_cflag &= ~CSTOPB;
    }

    tcflush(fd,TCIFLUSH);
    if(0 != tcsetattr(fd,TCSANOW,&mySerial))
    {
      ret = -1;
    }

    return ret;
}

int serial::sOpen(const char* port, int baud,bool stpBit,bool par)
{
    baudRate = baud;
    stopBit = stpBit;
    noParity = par;

    if(0 > (fd = open(port,O_RDWR|O_NOCTTY|O_NDELAY)))
    {
        return (-1);
    }
    else if(0 > sConfig())
    {
        return (-1);
    }
    else
    {
        return 0;
    }
}

int serial::sRead(unsigned char *dataBuffer, unsigned int len)
{
    int bytesRead = 0;

    *dataBuffer = 0;
    bytesRead = read(fd,dataBuffer,len);

    return bytesRead;
}

int serial::sWrite(char *dataBuffer, unsigned int len)
{
    int bytesWrote;

    bytesWrote = write(fd,dataBuffer,len);
    tcdrain(fd);
    usleep(len*100);
    return bytesWrote;
}

void serial::vClose()
{
    close(fd);
}
