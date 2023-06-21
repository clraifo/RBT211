/* External_Interrupts.c
 * Created: 6/20/2023 6:25:28 PM
 * Author : casey.raiford */ 

#ifndef F_CPU
#define F_CPU 16000000UL 
#endif

#include <avr/io.h>
#include <util/delay.h>	
#include <avr/interrupt.h>

int main(void)
{
    DDRB |= (1 << PB5) | (1 << PB0); // Port B data direction register = 0b00100001
    DDRD &= ~(1 << PD2); // Sets PD2 to an input pin
    PORTD |= (1 << PD2); // Sets PD2 to HIGH, making it a pull-up resistor
    DDRD |= (1 << PD7); // Set pin PD7 as output

    EICRA |= (1 << ISC00); // Trigger INT0 on any edge (falling and rising)
    EICRA &= ~(1 << ISC01);

    EIMSK |= (1 << INT0); // Sets the INT0 bit in the EIMSK register to 1. This will enable the INT0 interrupt.

    sei(); // Enables global interrupts
	     
    while (1) // Kicks off a loop to blink the external LED
    {
        PORTD ^= (1 << PD7); // LED toggler, flips every half second per the delay
        _delay_ms(500); // Delay for 500 milliseconds
    } 

    return(0);
}

ISR(INT0_vect) // Interrupt Service Routine starts here
{ 
	if (!(PIND & (1 << PD2))) // Reads whether PD2 is LOW
	{
		PORTB |= (1 << PB5); // Writes the onboard LED to HIGH if it is
	} 
	else 
	{
		PORTB &= ~(1 << PB5); // Writes the onboard LED to LOW if PD2 is HIGH
	}
}
