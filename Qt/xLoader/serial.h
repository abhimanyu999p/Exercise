#ifndef SERIAL_H
#define SERIAL_H


class serial
{
private:
    int fd;
    int baudRate;
    bool stopBit;
    bool noParity;


private:
    int sConfig(void);
public:
    serial(void);
    int sOpen(const char* port, int baud,bool stpBit,bool par);
    int sRead(unsigned char* dataBuffer, unsigned int len);
    int sWrite(char* dataBuffer, unsigned int len);
    void vClose(void);
};

#endif // SERIAL_H
