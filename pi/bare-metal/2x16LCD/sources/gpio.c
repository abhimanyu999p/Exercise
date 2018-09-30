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

static inline uint32_t GPIO_FUNCTION_SELECT(uint32_t gpio)
{
	switch (gpio) {
		case 0 ... 9:
			return GPFSEL0;
		case 10 ... 19:
			return GPFSEL1;
		case 20 ... 29:
			return GPFSEL2;
		case 30 ... 39:
			return GPFSEL3;
		case 40 ... 49:
			return GPFSEL4;
		case 50 ... 53:
			return GPFSEL5;
		default:	
			return 0;
	}
}	

void GPIO_SET(uint32_t gpio)
{
	switch (gpio) {
		case 0 ... 31:
			_reg_write(GPSET0, (1 << gpio));
		case 32 ... 53:
			_reg_write(GPSET1, (1 << (gpio%32)));
	}
}

void GPIO_CLR(uint32_t gpio)
{
	switch (gpio) {
		case 0 ... 31:
			_reg_write(GPCLR0, (1 << gpio));
		case 32 ... 53:
			_reg_write(GPCLR1, (1 << (gpio%32)));
	}
}

/*
 * @fn: Init GPIO number
 *
 * @params
 * 	gpio - configure the function select to GPIO
 *
 */ 
void gpio_init(uint32_t gpio)
{
	uint32_t reg;

	reg = _reg_read(GPIO_FUNCTION_SELECT(gpio));
	reg &= ~(FLD_MASK << (gpio%10)*3);
	reg |= (DIR_OUT << (gpio%10)*3);
	_reg_write(GPIO_FUNCTION_SELECT(gpio), reg);

}

