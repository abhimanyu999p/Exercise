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

#define LCD_RS    7
#define LCD_E     8
#define LCD_D4   25
#define LCD_D5   24
#define LCD_D6   23
#define LCD_D7   18
#define LED       4  

#define LCD_CMD_FOUR_BIT_MODE           0x28
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF   0x0C
#define LCD_CMD_CLR_DISPLAY             0x01
#define LCD_CMD_CURSOR_FIRST_LINE       0x80
#define LCD_CMD_CURSOR_SECOND_LINE      0xC0

void lcd_init(void);
void lcd_writeCmd(unsigned char);
void lcd_writeData(const char*);
void lcd_writeByte(unsigned char);
void blink(uint32_t gpio);

int myMain(void)
{
   uart_init();
   uart_puts("Hello World! ...\n");

   lcd_init();
   lcd_writeData("Hello Sir");
   //delay(0x100000);
   //lcd_writeCmd(LCD_CMD_CURSOR_SECOND_LINE);
   //lcd_writeData("LCD on raspberry");
   //delay(0x100000);

   while(1)
   {
      blink(LED);
   }

   return 0;
}

void lcd_init()
{
   gpio_init(LCD_RS);
   gpio_init(LCD_E);
   gpio_init(LCD_D4);
   gpio_init(LCD_D5);
   gpio_init(LCD_D6);
   gpio_init(LCD_D7);
   gpio_init(LED);

//lcd_writeCmd(0xC5);
   lcd_writeCmd(LCD_CMD_FOUR_BIT_MODE);
   lcd_writeCmd(LCD_CMD_DISPLAY_ON_CURSOR_OFF);
   lcd_writeCmd(LCD_CMD_CLR_DISPLAY);
   lcd_writeCmd(LCD_CMD_CURSOR_FIRST_LINE);
}

void lcd_writeCmd(unsigned char byte)
{  
   //GPIO_SET(LCD_E);
   GPIO_CLR(LCD_RS);

   lcd_writeByte(byte);
   delay(1000);

   //GPIO_CLR(LCD_E);

}

void lcd_writeData(const char *str)
{
   //GPIO_SET(LCD_E);
   GPIO_SET(LCD_RS);

   for ( size_t i = 0; i < strlen(str); i++ )
   {
      lcd_writeByte(str[i]);
      delay(1000);
   }
   //GPIO_CLR(LCD_E);

}

void lcd_writeByte(unsigned char byte)
{

   ((byte & 0x80) == 0x80) ? GPIO_SET(LCD_D7) : GPIO_CLR(LCD_D7);
   ((byte & 0x40) == 0x40) ? GPIO_SET(LCD_D6) : GPIO_CLR(LCD_D6);
   ((byte & 0x20) == 0x20) ? GPIO_SET(LCD_D5) : GPIO_CLR(LCD_D5);
   ((byte & 0x10) == 0x10) ? GPIO_SET(LCD_D4) : GPIO_CLR(LCD_D4);

   GPIO_SET(LCD_E);
   delay(50);
   GPIO_CLR(LCD_E);
   delay(50);
   
   ((byte & 0x08) == 0x08) ? GPIO_SET(LCD_D7) : GPIO_CLR(LCD_D7);
   ((byte & 0x04) == 0x04) ? GPIO_SET(LCD_D6) : GPIO_CLR(LCD_D6);
   ((byte & 0x02) == 0x02) ? GPIO_SET(LCD_D5) : GPIO_CLR(LCD_D5);
   ((byte & 0x01) == 0x01) ? GPIO_SET(LCD_D4) : GPIO_CLR(LCD_D4);

   GPIO_SET(LCD_E);
   delay(50);
   GPIO_CLR(LCD_E);
   delay(50);
}

void blink(uint32_t gpio)
{
	// Turn ON LED connected to GPIO @ ...
	GPIO_SET(gpio);

	delay(0x100000);
	// Turn OFF LED connected to GPIO @ ... 
	GPIO_CLR(gpio);
	
	delay(0x100000);
}
