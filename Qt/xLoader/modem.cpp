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
#include <sys/stat.h>
#include <iostream>

#include "modem.h"

#define SOH  0x01
#define ACK  0x04
#define NAK  0x08
#define EOT  0x0C
#define SIZE_DATA_FIELD                 128
#define SIZE_MSG_PACKET   SIZE_DATA_FIELD+4
#define MAX_TRIES                         5

char msgString[SIZE_MSG_PACKET] = {0};

using namespace std;

modem::modem(QObject *parent) : QObject(parent)
{
  s = new serial;
}

int modem::sModemOpen(const char* port, int baudRate, bool stopBit, bool noParity)
{
    int retVal = 0;

    binFile.open(filePath);

    if(!(binFile))
    {
        retVal = -1;
        emit updateConsole("              unable to open file");
    }
    else if(0 > (s->sOpen(port, baudRate, stopBit, noParity)))
    {
        binFile.close();
        retVal = -1;
        emit updateConsole("        unable to open COM port");
    }
    else
    {
        retVal = 1;
    }

    return retVal;
}

int modem::sflash()
{
    unsigned char sRead;
    unsigned char seqNum=0,crc=0;
    unsigned char i,readBuffer[1];
    int ret = 0;

    binFile.read((msgString+2),SIZE_DATA_FIELD);
    std::streamsize bytesRead = binFile.gcount();


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
        s->sWrite(msgString,(unsigned int)bytesRead+4);
        sRead = s->sRead((unsigned char*)readBuffer,sizeof(readBuffer));
        if((sRead == 1) && (ACK == readBuffer[0]))
        {
          emit updateProgressBar();
          break;
        }
        else
        {
          emit updateConsole("              no ACK from target");
        }
      }

      if(i == MAX_TRIES)
      {
        emit updateConsole("           write failed for block "+QString::number(seqNum-1));
        ret = -1;
        break;
      }
      memset(msgString,0,sizeof(msgString));
      binFile.read((msgString+2),SIZE_DATA_FIELD);
      bytesRead = binFile.gcount();
    }

    if(ret == 0)
    {
      msgString[0] = EOT;
      for(i=0;i<MAX_TRIES;i++)
      {
        s->sWrite(msgString,1);
        sRead = s->sRead(readBuffer,sizeof(readBuffer));
        if((sRead == 1) && (ACK == readBuffer[0]))
        {
          emit updateConsole("          Pi flashed successfully :)");
          break;
        }
      }
    }
    modem::vModemClose();

    return ret;
}

int modem::sGetTotalBlocks()
{
    struct stat fileSt;
    int blocks,sizeInBytes;

    stat(filePath,&fileSt);
    sizeInBytes = fileSt.st_size;
    blocks = sizeInBytes / SIZE_DATA_FIELD;
    if(0 != (sizeInBytes % SIZE_DATA_FIELD))
    {
        blocks += 1;
    }

    return blocks;
}

void modem::vModemClose()
{
    binFile.close();
    s->vClose();
}
