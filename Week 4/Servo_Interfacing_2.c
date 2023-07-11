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

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

// Pulse values defined as per the datasheet (page 102)
#define PULSE_MIN 999U
#define PULSE_MAX 4799U
#define PULSE_MID 2899U

// Top value to achieve 50Hz frequency with 16MHz/8 = 2MHz timer clock
#define TOP_VALUE 39999

// Loop increment/decrement step
#define STEP 20

// Base delay between each servo position update
#define BASE_UPDATE_DELAY_MS 1

// Button pin
#define BUTTON_PIN PD2

// ADC channel for potentiometer
#define POT_CHANNEL 0

// Variable to keep track of whether the servo should be moving
volatile uint8_t isRunning = 0;

// Function to initialize ADC
void ADC_init()
{
	ADMUX |= (1 << REFS0); // Reference voltage is AVCC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // ADC clock prescaler /128
	ADCSRA |= (1 << ADEN); // Enable ADC
}

// Function to read ADC value
uint16_t ADC_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Select ADC channel
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
	return ADC; // Return ADC value
}

int main(void)
{
	// Initialize ADC
	ADC_init();

	ICR1 = TOP_VALUE;

	// Set timer/counter mode to fast PWM, with TOP defined by ICR1
	TCCR1A |= (1 << WGM11);
	TCCR1A |= (1 << COM1A1); // Set OC1A (PB1) as output compare pin
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler of 8

	DDRB |= (1 << PB1);

	// Configure button pin as input with pull-up
	DDRD &= ~(1 << BUTTON_PIN);
	PORTD |= (1 << BUTTON_PIN);

	OCR1A = PULSE_MIN;

	while (1)
	{
		// If button is pressed, toggle isRunning
		if (!(PIND & (1 << BUTTON_PIN)))
		{
			_delay_ms(50); // Debounce delay
			while (!(PIND & (1 << BUTTON_PIN))); // Wait for button release
			isRunning ^= 1; // Toggle isRunning
		}

		// If isRunning is set, move the servo
		if (isRunning)
		{
			uint16_t delay = ADC_read(POT_CHANNEL) / 1023.0 * 100 + BASE_UPDATE_DELAY_MS;

			// Move servo from minimum to maximum position
			for (uint16_t i = PULSE_MIN; i < PULSE_MAX; i += STEP)
			{
				OCR1A = i;
				for (uint16_t j = 0; j < delay; ++j)
				{
					_delay_us(1000); // 1 ms
				}
			}

			// Move servo from maximum to minimum position
			for (uint16_t i = PULSE_MAX; i > PULSE_MIN; i -= STEP)
			{
				OCR1A = i;
				for (uint16_t j = 0; j < delay; ++j)
				{
					_delay_us(1000); // 1 ms
				}
			}

			// Move servo to different positions with delay in between
			OCR1A = PULSE_MIN;
			for (uint16_t j = 0; j < delay; ++j)
			{
				_delay_us(1000); // 1 ms
			}
			OCR1A = PULSE_MID;
			for (uint16_t j = 0; j < delay; ++j)
			{
				_delay_us(1000); // 1 ms
			}
			OCR1A = PULSE_MAX;
			for (uint16_t j = 0; j < delay; ++j)
			{
				_delay_us(1000); // 1 ms
			}
			OCR1A = PULSE_MID;
			for (uint16_t j = 0; j < delay; ++j)
			{
				_delay_us(1000); // 1 ms
			}
		}
	}
}
