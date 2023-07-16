/*

This `main.c` file is part of a project that implements an ultrasonic distance meter with an AVR microcontroller. The program uses an HC-SR04 ultrasonic sensor to 
measure distance and communicates the measurement results to an LCD display and a serial monitor.

Here is a step-by-step narrative of the program:

1. **Initialization**: The program starts by defining several macros for frequency (F_CPU), baud rate for serial communication (BAUD), and digital HIGH and LOW 
values. The necessary libraries are then included. The trigger (TRIG) and echo (ECHO) pins for the HC-SR04 ultrasonic sensor are defined as PB1 and PB2 respectively.

2. **UART Communication Setup**: The program defines functions for initializing UART communication (`uart_init`) and for sending strings (`uart_puts`) and integers 
(`uart_puti`, `uart_putlni`) over UART. These functions are used for sending data to the serial monitor.

3. **Pulse Reading**: The `pulseIn` function measures the duration of a HIGH or LOW pulse on a given pin. This function is used to measure the duration of the echo 
pulse from the HC-SR04 sensor, which is proportional to the distance measured by the sensor.

4. **Main Loop**: In the `main` function, the program first initializes UART communication and the LCD display. The TRIG pin is set as output and the ECHO pin as
input. The program then enters an infinite loop, where it triggers a measurement by sending a pulse on the TRIG pin, measures the duration of the returned echo pulse, 
calculates the distance in centimeters and inches, and displays the results on the LCD display and the serial monitor. The loop runs continuously with a delay of 
500 milliseconds between each measurement.
*/ 

#define F_CPU 16000000UL
#define BAUD 9600
#define HIGH 1
#define LOW 0
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
#include <util/setbaud.h>

#define TRIG PB1
#define ECHO PB2

// rest of your code...

void uart_init() {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= _BV(U2X0);
	#else
	UCSR0A &= ~(_BV(U2X0));
	#endif
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8-bit data
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); // Enable RX and TX
}

void uart_puts(char *s) {
	while (*s) {
		while (!(UCSR0A & _BV(UDRE0))) {} // Wait for empty transmit buffer
		UDR0 = *s;
		s++;
	}
}

void uart_puti(int n) {
	char buffer[10];
	itoa(n, buffer, 10);
	uart_puts(buffer);
}

void uart_putlni(int n) {
	uart_puti(n);
	uart_puts("\n");
}

unsigned long pulseIn(uint8_t pin, uint8_t state) {
	uint8_t mask = (1 << pin);
	unsigned long width = 0;
	// Wait for any previous pulse to end
	while ((PINB & mask) == state) {}
	// Wait for the pulse to start
	while ((PINB & mask) != state) {}
	// Then wait for the pulse to stop
	while ((PINB & mask) == state) {
		width++;
		_delay_us(1);
	}
	return width;
}

int main(void)
{
	char buffer[10];
	unsigned long duration;
	int distanceCm, distanceInch;

	uart_init();  // Initialize UART for serial communication
	lcd_init();
	
	DDRB |= (1<<TRIG); // Sets the TRIG_PIN as Output
	DDRB &= ~(1<<ECHO); // Sets the ECHO_PIN as Input
	while(1)
	{
		// Code to send the trigger pulse
		PORTB |= (1<<TRIG);
		_delay_us(10);
		PORTB &= ~(1<<TRIG);

		// Code to read the echo pulse and calculate distance
		duration = pulseIn(ECHO, HIGH);
		distanceCm= duration*0.034/2;
		distanceInch = duration*0.0133/2;

		// Send distance to LCD
		lcd_gotoxy(0,0);
		lcd_puts("Dist: ");
		lcd_puts(itoa(distanceCm, buffer, 10)); // Convert integer to string before sending to LCD.
		lcd_puts(" cm");
		lcd_gotoxy(0,1);
		lcd_puts("Dist: ");
		lcd_puts(itoa(distanceInch, buffer, 10));
		lcd_puts(" in");

		// Send distance to serial
		uart_puts("Duration: ");
		uart_putlni(duration);
		uart_puts("Distance cm: ");
		uart_putlni(distanceCm);
		uart_puts("Distance inch: ");
		uart_putlni(distanceInch);

		_delay_ms(500);
	}

}
