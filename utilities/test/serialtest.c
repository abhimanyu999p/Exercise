#include<stdio.h>
#include<string.h>
#include<fcntl.h>   /* File Control Definitions           */
#include<termios.h> /* POSIX Terminal Control Definitions */
#include<unistd.h>  /* UNIX Standard Definitions 	        */ 
#include<errno.h>   /* ERROR Number Definitions           */

int serial_init(int fd,int brate)
{
  struct termios mySerial;
	int ret = 0;

  fcntl(fd,F_SETFL,FNDELAY);
	memset(&mySerial,0,sizeof(mySerial));
	/*if(0 != tcgetattr(fd,&mySerial))
	{
		printf("Errno %d from tcgetattr\n",errno);
		ret = -1;
	}
	
	cfsetospeed(&mySerial,brate);
	cfsetispeed(&mySerial,brate);
	
	mySerial.c_cflag &= ~(PARENB | CRTSCTS | CSTOPB);       // disable parity, no hardware control,1 stop bit,
  mySerial.c_cflag |= (CS8 | CLOCAL | CREAD);             // 8-bit data,ignore modem controls, enable reading
  
  mySerial.c_iflag &= ~(ICANON | ECHO | ISIG);            // no canonical processing
  mySerial.c_iflag &= ~(IXON | IXOFF | IXANY);            // shut off xon/xoff ctrl
  mySerial.c_iflag &= ~IGNPAR;                   
                                       
  mySerial.c_cc[VMIN]  = 1;                               // read doesn't block
  mySerial.c_cc[VTIME] = 5;  */                             // 0.5 seconds read timeout

  mySerial.c_cflag = (B115200 | CS8 | CLOCAL | CREAD);
  mySerial.c_iflag = IGNPAR;
  tcflush(fd,TCIFLUSH);
  if(0 != tcsetattr(fd,TCSANOW,&mySerial))
  {
  	printf("Errno %d from tcsetattr\n",errno);
  	ret = -1;
  }
  
  return ret;
}

int serial_open(const char* device)
{
	int fd;
	if(0 > (fd = open(device,O_RDWR|O_NOCTTY|O_NDELAY)))
	{
		printf("ERROR!! cannot open %s, errno:%d\n",device,errno);
		return (-1);
	}
	else if(0 > (serial_init(fd,B115200)))
	{
		printf("ERROR initializing serial port\n");
		return (-1);
	}
	else
	{
		printf("connected to serial port %s\n",device);
	}
	return fd;
}

int serial_write(int port,unsigned char* dataBuffer, unsigned int len)
{
	unsigned int bytesWrote;
	
	bytesWrote = write(port,dataBuffer,len);
	tcdrain(port);
	return bytesWrote;
}

int serial_read(int port,unsigned char* dataBuffer, unsigned int len)
{
	int bytesRead =0;

	*dataBuffer = 0; 
	bytesRead = read(port,dataBuffer,len);
	usleep(100000);
	
	return bytesRead;
}

void serial_close(int port)
{
	close(port);
}

int main(void)
{
	int fd;
	unsigned char buffer[1];
	
	fd=serial_open("/dev/ttyUSB0");
	if(fd > 0)
	{
		serial_write(fd,"hello",6);
	 for(int i=0; i<100;i++)
	 {
	 	serial_read(fd,(unsigned char*)buffer,sizeof(buffer));
	 	printf("%s\n",buffer);
	 }
	serial_close(fd);
	} 
}
