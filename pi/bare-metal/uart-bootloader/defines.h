#ifndef __DEFINES_H_
#define __DEFINES_H_


#define GPFSEL1             0x3F200004
#define GPSET0              0x3F20001C
#define GPCLR0              0x3F200028
#define GPPUD               0x3F200094
#define GPPUDCLK0           0x3F200098

#define AUX_ENABLES         0x3F215004
#define AUX_MU_IO_REG       0x3F215040
#define AUX_MU_IER_REG      0x3F215044
#define AUX_MU_IIR_REG      0x3F215048
#define AUX_MU_LCR_REG      0x3F21504C
#define AUX_MU_MCR_REG      0x3F215050
#define AUX_MU_LSR_REG      0x3F215054
#define AUX_MU_MSR_REG      0x3F215058
#define AUX_MU_SCRATCH      0x3F21505C
#define AUX_MU_CNTL_REG     0x3F215060
#define AUX_MU_STAT_REG     0x3F215064
#define AUX_MU_BAUD_REG     0x3F215068

#define ADDRBASE                0x8000

#define SOH                       0x01
#define ACK                       0x04
#define NAK                       0x08
#define EOT                       0x0C
#define MSG_LENGHT                 128
#define PACKET_LENGHT   MSG_LENGHT + 4
#define MSG_DELIMITER              '*'

#endif
