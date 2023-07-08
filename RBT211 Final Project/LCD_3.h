/*
LCD_3.h code overview :

**Library Dependencies
This library just has stdint.h, which provides definitions for fixed-size integer types

**Pin Definitions
The code defines the pins connected to the LCD module using macros. These definitions specify the data and control pins of the LCD 
module that are connected to the microcontroller.

**Function Prototypes
The code declares function prototypes for various LCD operations. These functions include initializing the LCD module, sending 
commands to the LCD, sending data to the LCD, and controlling the cursor position.

**Command Definitions
The code defines various commands used to control the LCD module. These commands are specific instructions sent to the LCD to 
perform various operations, such as clearing the display, turning on/off the display, and shifting the cursor.

Function Definitions
**The code provides inline function definitions for the LCD operations. These functions are implemented using the defined pin 
connections and AVR I/O operations. They encapsulate the low-level details of interfacing with the LCD module.

*/

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD pin configuration
#define LCD_RS_PIN     7   // Register Select
#define LCD_EN_PIN     8   // Enable
#define LCD_DATA_PORT  PORTB   // Data port
#define LCD_DATA_DIR   DDRB    // Data direction register

// LCD command codes
#define LCD_CMD_CLEAR_DISPLAY   0x01
#define LCD_CMD_RETURN_HOME     0x02
#define LCD_CMD_ENTRY_MODE      0x04
#define LCD_CMD_DISPLAY_CTRL    0x08
#define LCD_CMD_SHIFT           0x10
#define LCD_CMD_FUNCTION_SET    0x20
#define LCD_CMD_SET_CGRAM_ADDR  0x40
#define LCD_CMD_SET_DDRAM_ADDR  0x80

// LCD function set options
#define LCD_FUNCTION_8BIT		0x10
#define LCD_FUNCTION_4BIT		0x00
#define LCD_FUNCTION_2LINE		0x08
#define LCD_FUNCTION_1LINE		0x00
#define LCD_FUNCTION_5X10FONT   0x04
#define LCD_FUNCTION_5X8FONT    0x00

// LCD display control options
#define LCD_DISPLAY_ON			0x04
#define LCD_DISPLAY_OFF			0x00
#define LCD_CURSOR_ON			0x02
#define LCD_CURSOR_OFF			0x00
#define LCD_BLINK_ON			0x01
#define LCD_BLINK_OFF			0x00

// LCD entry mode options
#define LCD_ENTRY_INC			0x02
#define LCD_ENTRY_DEC			0x00
#define LCD_ENTRY_SHIFT			0x01
#define LCD_ENTRY_NOSHIFT		0x00

// LCD line addresses
#define LCD_LINE0_START			0x00
#define LCD_LINE1_START			0x40

// Function to initialize the LCD
void LCD_init();

// Function to send a command to the LCD
void LCD_send_cmd(uint8_t cmd);

// Function to send data to the LCD
void LCD_send_data(uint8_t data);

// Function to clear the LCD screen
void LCD_clear();

// Function to set the cursor position on the LCD
void LCD_gotoxy(uint8_t x, uint8_t y);

// Function to write a string to the LCD
void LCD_puts(const char *str);

#endif // LCD_H
