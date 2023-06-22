/*
connect external LEDs to PD6 and PD7
connect button input to PD2
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED0 PD6        // This will set variable LED0 as PD6
#define LED1 PD7        // This will set variable LED1 as PD7
#define BUTTON PD2      // This will set variable BUTTON as PD2

int main(void)
{

    //This block configures the data direction register (DDR) and the port data register (PORT) for the LEDs and the button
    
    DDRD |= (1 << LED0) | (1 << LED1);  // Set LED0 and LED1 pins as outputs. This is done by setting the corresponding bits in DDRD to 1.
    DDRD &= ~(1 << BUTTON);             // Set BUTTON as input. This is done by setting the corresponding bit in DDRD to 0.
    PORTD |= (1 << BUTTON);             // Enable pull-up resistor for BUTTON. When the BUTTON pin is configured as an input, 
                                        //writing a 1 to the corresponding bit in PORTD will enable the internal pull-up resistor.


    // This block sets up Timer1 to generate an interrupt every second
    TCCR1A = 0;                                               // No special output mode needed, so all bits in TCCR1A are set to 0.
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);        // WGM12 bit is set to enable CTC (Clear Timer on Compare match) mode. 
                                                              // CS12 and CS10 are set to configure the prescaler as 1024.
    TIMSK1 = (1 << OCIE1A);                                   // OCIE1A bit is set in TIMSK1 to enable the output compare A match interrupt. 
                                                              // This will cause an interrupt to be triggered every time the timer value 
                                                              //matches the value in OCR1A.
    OCR1A = 15624;                                            // OCR1A is set to 15624. With a 16 MHz clock and a 1024 prescaler, the 
                                                              //timer will reach this value approximately every second 
                                                              //(16,000,000 / 1024 / 1 = 15625).


    // This block sets up external interrupt INT0 (which corresponds to digital pin 2 on the Arduino Uno) to trigger an 
    // interrupt when the button is pressed
    
    EICRA = (1 << ISC01);         // ISC01 bit is set in EICRA to configure INT0 to trigger an interrupt on the falling 
                                  //edge (when the button is pressed and the pin goes from high to low).
    EIMSK = (1 << INT0);          // INT0 bit is set in EIMSK to enable the external interrupt INT0.


    sei();

    while (1) 
    {
        // NO CODE SHOULD EXIST
    }

    return(0);  
}

ISR(TIMER1_COMPA_vect) {
    PORTD ^= (1 << LED0); // Toggle LED0
}

ISR(INT0_vect){
    if (!(PIND & (1 << BUTTON))) { // If button is pressed
        PORTD |= (1 << LED1); // Turn LED1 on
    } else { // If button is not pressed
        PORTD &= ~(1 << LED1); // Turn LED1 off
    }
}
