/*
LCD_#.c Overview

The LCD.c library provides functions to control and communicate with an LCD (Liquid Crystal Display) module. 

** LCD_send_cmd(uint8_t cmd)
This function sends a command to the LCD module. It takes a single-byte command as an argument and performs the 
necessary operations to send the command to the LCD.

** LCD_send_data(uint8_t data)
This function sends data to the LCD module. It takes a single-byte data value as an argument and sends it to the LCD.

** LCD_init()
This function initializes the LCD module. It sets the data and control pins as outputs, waits for the LCD to power up, and 
performs the initialization sequence required by the LCD.

** LCD_clear()
This function clears the LCD display by sending the appropriate command to the LCD module.

**LCD_gotoxy(uint8_t x, uint8_t y)
This function sets the cursor position on the LCD display. It takes the x and y coordinates (zero-based) as 
arguments and calculates the corresponding DDRAM (Display Data RAM) address to set the cursor position.

** LCD_puts(const char *str)
This function sends a null-terminated string to the LCD. It takes a pointer to a string as an argument and iterates 
through each character, sending it to the LCD using the LCD_send_data() function.

Library dependencies are avr/io.h for AVR microcontroller I/O operations and util/delay.h for creating delays in 
microseconds and milliseconds. Additionally, it includes the LCD_3.h header file, which defines constants and pin configurations specific to the 
LCD module being used.

*/

#ifndef F_CPU
#define F_CPU 16000000UL // Replace with your desired CPU frequency in Hertz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "LCD_3.h"

// Function to send a command to the LCD
void LCD_send_cmd(uint8_t cmd)
{
	// Set RS pin LOW to indicate a command
	LCD_DATA_PORT = cmd;
	PORTD &= ~(1 << LCD_RS_PIN);

	// Enable pulse
	PORTD |= (1 << LCD_EN_PIN);
	_delay_us(1);
	PORTD &= ~(1 << LCD_EN_PIN);

	// Delay after each command
	_delay_us(100);
}

// Function to send data to the LCD
void LCD_send_data(uint8_t data)
{
	// Set RS pin HIGH to indicate data
	LCD_DATA_PORT = data;
	PORTD |= (1 << LCD_RS_PIN);

	// Enable pulse
	PORTD |= (1 << LCD_EN_PIN);
	_delay_us(1);
	PORTD &= ~(1 << LCD_EN_PIN);

	// Delay after each data transmission
	_delay_us(100);
}

void LCD_init()
{
	// Set data and control pins as output
	LCD_DATA_DIR = 0xFF;
	DDRD |= (1 << LCD_RS_PIN) | (1 << LCD_EN_PIN);

	// Wait for LCD to power up
	_delay_ms(20);

	// Initialization sequence
	LCD_send_cmd(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_4BIT);
	_delay_us(4500);

	LCD_send_cmd(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_4BIT);
	_delay_us(150);

	LCD_send_cmd(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_4BIT);
	LCD_send_cmd(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINE | LCD_FUNCTION_5X8FONT);

	LCD_send_cmd(LCD_CMD_DISPLAY_CTRL | LCD_DISPLAY_ON);
	LCD_send_cmd(LCD_CMD_CLEAR_DISPLAY);
	_delay_ms(2);

	LCD_send_cmd(LCD_CMD_ENTRY_MODE | LCD_ENTRY_INC);
}

void LCD_clear()
{
	LCD_send_cmd(LCD_CMD_CLEAR_DISPLAY);
	_delay_ms(2);
}

void LCD_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t address;

	switch (y)
	{
		case 0:
		address = LCD_LINE0_START + x;
		break;
		case 1:
		address = LCD_LINE1_START + x;
		break;
		default:
		return;
	}

	LCD_send_cmd(LCD_CMD_SET_DDRAM_ADDR | address);
}

void LCD_puts(const char *str)
{
	while (*str)
	{
		        LCD_send_data(*str);
		        str++;
	        }
        }
