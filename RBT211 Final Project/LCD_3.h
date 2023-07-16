/*
The `lcd.h` file is a header file that contains the declarations for the functions and macros used to control a 16x2 LCD display with an AVR microcontroller. 
Here is a brief description of its contents:

1. **Macro Definitions**: The file begins by defining several macros for LCD control. These macros include definitions for the data and control ports and 
pins used to interface with the LCD (`LCD_DATA_DDR`, `LCD_DATA_PORT`, `LCD_CONTROL_DDR`, `LCD_CONTROL_PORT`, `RS`, `E`), and definitions for various LCD 
commands (`LCD_DISP_ON`, `LCD_DISP_OFF`, etc.) that are used to control the operation of the LCD.

2. **Function Declarations**: The file declares several functions for interacting with the LCD:
   - `lcd_init()`: This function initializes the LCD.
   - `lcd_command(unsigned char cmnd)`: This function sends a command to the LCD.
   - `lcd_data(unsigned char data)`: This function sends data to the LCD.
   - `lcd_puts(const char *s)`: This function displays a string on the LCD.
   - `lcd_gotoxy(unsigned char x, unsigned char y)`: This function moves the cursor to the specified position on the LCD.
   - `lcd_clrscr()`: This function clears the LCD screen.

These functions are defined in the `lcd.c` file, and they are used in the main program to control the LCD.tions and AVR I/O operations. They encapsulate the 
low-level details of interfacing with the LCD module.
*/

#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>

#define LCD_DATA_DDR DDRD
#define LCD_DATA_PORT PORTD
#define LCD_CONTROL_DDR DDRD
#define LCD_CONTROL_PORT PORTD
#define RS PD2
#define E PD3

#define LCD_DISP_ON 0x0C // Display on
#define LCD_DISP_OFF 0x08 // Display off
#define LCD_DISP_ON_CURSOR 0x0E // Display on, cursor on
#define LCD_DISP_ON_BLINK 0x0F // Display on, cursor off, blink on
#define LCD_DISP_ON_CURSOR_BLINK 0x0F // Display on, cursor on, blink on
#define LCD_HOME 0x02 // Move cursor to home position
#define LCD_ENTRY_MODE 0x06 // Shift cursor from left to right on read/write
#define LCD_FUNCTION_RESET 0x30 // Reset the LCD
#define LCD_FUNCTION_4BIT_2LINES 0x28 // 4-bit data, 2-line display, 5 x 7 font
#define LCD_SET_CURSOR 0x80 // Set cursor position
#define LCD_CURSOR_ON 0x0E // Cursor ON
#define LCD_CURSOR_OFF 0x0C // Cursor OFF
#define LCD_BLINK_CURSOR_ON 0x0D // Cursor blink ON
#define LCD_BLINK_CURSOR_OFF 0x0C // Cursor blink OFF
#define LCD_MOVE_CURSOR_LEFT 0x10 // Move cursor left without changing display data RAM
#define LCD_MOVE_CURSOR_RIGHT 0x14 // Move cursor right without changing display data RAM
#define LCD_TURN_ON 0x0C // Turn Lcd display on
#define LCD_TURN_OFF 0x08 // Turn Lcd display off
#define LCD_SHIFT_LEFT 0x18 // Shift display left without changing display data RAM
#define LCD_SHIFT_RIGHT 0x1C // Shift display right without changing display data RAM
#define LCD_RETURN_HOME 0x02 // Sets DDRAM address 0 in address counter. Also returns display from being shifted to original position. DDRAM contents remain unchanged.
#define LCD_CLR 0x01

#define LCD_CLEAR 0x01 // Clear LCD

#define LCD_LINE_1 0x80 // Start of line 1
#define LCD_LINE_2 0xC0 // Start of line 2

void lcd_init(void);
void lcd_command(unsigned char cmnd);
void lcd_data(unsigned char data);
void lcd_puts(const char *s);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_clrscr(void);


#endif /* LCD_H_ */
