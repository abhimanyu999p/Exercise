/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "gpio.h"
#include "uart.h"

size_t strlen(const char* str)
{
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
}
 
void uart_init(void)
{
	// Disable UART0.
	_reg_write(UART0_CR, 0x00000000);
 
	// Setup the GPIO pin 14 && 15.
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	_reg_write(GPPUD, 0x00000000);
	delay(150);
 
	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	_reg_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
 
	// Write 0 to GPPUDCLK0 to make it take effect.
	_reg_write(GPPUDCLK0, 0x00000000);

	// Clear pending interrupts.
	_reg_write(UART0_ICR, 0x7FF);
 

	/*
	 * The IBRD and FBRD registers specify the baud rate.
	 *
	 * The baud rate divisor is calculated as follows:
	 * Baud rate divisor BAUDDIV = (FUARTCLK/(16*Baud rate))
	 * where FUARTCLK is the UART reference clock frequency.
	 *
	 * UART_CLOCK = 3,000,000; Baud = 115200.
	 * IBRD = int(3,000,000/(16*115,200)) = int(1.627) = 1
	 * FBRD = round(0.627*64) = 40
	 *
	 * Why 64 ? FBRD is 6bits, muliplying the fractional part by 64
	 * gets you closest to the REQUIRED baud rate.
	 *
	 * Baud rate =  (3 MHz)/(16* (m+n/64))
	 * m = interger part, n = fractional part
	 *
	 */
	_reg_write(UART0_IBRD, 1);
	_reg_write(UART0_FBRD, 40);
 
	// Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
	_reg_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
	// Mask all interrupts.
	_reg_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	// Enable UART0, receive & transfer part of UART.
	_reg_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
 
/*
 * @func: UART read
 *
 */
unsigned char uart_getc(void)
{
    // Wait for UART to have recieved something.
    while ( _reg_read(UART0_FR) & (1 << 4) ) { }
    return _reg_read(UART0_DR);
}
 
/*
 * @func: UART write
 *
 */
void uart_putc(unsigned char byte)
{
	// Wait for UART to become ready to transmit.
	while ( _reg_read(UART0_FR) & (1 << 5) ) { }
	_reg_write(UART0_DR, byte);
}
 
void uart_puts(const char* str)
{
        unsigned char size;

	size = strlen(str);
	for ( size_t i = 0; i < size; i++ )
	{
		uart_putc(str[i]);
	}
}
 
