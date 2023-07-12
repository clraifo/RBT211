/*
This code initializes the ADC and configures the button pin as an input with a pull-up resistor.
It checks the button state in the main loop, and if the button is pressed, it toggles the isRunning
variable. The servo movement code is only executed if isRunning is set.

The ADC value read from the potentiometer is mapped to a range of 1ms to 100ms, and this value is
used as the delay between each servo position update.

This code implements a simple button debounce by waiting for 50ms after a button press is
detected. This might be sufficient, but in a more robust application, you might want to implement
a more sophisticated button debounce algorithm.
*/

#ifndef F_CPU				// Set the clock
#define F_CPU 16000000UL
#endif

#include <avr/io.h>			// Standard AVR IO library
#include <util/delay.h>		// Delay library

// Pulse values defined as per the datasheet (page 102)
#define PULSE_MIN 999U		// Minimum pulse width of 1ms
#define PULSE_MAX 4799U		// Maximum pulse width of 2.4ms
#define PULSE_MID 2899U		// Mid pulse width of 1.45ms

// Top value to achieve 50Hz frequency with 16MHz/8 = 2MHz timer clock
#define TOP_VALUE 39999		// 2MHz / 50Hz = 40000

#define STEP 20				// Loop increment/decrement step


#define BASE_UPDATE_DELAY_MS 1	// Base delay between each servo position update

#define BUTTON_PIN PD2		// Button pin is PD2

// ADC channel for potentiometer
#define POT_CHANNEL 0

// Variable to keep track of whether the servo should be moving
volatile uint8_t isRunning = 0;

// Function to initialize ADC
void ADC_init()
{
	ADMUX |= (1 << REFS0);									// Reference voltage is AVCC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// ADC clock prescaler /128
	ADCSRA |= (1 << ADEN);									// Enable ADC
}

// Function to read ADC value
uint16_t ADC_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | (channel & 0x07);	// Select ADC channel
	ADCSRA |= (1 << ADSC);						// Start conversion
	while (ADCSRA & (1 << ADSC));				// Wait for conversion to complete
	return ADC;									// Return ADC value
}

int main(void)
{
	// Initialize ADC
	ADC_init();

	ICR1 = TOP_VALUE;	// Set TOP value for timer/counter 1

	TCCR1A |= (1 << WGM11);									// Fast PWM, TOP = ICR1
	TCCR1A |= (1 << COM1A1);								// Set OC1A (PB1) as output compare pin
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);	// Prescaler of 8

	DDRB |= (1 << PB1);										// Configure OC1A (PB1) as output

	// Configure button pin as input with pull-up
	DDRD &= ~(1 << BUTTON_PIN);								// Configure button pin as input
	PORTD |= (1 << BUTTON_PIN);								// Enable pull-up resistor		

	OCR1A = PULSE_MIN;										// Set initial pulse width to minimum

	while (1)
	{
		// If button is pressed, toggle isRunning
		if (!(PIND & (1 << BUTTON_PIN)))
		{
			_delay_ms(50);							// Debounce delay
			while (!(PIND & (1 << BUTTON_PIN)));	// Wait for button release
			isRunning ^= 1;							// Toggle isRunning
		}

		// If isRunning is set, move the servo
		if (isRunning)
		{
			// Read ADC value and map to 1ms to 100ms
			uint16_t delay = ADC_read(POT_CHANNEL) / 1023.0 * 100 + BASE_UPDATE_DELAY_MS;	

			// Move servo from minimum to maximum position
			for (uint16_t i = PULSE_MIN; i < PULSE_MAX; i += STEP)
			{
				OCR1A = i;								// Set pulse width
				for (uint16_t j = 0; j < delay; ++j)	// Delay
				{
					_delay_us(1000);					// 1 ms
				}
			}

			// Move servo from maximum to minimum position
			for (uint16_t i = PULSE_MAX; i > PULSE_MIN; i -= STEP)	// Decrement pulse width
			{
				OCR1A = i;											// Set pulse width	
				for (uint16_t j = 0; j < delay; ++j)				// Delay
				{
					_delay_us(1000);								// 1 ms
				}
			}

			// Move servo to different positions with delay in between
			OCR1A = PULSE_MIN;							// Set pulse width to minimum
			for (uint16_t j = 0; j < delay; ++j)		// Delay
			{
				_delay_us(1000);						// 1 ms	
			}
			OCR1A = PULSE_MID;							// Set pulse width to mid	
			for (uint16_t j = 0; j < delay; ++j)		// Delay
			{
				_delay_us(1000);						// 1 ms
			}
			OCR1A = PULSE_MAX;							// Set pulse width to maximum
			for (uint16_t j = 0; j < delay; ++j)		// Delay
			{
				_delay_us(1000);						// 1 ms
			}
			OCR1A = PULSE_MID;							// Set pulse width to mid
			for (uint16_t j = 0; j < delay; ++j)		// Delay
			{
				_delay_us(1000);						// 1 ms
			}
			{
				_delay_us(1000); // 1 ms
			}
		}
	}
}
