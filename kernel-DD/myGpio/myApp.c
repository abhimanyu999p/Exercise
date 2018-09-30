#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#include"myGpio.h"

int readPin()
{
  int fd;
  myGpio_t p1;

  p1.pin = P16;

  fd=open("/dev/myGpio",O_RDWR);

  if(fd < 0)
  {
    printf("open fail\n");
  }
  else if (ioctl(fd,IOCTL_READ_PIN,(myGpio_t *)&p1) < 0)
  {
    printf("IOCTL 1 fail\n");
  }

  if(fd > 0)
  {
    close(fd);
  }
return p1.mode;
}

void writePin(Pin_mode_t m)
{
 int fd;
  myGpio_t p1;

  p1.pin = P16;
  p1.func = OUTPUT;
  p1.mode = m;
  fd=open("/dev/myGpio",O_RDWR);

  if(fd < 0)
  {
    printf("open fail\n");
  }
  else if (ioctl(fd,IOCTL_SET_PIN_FUNC,(myGpio_t *)&p1) < 0)
  {
    printf("IOCTL 1 fail\n");
  }
  else if(ioctl(fd,IOCTL_WRITE_PIN,(myGpio_t *)&p1) < 0)
  {
    printf("IOCTL 2 fail\n");
  }

  if(fd > 0)
  {
    close(fd);
  }

}

int main(int argc,char *argv[])
{
  Pin_mode_t mode;

  if(strcmp(argv[1],"--read") == 0)
  {
    mode = readPin();
    printf("pin mode is %d\n",mode);
  }
  else if(strcmp(argv[1],"--setHigh") == 0)
  {
    writePin(HIGH);
  }
  else if(strcmp(argv[1],"--setLow") == 0)
  {
    writePin(LOW);
  }
  else
  {
    printf("invalid arguement\n");
  }
return 0;
}
