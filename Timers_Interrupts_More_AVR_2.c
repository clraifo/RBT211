/*
Two external LEDs alternate blinking in the default state. When the ISR is triggered by the pushbutton, they are turned off and the onboard LED blinks 
at the same rate. The blink rate of the LEDs can be adjusted by changing the value of the Output Compare Register A (OCR1A) for Timer1.

To increase the blink rate (make the LEDs blink more frequently), decrease the value of OCR1A. To decrease the blink rate (make the LEDs blink less 
frequently), increase the value of OCR1A. The actual blink rate can be calculated using the following formula:

Blink rate (in seconds) = (OCR1A + 1) * Prescaler / F_CPU (current settings = (15624 + 1) * 1024 / 16,000,000 ≈ 1 second)

- This uses Timer1 interrupt to alternate between the two external LEDs (PB6 and PB7)
- The button press is be handled with an external interrupt
- When the button is pressed, the external LEDs will turn OFF and the the on-board LED (PB5) starts blinking
- When the button is pressed again, the circuit returns to the default state of PB^ & PB7 alternating blinking and onboard LED OFF

This version maintains a global button_flag variable to keep track of whether the button is currently pressed. This flag determines which LED(s) to 
toggle in the Timer1 compare match ISR.

The INT0 ISR has been modified to check the button_flag. If the button was not previously pressed, it turns on both external LEDs and sets the flag. 
If the button was previously pressed, it turns off both external LEDs and the on-board LED, and clears the flag.
*/

#ifndef F_CPU			//Checks whether the clock is defined
#define F_CPU 16000000UL	//Sets it to 16mHz
#endif                          //Ends the if loop

#include <avr/io.h>		//Enables AVR I/O
#include <avr/interrupt.h>	//Enables use of interrupts

#define LED0 PD6                //Assigns LED0 to Pin PD6
#define LED1 PD7                //Assigns LED0 to Pin PD7
#define ONBOARD_LED PB5         //Assigns the onboard LED to PB5 (this is not really necessary)
#define BUTTON PD2              //Assigns te pushbutton to Pin PD2

volatile uint8_t button_flag = 0;    	//declares a volatile variable as an unsigned 8 bit integer, names it button_flag and
					//initializes it to 0

int main(void)
{
	PORTD |= (1 << LED0);     		// Turns LED0 ON
	PORTD &= ~(1 << LED1);    		// Turns LED1 OFF
	DDRD |= (1 << LED0) | (1 << LED1);     	//sets LED0/PD6 and LED1/PD7 to output
	DDRB |= (1 << ONBOARD_LED);            	//sets ONBOARD_LED/PB5 to output
	DDRD &= ~(1 << BUTTON);                	//sets BUTTON/PD2 to a pullup resistor
	PORTD |= (1 << BUTTON);

	TCCR1A = 0;             //Sets the Timer/Counter Control Register A to 0, disables all features controlled by TCCR1A, leaves
				//it in a simple counting mode. TCCR1A is in 15.11.1 in the datasheet.
	
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  	// Sets specific bits in the Timer/Counter Control Register B to control
								// the operation of Timer1. This sets bits for Clock Select, which  control
								// the prescaler for Timer1. By setting CS12 and CS10 (and leaving CS11
								// unset), it sets a prescaler of 1024. TCCR1B is in 15.11.2 in the datasheet.
	
	TIMSK1 = (1 << OCIE1A);         // Sets Output Compare Interrupt Enable 1 A bit (OCIE1A) in the Timer/Counter Interrupt Mask (TIMSK) Register
					// This enables the Output Compare A Match interrupt for Timer1, which is triggered whenever the timer
					// value matches the value stored in OCR1A (below).  TIMSK1 is in 15.11.8 in the datasheet.
	
	OCR1A = 15624;                  //Output Compare Register A (OCR1A) for Timer1 to 15624, which takes one second to hit at 16mHz
					//OCR1A is in 1701104 in the datasheet.

	EICRA = (1 << ISC01) | (1 << ISC00);     // Sets the ISC01 and ISC00 bits of the External Interrupt Control Register A to 1.
						// This defines the trigger condition for INT0 (below) to its rising edge (LOW to HIGH)
						//EICRA is in 12.2.1 in the datasheet.
	
	EIMSK = (1 << INT0);             // Sets  EIMSK register INT0 to 1, which enables external interrupts and triggers EICRA (above)
					// when the defined condition (rising edge, defined in AICRA above) is met. EIMSK is in 12.2.2 in the datasheet.

	sei();
	
	while (1)                        // Just an empty, endless loop. Kind of like life sometimes.
	{
	}

	return(0);
}

ISR(TIMER1_COMPA_vect) {     		// Interrupt Service Routine for the Timer1 Compare A vector. Is called whenever Timer1
					//reaches the value in OCR1A, triggering an interrupt.

	if (button_flag == 0) {         // Checks if the button_flag is equal to zero. Zero == the button is not pressed.

		PORTD ^= (1 << LED0) | (1 << LED1);		// Toggles the state of the LED0 and LED1. The '^=' operation is a bitwise XOR that
								// will flip the specific bits corresponding to the LED0 and LED1 pins in the PORTD register.
								// As a result, if an LED was on it turns off, and if it was off it turns on. Since these LEDs
								// were setup to blink alternately, this operation ensures that when one is on, the other is off.
								// They swap states with each timer tick.

		} else {                             		// If the button_flag is not zero, it means the button is currently being pressed.

		PORTB ^= (1 << ONBOARD_LED);    // Toggles the state of the onboard LED. This makes the onboard LED blink when the button is pressed.
	}
}


ISR(INT0_vect){						// ISR for the INT0 vector, . This ISR gets called the button is pressed or released
	if (button_flag == 0){				// Checks if the button_flag is equal to 0. This means the button was not previously pressed.
		PORTD &= ~((1 << LED0) | (1 << LED1));  // Turns OFF both external LEDs (LED0 and LED1). This is done by clearing the specific bits 
							// in the PORTD register corresponding to the pins of LED0 and LED1.		
		button_flag = 1;			// Changes the button_flag to 1, indicating that the button is now pressed.
		
		} else {                                // If the button_flag is not 0, this means the button was previously pressed.
		PORTD &= ~((1 << LED0) | (1 << LED1));  // Turns OFF both external LEDs (LED0 and LED1) by clearing their bits in the PORTD register.
		PORTB &= ~(1 << ONBOARD_LED);           // Turns OFF the onboard LED by clearing its bit in the PORTB register.
		
							// The next two lines resume the alternate blinking pattern.
		PORTD |= (1 << LED0);                   // Turns ON LED0 by setting its bit in the PORTD register.
		PORTD &= ~(1 << LED1);                  // Turns OFF LED1 by clearing its bit in the PORTD register.
		button_flag = 0;                        // Resets the button_flag to 0, indicating that the button is now released.
	}
}

}

