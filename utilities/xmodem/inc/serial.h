#ifndef __SERIAL_H__
#define __SERIAL_H_

#define BAUD_RATE B115200

int serial_open(const char* device);
int serial_write(int port, unsigned char* string, unsigned int len);
int serial_read(int port, unsigned char* dataBuffer, unsigned int len);
void serial_close(int port);

#endif
