#ifndef MODEM_H
#define MODEM_H

#include "serial.h"
#include <fstream>
#include<QObject>

class modem : public QObject
{
    Q_OBJECT

public:
    explicit modem(QObject *parent = 0);
    int sModemOpen(const char* port, int baudRate, bool stopBit, bool noParity);
    int sflash(void);
    void vModemClose(void);
    int sGetTotalBlocks(void);

    const char* filePath;

    serial *s;

signals:
     void updateProgressBar(void);

     void updateConsole(QString);

private:
    std::ifstream binFile;

};

#endif // MODEM_H
