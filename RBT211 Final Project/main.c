/*
Main.c overview

This program reads output of an ultrasonic sensor and displays it on an LCD. Additionally, it features a warning 
function that alerts the user of approaching object by increasing the brightness of an LED

The preprocessor directive and inclusions are all pretty standard, with the exception of two libraries included to
control the LCD, LCD_3.c and LCD_3.c.

Definitions of some constants follow: TRIGGER_PIN, ECHO_PIN, MAX_ECHO_DURATION, MAX_DISTANCE, ERROR_DISTANCE, 
LED_PIN, PWM_CHANNEL, and PWM_TOP.

** setup() 
Sets the necessary pins as input or output, enables global interrupts, and configures Timer1 for PWM.

** measureDistance() 
Measures the distance using the ultrasonic sensor. It sends a trigger pulse, measures the duration of the echo pulse, 
and calculates the distance based on the pulse duration.

** main() 
Sets the CPU frequency, initializes the LCD and system using the setup() function, and enters an infinite loop.

Inside the loop, the distance is measured using the measureDistance() function.

The LED brightness is adjusted based on the distance. If no object is detected (distance is ERROR_DISTANCE), the LED 
is turned off by setting the PWM value to 0. Otherwise, the distance is mapped to the PWM range (0 to PWM_TOP) to control 
the LED brightness.

The distance is displayed on the LCD using the LCD_puts() function.

A delay of 500 milliseconds is added before the next distance measurement.

Controls of the LED brightness using PWM based on the measured distance from the ultrasonic sensor. 
As an object gets closer, the LED brightness increases, and as the object moves farther away, the LED brightness decreases. 
The distance is also displayed on the LCD for visual feedback.
*/

#ifndef F_CPU
#define F_CPU 16000000UL // Replace with your desired CPU frequency in Hertz
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD_3.h"

#define TRIGGER_PIN    2
#define ECHO_PIN       3
#define MAX_ECHO_DURATION 10000  // Maximum duration in microseconds for valid echo
#define MAX_DISTANCE 400         // Maximum distance in centimeters
#define ERROR_DISTANCE 9999      // Error value indicating an invalid or out-of-range distance
#define LED_PIN        6         // LED pin
#define PWM_CHANNEL    1         // PWM channel for LED control
#define PWM_TOP        255       // PWM top value (8-bit resolution)

void setup()
{
	// Initialize LCD
	LCD_init();

	// Set trigger pin as output
	DDRD |= (1 << TRIGGER_PIN);

	// Set echo pin as input
	DDRD &= ~(1 << ECHO_PIN);

	// Set LED pin as output
	DDRD |= (1 << LED_PIN);

	// Set PWM pin as output
	DDRD |= (1 << PD5); // Assuming OC1A pin is used for PWM

	// Enable global interrupts
	sei();

	// Configure Timer1 for PWM
	TCCR1A = (1 << COM1A1) | (1 << WGM10); // Non-inverted PWM mode, 8-bit resolution
	TCCR1B = (1 << WGM12) | (1 << CS11);   // Fast PWM mode, prescaler 8
	OCR1A = 0;                             // Initial PWM value
}

uint16_t measureDistance()
{
	// Send a 10us pulse to trigger the ultrasonic sensor
	PORTD |= (1 << TRIGGER_PIN);
	_delay_us(10);
	PORTD &= ~(1 << TRIGGER_PIN);

	// Measure the pulse duration
	uint32_t pulse_duration = 0;
	while (!(PIND & (1 << ECHO_PIN)))
	;
	TCNT1 = 0;
	TCCR1B |= (1 << CS10); // Start Timer1 with no prescaler
	while (PIND & (1 << ECHO_PIN))
	{
		// Check if the echo pulse exceeds the maximum duration
		if (TCNT1 > MAX_ECHO_DURATION)
		{
			// If the pulse exceeds the maximum duration, return an error value
			return ERROR_DISTANCE;
		}
	}
	pulse_duration = TCNT1;
	TCCR1B = 0; // Stop Timer1

	// Calculate distance in centimeters
	uint16_t distance = (pulse_duration * 0.034) / 2;

	return distance;
}

int main()
{
	// Set CPU frequency
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	// Initialize LCD
	LCD_init();

	// Initialize AVR
	setup();

	while (1)
	{
		// Measure distance
		uint16_t distance = measureDistance();

		// Adjust LED brightness based on distance
		uint8_t pwm_value = 0;
		if (distance == ERROR_DISTANCE)
		{
			pwm_value = 0; // No object detected, turn off LED
		}
		else
		{
			// Map the distance range to PWM range
			pwm_value = (uint8_t)((distance * PWM_TOP) / MAX_DISTANCE);
		}
		OCR1A = pwm_value; // Set PWM value

		// Display distance on LCD
		char buffer[16];
		if (distance == ERROR_DISTANCE)
		{
			snprintf(buffer, sizeof(buffer), "Distance: Error");
		}
		else
		{
			snprintf(buffer, sizeof(buffer), "Distance: %d cm", distance);
		}
		LCD_clear();
		LCD_gotoxy(0, 0);
		LCD_puts(buffer);

		_delay_ms(500); // Wait for a while before measuring again
	}

	return 0;
}
