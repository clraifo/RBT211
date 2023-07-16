/*
The `lcd.c` file contains the definitions of the functions declared in the `lcd.h` file for controlling a 16x2 LCD display with an AVR microcontroller. 
Here is a brief description of the contents of this file:

1. **Function Definitions**: The following functions are defined in this file:

   - `lcd_command(unsigned char cmnd)`: This function sends a command to the LCD. The command is first divided into its upper and lower nibbles (4 bits
   each), which are then sent to the LCD one by one.
   - `lcd_data(unsigned char data)`: This function sends data to the LCD. Similar to the `lcd_command` function, the data is divided into upper and 
   lower nibbles and sent one by one.
   - `lcd_init(void)`: This function initializes the LCD. It sets the data direction registers for the LCD data and control ports, and sends several
   commands to the LCD to initialize it and configure its settings.
   - `lcd_gotoxy(unsigned char x, unsigned char y)`: This function moves the cursor to the specified position on the LCD.
   - `lcd_puts(const char *s)`: This function displays a string on the LCD. It sends the characters of the string one by one using the `lcd_data` function.
   - `lcd_clrscr()`: This function clears the LCD screen. It sends the `LCD_CLEAR` command to the LCD and then waits for the command to be processed.

2. **Delay Functions**: The `_delay_us` and `_delay_ms` functions from the AVR `util/delay.h` library are used throughout this file to introduce delays 
between certain operations. These delays are necessary because some operations on the LCD take a certain amount of time to complete, and trying to perform 
another operation before the previous one has completed can cause errors.

These functions allow the microcontroller to control the LCD, sending commands to it, writing data to it, clearing its screen, and moving the cursor to 
different positions on the screen.
 */ 
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

void lcd_command(unsigned char cmnd) {
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd & 0xF0); // send upper nibble
	LCD_CONTROL_PORT &= ~(1<<RS); // RS=0, command reg.
	LCD_CONTROL_PORT |= (1<<E); // E=1
	_delay_us(1);
	LCD_CONTROL_PORT &= ~(1<<E); // E=0
	_delay_us(200);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd << 4); // send lower nibble
	LCD_CONTROL_PORT |= (1<<E);
	_delay_us(1);
	LCD_CONTROL_PORT &= ~(1<<E);
	_delay_ms(2);
}

void lcd_data(unsigned char data) {
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0); // send upper nibble
	LCD_CONTROL_PORT |= (1<<RS); // RS=1, data reg.
	LCD_CONTROL_PORT|= (1<<E);
	_delay_us(1);
	LCD_CONTROL_PORT &= ~(1<<E);
	_delay_us(200);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data << 4); // send lower nibble
	LCD_CONTROL_PORT |= (1<<E);
	_delay_us(1);
	LCD_CONTROL_PORT &= ~(1<<E);
	_delay_ms(2);
}

void lcd_init(void) {
	LCD_DATA_DDR |= 0xF0; // make PORT data direction register output
	LCD_CONTROL_DDR |= (1<<E) | (1<<RS); // make E and RS data direction register output
	_delay_ms(20); // LCD Power ON delay always >15ms
	lcd_command(0x02); // send for 4 bit initialization of LCD
	lcd_command(0x28); // 2 line, 5*7 matrix in 4-bit mode
	lcd_command(0x0C); // Display on cursor off
	lcd_command(0x06); // Increment cursor (shift cursor to right)
	lcd_command(0x01); // Clear display screen
	_delay_ms(2);
}

void lcd_gotoxy(unsigned char x, unsigned char y) {
	if (y == 1)
	lcd_command(0x80 + x);
	else if (y == 2)
	lcd_command(0xC0 + x);
}

void lcd_puts(const char *s) {
	while (*s)
	lcd_data(*s++);
}

void lcd_clrscr() {
	lcd_command(LCD_CLEAR);
	_delay_ms(2);
}



