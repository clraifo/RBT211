/*
 * THIS IS THE ONE THAT WORKS - KEEP
 * Light_Meter_6d.c
 *
 * Created: 7/3/2023 1:54:11 PM
 * Author : Casey R
 */ 

#ifndef F_CPU					// if F_CPU is not defined 
#define F_CPU 16000000UL			// define it as 16 MHz
#endif

#include <avr/io.h>				// allows use of I/O pins
#include <util/delay.h>				// allows use of _delay_ms() function

volatile uint8_t pwm_value = 0;			// variable to store the PWM value
volatile uint8_t pwm_counter = 0;		// variable to count the PWM value

// Function to map range of input values to output values, basically takes the input values from the pot or photoresistor and
// correlates them to PWM values for output ot the LEDs
uint8_t map_value(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void) {

	DDRD = 0b11111100;		// sets PD2 - PD7 to ouput
	DDRB = 0b00000011;		// sets PB0 and PB1 to output

	ADMUX = 0b01100000;		// sets the ADC input to PD6

	ADCSRA = 0b10100100;		// enables the ADC and sets the prescaler to 16

	ADCSRB = 0b00000000;		// sets the ADC to free running mode

	// sets the PWM mode to fast PWM and sets the prescaler to 8
	TCCR0A = TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1) | (1 << COM2B1);	//
	TCCR0B = TCCR2B = (1 << CS21);	

	ADCSRA |= (1 << ADSC);	// starts the ADC

	while (1) {
		// mapping the ADC value to the range of 0-255; these are nominal values that I made up (I have an O-scope but no idea how to use it yet)
		pwm_value = map_value(ADCH, 200, 20, 255, 0);	// map the ADC value to the PWM value

		OCR0A = OCR2A = pwm_value;	// set the PWM value

		if (pwm_counter < pwm_value) {
			PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);		// sets PD2 - PD7 high
			PORTB |= (1 << PB0) | (1 << PB1);								// sets PB0 & PB1 high
		}
		else {
			PORTD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));	// sets PD2 - PD7 low
			PORTB &= ~(1 << PB0) | (1 << PB1);								// sets PB0 & PB1 low
		}

		pwm_counter++;			// increment the counter
		_delay_us(10);			// delay for 10 microseconds
	}
	return(0);				// should never get here, this is to prevent a compiler warning
}
