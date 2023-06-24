/*
This one WORKS! KEEP IT!
*/

#define F_CPU 16000000UL	// Set the CPU clock speed to 16MHz

#include <avr/io.h>			// Standard AVR header, allows I/O
#include <util/delay.h>		// Allows for use of _delay_ms() function
#include <avr/interrupt.h>	// Allows for use of interrupts

#define MAX_PWM 255		// 100% of max brightness
#define HALF_PWM 127	// 50% of max brightness	
#define MIN_PWM 0		// 0% of max brightness
#define QUARTER_PWM 64  // 25% of max brightness

int main(void)	// Main function
{

	DDRD |= (1 << PORTD6);	//Set PD6 as output
	DDRD |= (1 << PORTD5);	//Set PD5 as output

	DDRB |= (1 << PORTB5);		//Set PB5 as output
	PORTB &= ~(1 << PORTB5);	//Set PB5 to low

	// Set Timer0 for Fast PWM mode and normal port operation, clear OC0A/OC0B on compare match
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);	//Set Timer0 for Fast PWM mode and normal port operation, clear OC0A/OC0B on compare match

	// Set prescaler to 64 and start the timer
	TCCR0B = (1 << CS01) | (1 << CS00);

	while (1)	
	{
		for (uint8_t i = MIN_PWM; i < HALF_PWM; i++)	// Fade in PD6 to half brightness
		{
			OCR0A = i;	// Set the value for OC0A (PD6) PWM duty cycle
			_delay_ms(2);	// Delay for 2 milliseconds
		}

		for (uint8_t i = HALF_PWM; i > MIN_PWM; i--)	// Fade out PD6
		{
			OCR0A = i;  	// Set the value for OC0A (PD6) PWM duty cycle
			_delay_ms(2);	// Delay for 2 milliseconds
		}

		for (uint8_t i = MIN_PWM; i < MAX_PWM; i++)		// Fade in PD5 to full brightness
		{
			OCR0B = i;	// Set the value for OC0B (PD5) PWM duty cycle
			_delay_ms(2);	// Delay for 2 milliseconds
		}


		for (uint8_t i = MAX_PWM; i > MIN_PWM; i--) 		// Fade out PD5
		{
			OCR0B = i;	// Set the value for OC0B (PD5) PWM duty cycle
			_delay_ms(2);	// Delay for 2 milliseconds
		}
	}

	return 0;	
}
