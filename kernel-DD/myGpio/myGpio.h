#ifndef _MYGPIO_H_
#define _MYGPIO_H_

#include<linux/ioctl.h>

typedef enum
{
  PO = 0,
  P1,
  P2,
  P3,
  P4,
  P5,
  P6,
  P7,
  P8,
  P9,
  P10,
  P11,
  P12,
  P13,
  P14,
  P15,
  P16,
  P17,
  P18,
  P19,
  P20,
  P21,
  P22,
  P23,
  P24,
  P25,
  P26,
  P27,
  P28,
  P29,
  P30,
  P31,
  P32,
  P33,
  P34,
  P35,
  P36,
  P37,
  P38,
  P39,
  P40,
  P41,
  P42,
  P43,
  P44,
  P45,
  P46,
  P47,
  P48,
  P49,
  P50,
  P51,
  P52,
  P53,
}Pin_t;

typedef enum
{
  INPUT = 0x00,
  OUTPUT = 0x01,
  ALT0 = 0x04,
  ALT1 = 0x05,
  ALT2 = 0x06,
  ALT3 = 0x07,
  ALT4 = 0x03,
  ALT5 = 0x02
}Pin_func_t;

typedef enum
{
  LOW = 0,
  HIGH = 1
}Pin_mode_t;

typedef struct 
{
  Pin_t pin;
  Pin_func_t func;
  Pin_mode_t mode;
}myGpio_t;

#define IOCTL_SET_PIN_FUNC _IOW('p',1,myGpio_t *)
#define IOCTL_READ_PIN     _IOWR('p',2,myGpio_t *)
#define IOCTL_WRITE_PIN    _IOW('p',3,myGpio_t *)




#endif  // _MYGPIO_H_
